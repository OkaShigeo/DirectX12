/* ���[�g�V�O�l�`�� */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"
/* �e�N�X�`�� */
RWTexture2D<float4> tex : register(u0);
/* �K�v�p�����[�^ */
cbuffer RaytracingParam : register(b0)
{
	/* ���_�ʒu */
    float3 eye_pos;
	/* ���C�̍ő勗�� */
    float distance;
    /* ���C�g�ʒu */
    float3 light;
}

/* �O���[�v�X���b�h�� */
#define THREAD_X 64
#define THREAD_Y 1
#define THREAD_Z 1

/* ���̂̍ő吔 */
#define SPHERE_MAX 5

/* �}�e���A����� */
#define MATERIAL_NONE    0
#define MATERIAL_LAMBERT 1
#define MATERIAL_REFLECT 2
#define MATERIAL_REFRACT 3

/* �G���g���[�|�C���g�̈��� */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID        : SV_GroupID;
    uint3 dispatch_ID     : SV_DispatchThreadID;
    uint group_index      : SV_GroupIndex;
};

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

/* ���C */
struct Ray
{
    /* �n�_���W */
    float3 pos;
    /* �ړ����� */
    float3 direction;
    
    /* ���C�̈ʒu���擾 */
    float3 GetPos(in float time)
    {
        /* �����̕����� */
        return pos + (time * direction);
    }
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
    
    /* ���a�̎擾 */
    float GetRadius(in float3 pos)
    {
        float3 tmp = pos - center;

        /* ���̕����� */
        return sqrt(dot(tmp, tmp));
    }
    
    /* �q�b�g���� */
    bool IsHit(in Ray ray, out Hit hit, in float min = 0.001f, in float max = float(0xffffffff))
    {
        /* ax^2 + bx + c = 0 */
        float3 oc = ray.pos - center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0f * dot(ray.direction, oc);
        float c = dot(oc, oc) - (radius * radius);
        
        bool flag = true;
        
        /* ���莮 */
        float D = (b * b) - (4.0f * a * c);
        if(D > 0.0f)
        {
            /* �q�b�g�^�C�~���O���Z�o */
            float time = (-b - sqrt(D)) / (2.0f * a);
            /* �q�b�g�^�C�~���O���͈͓����m�F */
            if (!(min < time && time < max))
            {
                time = (-b + sqrt(D)) / (2.0f * a);
                if (!(min < time && time < max))
                {
                    flag = false;
                }
            }
            
            if(flag == true)
            {
                hit.time     = time;
                hit.pos      = ray.GetPos(hit.time);
                hit.normal   = (hit.pos - center) / radius;
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