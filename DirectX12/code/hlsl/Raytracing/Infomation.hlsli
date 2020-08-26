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
    
    /* ���a�̎擾 */
    float GetRadius(in float3 pos)
    {
        float3 tmp = pos - center;

        /* ���̕����� */
        return sqrt(dot(tmp, tmp));
    }
    /* ���S�ʒu�̎擾 */
    float3 GetCenter(in Ray ray, in float move_start = 0.0f, in float move_end = 1.0f)
    {
        /* �ړ��Ԋu */
        float period = move_end - move_start;
        
        return center + (((period * ray.motion_blur_timing) - move_start) / period) * velocity;
    }
    /* �q�b�g���� */
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
                
                return true;
            }
        }
        
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        
        return false;
    }
};

/* �w�i�F�̎Z�o */
float3 BackgroundColor(in Ray ray, in float3 finish_color, in float3 start_color = float3(1.0f, 1.0f, 1.0f))
{
    float time = 0.5f * (ray.direction.y + 1.0f);

    /* ���`��� */
    return lerp(start_color, finish_color, time);
}

/* �����_���l�̎擾 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* �����o�[�g���� */
float3 LambertReflect(in out Ray ray, in Hit hit, in float albedo, in uint loop = 10)
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
    
    return albedo * hit.color;
}

/* ���� */
float3 Reflect(in float3 direction, in float3 normal)
{
    return reflect(normalize(direction), normal);
    
    /* ���˃x�N�g���Ɩ@���x�N�g�����P�ʃx�N�g���̏ꍇ�̔��� */
    return direction + 2.0f * normal;
    /* �@���x�N�g�������P�ʃx�N�g���̏ꍇ�̔��� */
    //return direction - 2.0f * dot(direction, normal) * normal;
}

/* ���ʔ��� */
float3 SpecularReflection(in out Ray ray, in Hit hit, in float albedo, in float fazz = 0.0f, in uint loop = 10)
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
    
    return albedo * hit.color;
}

/* �t���l���̖@�� */
float Fresnel(in float refract, in float cos)
{
    /* Schlick�̋ߎ��� */
    float tmp = (1.0f - refract) / (1.0f + refract);
    tmp *= tmp;

    return tmp + (1.0f - tmp) * pow((1.0f - cos), 5.0f);
}

/* ���� */
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

/* ��ʊE�[�x�̃I�t�Z�b�g���擾 */
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

/* ���j�A�F��Ԃɕϊ� */
float3 Linear(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/* sRGB�F��Ԃɕϊ� */
float3 sRGB(in float3 color)
{
    return pow(color, 2.2f);
}

/* ���W�A���p�x�ɕϊ� */
float Radian(in float angle)
{
    return angle * acos(-1.0f) / 180.0f;
}
