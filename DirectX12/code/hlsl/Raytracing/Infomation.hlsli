/* �G���g���[�|�C���g�̈��� */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID        : SV_GroupID;
    uint3 dispatch_ID     : SV_DispatchThreadID;
    uint group_index      : SV_GroupIndex;
};

/* �J������� */
struct Camera
{
    /* �ʒu */
    float3 pos;
    /* ����p */
    float fov;
    /* �����_ */
    float3 target;
    /* �����Y�̑傫�� */
    float lens_aperture;
    /* ����� */
    float3 up;
    /* �A���C�����g */
    float alignment;
};

/* ���C��� */
struct Ray
{
    /* �n�_���W */
    float3 pos;
    /* �ړ����� */
    float3 direction;
    /* ���[�V�����u���[�^�C�~���O */
    float motion_blur_timing;
    
    /* ���C�̈ʒu���擾 */
    float3 GetPos(in float time)
    {
        /* �����̕����� */
        return pos + (time * direction);
    }
};

/* �}�e���A����� */
#define MATERIAL_NONE    0
#define MATERIAL_LAMBERT 1
#define MATERIAL_REFLECT 2
#define MATERIAL_REFRACT 3

/* �q�b�g��� */
struct Hit
{
    /* �q�b�g�^�C�~���O */
    float time;
    /* �q�b�g���W */
    float3 pos;
    /* �@���x�N�g�� */
    float3 normal;
    /* �q�b�g�J���[ */
    float3 color;
    /* �}�e���A����� */
    uint material;
    /* �q�b�gUV���W */
    float2 uv;
};

/* ���� */
struct Sphere
{
    /* ���S���W */
    float3 center;
    /* ���a */
    float radius;
    /* �J���[ */
    float3 color;
    /* �}�e���A����� */
    uint material;
    /* �ړ��� */
    float3 velocity;
    /* �A���C�����g */
    float alignment;
    
    /*�� ���a�̎擾 
     * @param pos �ʒu
     * @return �~�̔��a
     */
    float GetRadius(in float3 pos)
    {
        float3 tmp = pos - center;

        /* ���̕����� */
        return sqrt(dot(tmp, tmp));
    }
    /*�� ���S�ʒu�̎擾
     * @param ray ���C�̏��
     * @param �ړ��J�n����
     * @param �ړ��I������
     * @return �~�̒��S�ʒu
     */
    float3 GetCenter(in Ray ray, in float move_start = 0.0f, in float move_end = 1.0f)
    {
        /* �ړ��Ԋu */
        float period = move_end - move_start;
        
        return center + (((period * ray.motion_blur_timing) - move_start) / period) * velocity;
    }
    /*�� �q�b�g���� 
     * @param ray ���C�̏��
     * @param hit �q�b�g���̊i�[��
     * @param min_distance ���C�̍ŒZ����
     * @param max_distance ���C�̍ő勗��
     * @return true:�q�b�g����@false:�q�b�g�Ȃ�
     */
    bool IsHit(in Ray ray, out Hit hit, in float min_distance = 0.001f, in float max_distance = float(0xFFFFFFFF))
    {
        /* ���݂̒��S�ʒu */
        float3 now_center = GetCenter(ray);
        
        /* ax^2 + bx + c = 0 */
        float3 oc = ray.pos - now_center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0f * dot(ray.direction, oc);
        float c = dot(oc, oc) - (radius * radius);
        
        bool flag = true;
        
        /* ���莮 */
        float D = (b * b) - (4.0f * a * c);
        if (D > 0.0f)
        {
            /* �q�b�g�^�C�~���O���Z�o */
            float time = (-b - sqrt(D)) / (2.0f * a);
            /* �q�b�g�^�C�~���O���͈͓����m�F */
            if (!(min_distance < time && time < max_distance))
            {
                time = (-b + sqrt(D)) / (2.0f * a);
                if (!(min_distance < time && time < max_distance))
                {
                    flag = false;
                }
            }
            
            if (flag == true)
            {
                hit.time     = time;
                hit.pos      = ray.GetPos(hit.time);
                hit.normal   = (hit.pos - now_center) / radius;
                hit.color    = color;
                hit.material = material;
                
                float pi    = acos(-1.0f);
                float phi   = atan2(hit.normal.z, hit.normal.x);
                float theta = asin(hit.normal.y);
                
                hit.uv = float2(1.0f - (phi + pi) / (2.0f * pi), ((theta + pi) / 2.0f) / pi);
                
                return true;
            }
        }
        
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        
        return false;
    }
};

/*�� �����_���l�̎擾 
 * @param uv �K���Ȑ��l
 * @param seed �V�[�h�l
 * @return �����_���l(0.0f�`1.0f)
 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/*�� �񎟌��ȃ����_���l�̎擾 
 * @param uv �K���Ȑ��l
 * @param seed �V�[�h�l
 * @return �����_���l(0.0f�`1.0f)
 */
float2 Random2(in float2 uv, in int seed = 0)
{
    float2 tmp = float2(dot(uv, float2(127.1f, 311.7f)), dot(uv, float2(269.5f, 183.3f)));
    
    return 2.0f * frac(sin(tmp + seed) * 43758.5453123f) - 1.0f;
}

/** �����o�[�g���� 
 * @param ray ���C�̏��
 * @param hit �q�b�g���̊i�[��
 * @param specularity ���ʗ�(0.0f�`1.0f)
 * @param loop �͍���
 * @return �J���[�l(0.0f�`1.0f)
 */
float3 LambertReflect(in out Ray ray, in Hit hit, in float specularity, in uint loop = 10)
{
    /* �q�b�g�ʒu���甽�˕����ɒP�ʋ�������Ɖ��肵�A���ˈʒu���P�ʋ����ɂ���܂Ń��[�v */
    float3 random = 0.0f;
    random.x = 2.0f * Random(hit.pos.xy) - 1.0f;
    random.y = 2.0f * Random(hit.pos.yz) - 1.0f;
    random.z = 2.0f * Random(hit.pos.zx) - 1.0f;
    
    float value = float(0xffffffff);
    
    for (uint i = 0; i < loop; ++i)
    {
        random.x = 2.0f * Random(random.xy, i) - 1.0f;
        random.y = 2.0f * Random(random.yz, i) - 1.0f;
        random.z = 2.0f * Random(random.zx, i) - 1.0f;
        
        //float a = acos(-1.0f) * Random(random.xy, i);
        //float z = 2.0f * Random(random.yz, i) - 1.0f;
        //float r = sqrt(1.0f - (z * z));
        
        //random.x = r * cos(a);
        //random.y = r * sin(a);
        //random.z = z;
        
        float tmp = pow(length(random), 2.0f);
        if (tmp < 1.0f)
        {
            break;
        }
        else
        {
            value = min(tmp, value);
        }
    }
    
    if (!(dot(random, hit.normal) > 0.0f))
    {
        random = -random;
    }
    
    ray.pos = hit.pos;
    ray.direction = hit.normal + random;
    
    return specularity * hit.color;
}

/*�� ����
 * @param direction ���C�̕���
 * @param normal �@��
 * @return ���˕���
 */
float3 Reflect(in float3 direction, in float3 normal)
{
    return reflect(normalize(direction), normal);
    
    /* ���˃x�N�g���Ɩ@���x�N�g�����P�ʃx�N�g���̏ꍇ�̔��� */
    return direction + 2.0f * normal;
    /* �@���x�N�g�������P�ʃx�N�g���̏ꍇ�̔��� */
    //return direction - 2.0f * dot(direction, normal) * normal;
}

/** ���ʔ��� 
 * @param ray ���C�̏��
 * @param hit �q�b�g���̊i�[��
 * @param specularity ���ʗ�(0.0f�`1.0f)
 * @param fazz �ڂ₩��(0.0f�`1.0f)
 * @param loop �͍���
 */
float3 SpecularReflection(in out Ray ray, in Hit hit, in float specularity, in float fazz = 0.0f, in uint loop = 10)
{
    ray.pos = hit.pos;
    ray.direction = Reflect(ray.direction, hit.normal);
    
    /* �ڂ������� */
    if (fazz > 0.0f)
    {
        float max = float(0xffffffff);
        float3 random = 0.0f;
        random.x = 2.0f * Random(hit.pos.xy) - 1.0f;
        random.y = 2.0f * Random(hit.pos.yz) - 1.0f;
        random.z = 2.0f * Random(hit.pos.zx) - 1.0f;
        
        for (uint i = 0; i < loop; ++i)
        {
            random.x = 2.0f * Random(random.xy, i) - 1.0f;
            random.y = 2.0f * Random(random.yz, i) - 1.0f;
            random.z = 2.0f * Random(random.zx, i) - 1.0f;
            
            float tmp = pow(length(random), 2.0f);
            if (tmp < 1.0f)
            {
                break;
            }
            else
            {
                max = min(max, tmp);
            }
        }
        
        ray.direction += min(fazz, 1.0f) * random;
    }
    
    return specularity * hit.color;
}

/** �t���l���̖@�� 
 * @param refract ���ܗ�
 * @param cos �R�T�C���l
 * @return �t���l���l
 */
float Fresnel(in float refract, in float cos)
{
    /* Schlick�̋ߎ��� */
    float tmp = (1.0f - refract) / (1.0f + refract);
    tmp *= tmp;

    return tmp + (1.0f - tmp) * pow((1.0f - cos), 5.0f);
}

/** ���� 
 * @param ray ���C�̏��
 * @param hit �q�b�g���̊i�[��
 * @param refract ���ܗ�
 * @return �J���[�l(0.0f�`1.0f)
 */
float3 Refraction(in out Ray ray, in Hit hit, in float refract)
{
    float rate    = 1.0f / refract;
    float3 vec    = normalize(ray.direction);
    float3 normal = hit.normal;
    if (dot(vec, normal) > 0.0f)
    {
        rate = refract;
        normal = -normal;
    }
    
    float cos_theta = min(dot(-vec, normal), 1.0f);
    float random = 1.0f;
    
    /* ���莮 
    * �X�l���̖@��
    */
    float D = 1.0f - (rate * rate) * (1.0f - (cos_theta * cos_theta));
    if (D > 0.0f)
    {
        random = Fresnel(refract, cos_theta);
    }
    
    if (random < Random(ray.pos.xy))
    {
        ray.pos = hit.pos;
        ray.direction = rate * (vec + (cos_theta * normal)) - normal * sqrt(D);
    }
    else
    {
        ray.pos = hit.pos;
        ray.direction = Reflect(ray.direction, hit.normal);
    }
    
    return float3(1.0f, 1.0f, 1.0f);
}

/*�� ��ʊE�[�x�̃I�t�Z�b�g���擾 
 * @param cam �J�������
 * @param uv UV���W(0.0f�`1.0f)
 * @param loop �͍���
 */
float3 GetDepthOfFieldOffset(in Camera cam, in float2 uv, in uint loop = 10)
{
    float max = float(0xffffffff);
    float3 random = 0.0f;
    random.x = 2.0f * Random(uv) - 1.0f;
    random.y = 2.0f * Random(uv) - 1.0f;
    random.z = 2.0f * Random(uv) - 1.0f;
        
    for (uint i = 0; i < loop; ++i)
    {
        random.x = 2.0f * Random(random.xy, i) - 1.0f;
        random.y = 2.0f * Random(random.yz, i) - 1.0f;
        random.z = 2.0f * Random(random.zx, i) - 1.0f;
            
        float tmp = pow(length(float3(random.x, random.y, 0.0f)), 2.0f);
        if (tmp < 1.0f)
        {
            break;
        }
        else
        {
            max = min(max, tmp);
        }
    }
    
    float3 dir = normalize(cam.pos - cam.target);
    float3 u = normalize(cross(cam.up, dir));
    float3 v = cross(dir, u);
    
    return (u * random.x * (cam.lens_aperture / 2.0f)) + (v * random.y * (cam.lens_aperture / 2.0f));
}

/*�� ���j�A�F��Ԃɕϊ� 
 * @param color �J���[�l(0.0f�`1.0f)
 * @return �K���}�␳�K�p�J���[�l(0.0f�`1.0f)
 */
float3 Linear(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/*�� sRGB�F��Ԃɕϊ� 
 * @param color �J���[�l(0.0f�`1.0f)
 * @return SRGB�␳��K�p�����J���[�l(0.0f�`1.0f)
 */
float3 sRGB(in float3 color)
{
    return pow(color, 2.2f);
}

/*�� ���W�A���p�x�ɕϊ� 
 * @param angle �ʓx�@(0.0f�`360.0f)
 * @return ���W�A���p�x
 */
float Radian(in float angle)
{
    return angle * acos(-1.0f) / 180.0f;
}

/** �����_���m�C�Y
 * @param uv �K���Ȑ��l(0.0f�`1.0f)
 * @param seed �V�[�h�l
 * @return �����_���l(0.0f�`1.0f)
 */
float RandomNoise(in float2 uv, in int seed = 0)
{
    return 2.0f * Random(uv, seed) - 1.0f;
}

/** �u���b�N�m�C�Y
 * @param uv �K���Ȓl(0.0f�`1.0f)
 * @param block_num �u���b�N�̐�
 * @param seed �V�[�h�l
 * @return �����_���l(0.0f�`1.0f)
 */
float BlockNoise(in float2 uv, in float block_num, in int seed = 0)
{
    /* �������̒��o */
    float2 integer = float2(floor(uv.x * block_num), floor(uv.y * block_num));
    
    return RandomNoise(integer, seed);
}

/** �o�����[�m�C�Y 
 * @param uv �K���Ȑ��l(0.0f�`1.0f)
 * @param block_num �u���b�N�̐�
 * @param seed �V�[�h�l
 * @return �����_���l(0.0f�`1.0f)
 */
float ValueNoise(in float2 uv, in float block_num, in int seed = 0)
{
    /* �g���̃T�C�Y */
    float2 size = uv * block_num;
    
    /* �������̒��o */
    float2 integer = float2(floor(size.x), floor(size.y));
    /* �������̒��o */
    float2 decimal = float2(frac(size.x), frac(size.y));
    
   /* ��Ԋ�l(�O���G���~�[�g�Ȑ�) */
    float2 interpolation = -(2.0f * pow(decimal, 3.0f)) + (3.0f * pow(decimal, 2.0f));
    
    /* �ŏ��l */
    float2 min_value = float2(Random(integer + float2(0.0f, 0.0f)), Random(integer + float2(0.0f, 1.0f)));
    /* �ő�l */
    float2 max_value = float2(Random(integer + float2(1.0f, 0.0f)), Random(integer + float2(1.0f, 1.0f)));
    
    /* X���ɑ΂����Ԓl */
    float2 lerp_value = float2(lerp(min_value.x, max_value.x, interpolation.x), lerp(min_value.y, max_value.y, interpolation.x));
    
    /* Y���ɑ΂����Ԓl */
    return lerp(lerp_value.x, lerp_value.y, interpolation.y);
}

/** �p�[�����m�C�Y
 * @param uv �K���Ȑ��l(0.0f�`1.0f)
 * @param block_num �u���b�N��
 * @param seed �V�[�h�l
 * @return �����_���l
 */
float PerlinNoise(in float2 uv, in float block_num, in int seed = 0)
{
    /* �g���̃T�C�Y */
    float2 size = uv * block_num;
    
    /* �������̒��o */
    float2 integer = float2(floor(size.x), floor(size.y));
    /* �������̒��o */
    float2 decimal = float2(frac(size.x), frac(size.y));
    
    /* ��Ԋ�l(�O���G���~�[�g�Ȑ�) */
    float2 interpolation = -(2.0f * pow(decimal, 3.0f)) + (3.0f * pow(decimal, 2.0f));

    /* ���z�x�N�g�� */
    float2 gradient_1 = Random2(integer + float2(0.0f, 0.0f), seed);
    float2 gradient_2 = Random2(integer + float2(1.0f, 0.0f), seed);
    float2 gradient_3 = Random2(integer + float2(0.0f, 1.0f), seed);
    float2 gradient_4 = Random2(integer + float2(1.0f, 1.0f), seed);
    
    /* �����x�N�g�� */
    float2 distance_1 = decimal - float2(0.0f, 0.0f);
    float2 distance_2 = decimal - float2(1.0f, 0.0f);
    float2 distance_3 = decimal - float2(0.0f, 1.0f);
    float2 distance_4 = decimal - float2(1.0f, 1.0f);
    
    /* �eX���̐��`��� & �eY���̐��`��� */
    return lerp(lerp(dot(gradient_1, distance_1), dot(gradient_2, distance_2), interpolation.x),
                lerp(dot(gradient_3, distance_3), dot(gradient_4, distance_4), interpolation.x), interpolation.y) * 0.5f + 0.5f;
}

/** �t�F�[�h�֐�
 * @param time �^�C�~���O(0.0f�`1.0f)
 * @return �t�F�[�h�l
 */
float Fade(in float time)
{
    /* �C�[�Y�Ȑ��ɂ���� */
    return (6.0f * pow(time, 5.0f)) - (15.0f * pow(time, 4.0f)) + (10.0f * pow(time, 3.0f));
}

/** �V���v���b�N�X�m�C�Y 
 */
float3 SimplexNoise(in float2 uv, in float block_num)
{
     /* �g���̃T�C�Y */
    float2 size = uv * block_num;
    
    size.x *= 1.1547f;
    size.y += size.x * 0.5f;
    
    /* �������̒��o */
    float2 integer = float2(floor(size.x), floor(size.y));
    /* �������̒��o */
    float2 decimal = float2(frac(size.x), frac(size.y));
    
    /* ��Ԋ�l(�C�[�Y�Ȑ�) */
    float2 interpolation = (6.0f * pow(decimal, 5.0f)) - (15.0f * pow(decimal, 4.0f)) + (10.0f * pow(decimal, 3.0f));
    
    if (decimal.x > decimal.y)
    {
        return float3(float2(1.0f, 1.0f) - float2(decimal.x, decimal.y - decimal.x), decimal.y);
    }
    
    return float3(float2(1.0f, 1.0f) - float2(decimal.x - decimal.y, decimal.y), decimal.x);
}
