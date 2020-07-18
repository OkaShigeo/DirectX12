// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer RaytracingParam : register(b0)
{
	/* ���_ */
    float3 eye_pos;
	/* ���C�̍ő勗�� */
    float distance;
    /* ���C�g�ʒu */
    float3 light;
}

/* ���̂̍ő吔 */
#define SPHERE_MAX 2
/* �}�e���A������ */
#define MATERIAL_NONE 0
/* �����o�[�g���˃}�e���A�� */
#define MATERIAL_LAMBERT 1

/* ���C�̏�� */
struct Ray
{
	/* �ʒu */
    float3 pos;
	/* ���� */
    float3 direction;
    
    /* �����̕����� */
    float3 At(in float time)
    {
        /* ���Ԍo�߂ɂ��ʒu�����߂� */
        return pos + (time * direction);
    }
};

/* �q�b�g��� */
struct Hit
{
    /* �q�b�g�^�C�~���O */
    float time;
    /* �q�b�g�����ʒu */
    float3 pos;
    /* �q�b�g�����@�� */
    float3 normal;
    /* �q�b�g�J���[ */
    float3 color;
    /* �}�e���A�� */
    uint material;
};

/* ���̂̏�� */
struct Sphere
{
	/* ���S���W */
    float3 pos;
	/* ���a */
    float radius;
    /* �J���[ */
    float3 color;
    /* �}�e���A�� */
    uint material;
    
    /* ���̂̕����� */
    float At(in float3 vec)
    {
        return dot((vec - pos), (vec - pos)) - pow(radius, 2.0f);
    }
    /* �q�b�g���� */
    bool IsHit(in Ray ray, out Hit hit, in float min = 0.001f, in float max = float(0xffffffff))
    {
        float3 oc = ray.pos - pos;
        
        /* �����x�N�g�����m�̓��ς̓x�N�g���̒����̓��ɓ����� */
        float a = pow(length(ray.direction), 2.0f);
        float b = dot(oc, ray.direction);
        float c = pow(length(oc), 2.0f) - pow(radius, 2.0f);
        /* �񎟕����� */
        float discriminant = b * b - a * c;
        
        /* �q�b�g���� 
        * D > 0 �q�b�g����(�Q�ӏ�)
        * D = 0 �q�b�g����(�P�ӏ�)
        * D < 0 �q�b�g�Ȃ�
        */
        if (discriminant > 0.0f)
        {   
            /* �q�b�g�����^�C�~���O���Z�o */
            float time = (-b - sqrt(discriminant)) / a;
            if (!(min < time && time < max))
            {
                time = (-b + sqrt(discriminant)) / a;
                if (!(min < time && time < max))
                {
                    /* �q�b�g���̏����� */
                    hit.time = -1.0f;
                    hit.pos = hit.normal = hit.color = 0.0f;
                    hit.material = MATERIAL_NONE;
                    
                    return false;
                }
            }
            
            /* �@�����Z�o */
            float3 normal = (ray.At(time) - pos) / radius;
            /* ���C�͓���������ł��Ă��� */
            if (!(dot(ray.direction, normal) < 0.0f))
            {
                normal = float3(-normal.x, -normal.y, -normal.z);
            }
            
            hit.time     = time;
            hit.pos      = ray.At(time);
            hit.normal   = normal;
            hit.color    = color;
            hit.material = material;
            
            return true;
        }

        /* �q�b�g���̏����� */
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        
        return false;
    }
};

/* �G���g���[�|�C���g�̈��� */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID        : SV_GroupID;
    uint3 dispatch_ID     : SV_DispatchThreadID;
    uint group_index      : SV_GroupIndex;
};

/* �����_���l�̎擾 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* �w�i�F�̎Z�o */
float3 BackColor(in Ray ray)
{
	/* 0�`1�͈̔͂� */
    float t = 0.5f * (ray.direction.y + 1.0f);
	/* ���`�⊮ */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

/* ���j�A�F��Ԃɕϊ� */
float3 ChangeLinearColor(in float3 color)
{
    return pow(color, 2.2f);
}

/* sRGB�F��Ԃɕϊ� */
float3 ChangeSRGBColor(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/* 
* ���̂Ƃ̃q�b�g���� 
* �V���h�E�A�N�l�K�p
*/
bool IsSphereHit(in Ray ray, in Sphere sp[SPHERE_MAX], out Hit hit)
{
    const float min = 0.01f;
    float max = float(0xffffffff);
    
    /* �q�b�g���̏����� */
    hit.time = -1.0f;
    hit.pos = hit.normal = hit.color = 0.0f;
    hit.material = MATERIAL_NONE;
    
    for (uint i = 0; i < SPHERE_MAX; ++i)
    {
        Hit dummy_hit = hit;
        if (sp[i].IsHit(ray, dummy_hit, min, max) == true)
        {
            hit = dummy_hit;
            max = hit.time;
        }
    }
    
    return (hit.time >= 0.0f);
}

/* �����o�[�g���� */
float3 LambertReflection(out Ray ray, in Hit hit, in float albedo = 1.0f, uint random_num = 10)
{
    float max = float(0xffffffff);
    float3 random_vec = 0.0f;
    
    random_vec.x = Random(hit.pos.xy);
    random_vec.y = Random(hit.pos.yz);
    random_vec.z = Random(hit.pos.zx);
            
    /* �P�ʋ����Ɉ�ԋ߂����ˌ��������߂� */
    for (uint i = 0; i < random_num; ++i)
    {        
        /* ��l���z�ɋ߂Â��� */
        float a = Random(random_vec.xy, i) * 2.0f * acos(-1.0f);
        float z = 2.0f * Random(random_vec.yz, i) - 1.0f;
        float r = sqrt(1.0f - z * z);
            
        random_vec.x = r * cos(a);
        random_vec.y = r * sin(a);
        random_vec.z = z;
        
        float tmp = pow(length(random_vec), 2.0f);
        if(tmp < 1.0f)
        {
            break;
        }
        else
        {
            max = min(max, tmp);
        }
    }
            
    /* ���C�̔��� */
    ray.pos = hit.pos;
    ray.direction = hit.normal + random_vec;

    return hit.color * albedo;
}

/* �O���[�v�X���b�h�� */
#define THREAD_X 512
#define THREAD_Y 1
#define THREAD_Z 1

/* �O���[�v�X���b�h���Ƃ̌v�Z����  */
groupshared float3 result[THREAD_X];

[RootSignature(RS)]
[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(ComputeThreadID semantics)
{
	/* �e�N�X�`���T�C�Y�̎擾 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* �A�X�y�N�g��̎Z�o */
    const float aspect = float(size.x) / float(size.y);
	/* �r���[�|�[�g */
    const float2 viewport = float2(aspect * 2.0f, 2.0f);
     /* ������W */
    float3 left_pos = eye_pos - float3(viewport / 2.0f, distance);
    /* ���̂̐� */
    const uint sp_num = 2;
    /* ���� */
    Sphere sp[sp_num] =
    {
        { float3(0.0f, 0.0f, 0.0f), 0.5f, float3(1.0f, 1.0f, 1.0f), MATERIAL_LAMBERT },
        { float3(0.0f, -100.5f, 0.0f), 100.0f, float3(1.0f, 1.0f, 1.0f), MATERIAL_LAMBERT },
    };
    
    /* UV�l�̎Z�o */
    const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
	/* ���C */
    Ray ray = 
    { 
        eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos 
    };
    /* �q�b�g�J���[ */
    float3 color = 1.0f;
    /* ���ˋ��x */
    const float albedo = 0.5f;
    
    for (uint ref_cnt = 0; ref_cnt < 10; ++ref_cnt)
    {
        /* �q�b�g��� */
        Hit hit =
        {
            -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE
        };
        
        /* �q�b�g����`�F�b�N */
        if (IsSphereHit(ray, sp, hit) == true)
        {
            if (hit.material == MATERIAL_LAMBERT)
            {
                color *= LambertReflection(ray, hit, albedo) * max(0.0f, dot((light - hit.pos), hit.normal));
            }
        }
        else
        {
            color *= BackColor(ray);
            break;
        }
    }
    
    result[semantics.group_thread_ID.x] = color;
    
    /* �O���[�v�X���b�h�̓��� */
    GroupMemoryBarrierWithGroupSync();
    
    for (int th_index = THREAD_X / 2; th_index > 0; th_index /= 2)
    {
        if (semantics.group_thread_ID.x < th_index)
        {
            result[semantics.group_thread_ID.x] += result[semantics.group_thread_ID.x + th_index];
        }
            
        /* �O���[�v�X���b�h�̓��� */
        GroupMemoryBarrierWithGroupSync();
    }
    
    if(semantics.group_thread_ID.x == 0)
    {
        result[semantics.group_thread_ID.x] /= THREAD_X;
       
        tex[semantics.group_ID.xy] = float4(ChangeSRGBColor(result[semantics.group_thread_ID.x]), 1.0f);
    }
}
