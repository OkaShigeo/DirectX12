// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer RaytracingParam : register(b0)
{
	/* �J�����ʒu */
    float3 eye_pos;
	/* ���C�̍ő勗�� */
    float distance;
}

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
        return pos + (time * direction);
    }
};

/* ���̂̏�� */
struct Sphere
{
	/* ���S���W */
    float3 pos;
    /* �@�� */
    float3 normal;
	/* ���a */
    float radius;
    
    /* ���̂̕����� */
    float At(in float3 vec)
    {
        return dot((vec - pos), (vec - pos)) - pow(radius, 2.0f);
    }
    /* �q�b�g���� */
    bool IsHit(in Ray ray, out float time, in float min = 0.0f, in float max = float(0xffffffff))
    {
        /* �����x�N�g�����m�̓��ς̓x�N�g���̒����̓��ɓ����� */
        float a = pow(length(ray.direction), 2.0f);
        float b = dot((ray.pos - pos), ray.direction);
        /* �񎟕����� */
        float discriminant = b * b - a * At(ray.pos);
        
        /* �q�b�g���� */
        if (discriminant > 0.0f)
        {
            /* �q�b�g�����^�C�~���O���Z�o */
            time = (-b - sqrt(discriminant)) / a;
            if (!(min < time && time < max))
            {
                time = (-b + sqrt(discriminant)) / a;
                if (!(min < time && time < max))
                {
                    return false;
                }
            }
            
            /* �@�����Z�o */
            normal = (ray.At(time) - pos) / radius;
            /* ���C�͓���������ł��Ă��� */
            if (dot(ray.direction, normal) > 0.0f)
            {
                normal = float3(-normal.x, -normal.y, -normal.z);
            }
            
            return true;
        }

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

/* �p�x���烉�W�A���ϊ� */
float ChangeRadian(in float degree)
{
    return degree * acos(-1.0f) / 180.0f;
}

/* �����_���l�̎擾 */
float Randam(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* �͈͓��Ɏ��߂� */
float Clamp(in float value, in float min = 0.0f, in float max = 1.0f)
{
    if(value < min)
    {
        return min;
    }
    else if(value > max)
    {
        return max;
    }
    
    return value;
}

/* �w�i�F�̎Z�o */
float3 BackColor(in Ray ray)
{
	/* 0�`1�͈̔͂� */
    float t = 0.5f * ray.direction.y + 1.0f;
	/* ���`�⊮ */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

/* �O���[�v�X���b�h�� */
#define THREAD_X 256
#define THREAD_Y 1
#define THREAD_Z 1

/* �O���[�v�X���b�h���Ƃ̌v�Z����  */
groupshared float3 result[THREAD_X * THREAD_Y * THREAD_Z];

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
        { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), 0.5f },
        { float3(0.0f, -100.5f, 0.0f), float3(0.0f, 0.0f, 0.0f), 100.0f },
    };
    
    /* UV�l�̎Z�o */
    const float2 uv = (float2(semantics.group_ID.xy) + Randam(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
	/* ���C */
    Ray ray = { eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos };
        
    /* �q�b�g�^�C�� */
    float hit_time = float(0xffffffff);
    /* �q�b�g�J���[ */
    result[semantics.group_thread_ID.x] = BackColor(ray);
    
    /* �q�b�g�ŏ����� */
    const float min = 0.0f;
    /* �q�b�g�ő勗�� */
    float max = float(0xffffffff);
    /* �q�b�g����t���O */
    bool hit_flag = false;
    /* �q�b�g�ʒu�̖@�� */
    float3 hit_normal = 0.0f;
    for (uint sp_index = 0; sp_index < sp_num; ++sp_index)
    {
        if (sp[sp_index].IsHit(ray, hit_time, min, max) == true)
        {
            hit_flag = true;
            hit_normal = sp[sp_index].normal;
            max = hit_time;
            result[semantics.group_thread_ID.x] = 0.5f * (sp[sp_index].normal + 1.0f);
        }
    }
    
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
        tex[semantics.group_ID.xy] = float4(result[semantics.group_thread_ID.x] / THREAD_X, 1.0f);
    }
}
