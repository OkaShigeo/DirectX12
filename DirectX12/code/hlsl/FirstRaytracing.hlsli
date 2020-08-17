/* ���[�g�V�O�l�`�� */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

/* �O���[�v�X���b�h�� */
#define THREAD_X 64
#define THREAD_Y 1
#define THREAD_Z 1

/* ���̂̍ő吔 */
#define SPHERE_MAX 128

/* �}�e���A����� */
#define MATERIAL_NONE    0
#define MATERIAL_LAMBERT 1
#define MATERIAL_REFLECT 2
#define MATERIAL_REFRACT 3

/* �G���g���[�|�C���g�̈��� */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID : SV_GroupID;
    uint3 dispatch_ID : SV_DispatchThreadID;
    uint group_index : SV_GroupIndex;
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

/* �J���� */
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
};

/* ���C */
struct Ray
{
    /* �n�_���W */
    float3 pos;
    /* �ړ����� */
    float3 direction;
    /* ���[�V�����u���[�^�C�~���O */
    float time;
    
    /* ���C�̈ʒu���擾 */
    float3 GetPos(in float time)
    {
        /* �����̕����� */
        return pos + (time * direction);
    }
};

/* Axis Aligned Bounding Box */
struct AABB
{
    /* �ŏ��ʒu */
    float3 min_pos;
    /* �ő�ʒu */
    float3 max_pos;
    
    /* �q�b�g���� */
    bool IsHit(in Ray ray, in out float min_time = 0.001f, in out float max_time = float(0xffffffff))
    {
        for (uint i = 0; i < 3; ++i)
        {
            float tmin = (min_pos[i] - ray.pos[i]) / ray.direction[i];
            float tmax = (max_pos[i] - ray.pos[i]) / ray.direction[i];

            /* �t�����̏ꍇ�͓���ւ� */
            if (ray.direction[i] < 0.0f)
            {
                float tmp = tmin;
                tmin = tmax;
                tmax = tmp;
            }
            
            tmin = max(tmin, min_time);
            tmax = min(tmax, max_time);
            
            if (!(tmin < tmax))
            {
                return false;
            }
        }

        return true;
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
    float3 GetCenter(in Camera cam, in Ray ray, in float motion_start = 0.0f, in float motion_end = 1.0f)
    {
        float period = motion_end - motion_start;
        return center + (((period * ray.time) - motion_start) / (motion_end - motion_start)) * velocity;
    }
    
    /* �q�b�g���� */
    bool IsHit(in Camera cam, in Ray ray, out Hit hit, in float min_distance = 0.001f, in float max_distance = float(0xffffffff))
    {
        float3 now_center = GetCenter(cam, ray);
        
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

/* �K�v�p�����[�^ */
cbuffer RaytracingParam : register(b0)
{
    Camera cam;
    uint sphere_max;
    Sphere sp[SPHERE_MAX];
}
/* �e�N�X�`�� */
RWTexture2D<float4> tex : register(u0);