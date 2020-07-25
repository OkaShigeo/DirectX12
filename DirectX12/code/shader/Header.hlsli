// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4> tex : register(u0);

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
#define SPHERE_MAX 5
/* �}�e���A������ */
#define MATERIAL_NONE 0
/* �����o�[�g���˃}�e���A�� */
#define MATERIAL_LAMBERT 1
/* ���ʔ��˃}�e���A�� */
#define MATERIAL_METAL 2
/* �U�d�̃}�e���A�� */
#define MATERIAL_DIELECTRIC 3

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
    /* �@�������������m�F�t���O */
    bool inside_normal;
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
                    hit.inside_normal = false;
                    
                    return false;
                }
            }
            
            /* �@�����Z�o */
            float3 normal = (ray.At(time) - pos) / radius;
            /* ���C�͓���������ł��Ă��� */
            if (dot(ray.direction, hit.normal) > 0.0f)
            {
                normal = -normal;
                hit.inside_normal = true;
            }
            
            hit.time = time;
            hit.pos = ray.At(time);
            hit.normal = (normal);
            hit.color = color;
            hit.material = material;
            
            return true;
        }

        /* �q�b�g���̏����� */
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        hit.inside_normal = false;
        
        return false;
    }
};

/* �G���g���[�|�C���g�̈��� */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID : SV_GroupID;
    uint3 dispatch_ID : SV_DispatchThreadID;
    uint group_index : SV_GroupIndex;
};