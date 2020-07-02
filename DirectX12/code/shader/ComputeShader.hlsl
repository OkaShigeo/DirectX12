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
    bool IsHit(in Ray ray, out float time)
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
            /* �@�����Z�o */
            normal = (ray.At(time) - pos) / radius;
        
            return true;
        }

        time = (-b + sqrt(discriminant)) / a;
        
        return false;
    }
};

struct ComputeThreadID
{
	uint3 group_thread_ID : SV_GroupThreadID;
	uint3 group_ID        : SV_GroupID;
	uint3 dispatch_ID     : SV_DispatchThreadID;
	uint group_index      : SV_GroupIndex;
};

/* �w�i�F�̎Z�o */
float3 BackColor(in Ray ray)
{
	/* 0�`1�͈̔͂� */
    float t = 0.5f * ray.direction.y + 1.0f;
	/* ���`�⊮ */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

/* ���̂Ƃ̓����蔻�� */
bool CheckHitSphere(in Ray ray, in Sphere sphere, out float time)
{
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(ray.direction, (ray.pos - sphere.pos));
	/* ���̂̕����� */
    float c = dot((ray.pos - sphere.pos), (ray.pos - sphere.pos)) - pow(sphere.radius, 2.0f);
	
    /* �񎟕����� */  
    float discriminant = b * b - (4.0f * a * c);
    /* �q�b�g���� */
    if (discriminant > 0.0f)
    {
        /* �q�b�g�������Ԃ��Z�o */
        time = (-b - sqrt(discriminant)) / (2.0f * a);
        
        return true;
    }
    
    return false;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
	/* �e�N�X�`���T�C�Y�̎擾 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* �A�X�y�N�g��̎Z�o */
    const float aspect = float(size.x) / float(size.y);
	/* UV�l�̎Z�o */
    const float2 uv = float2(semantics.dispatch_ID.xy) / float2(size);
	/* �r���[�|�[�g */
    const float2 viewport = float2(aspect * 2.0f, 2.0f);
	/* ������W */
    float3 left_pos = eye_pos - float3(viewport / 2.0f, distance);
	
    Ray ray = { eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos };
    Sphere sp = { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), 0.5f };
    
    float hit_distance = 0.0f;
    if (sp.IsHit(ray, hit_distance) == true)
    {
        float3 normal = 0.5f * ((ray.pos + (hit_distance * ray.direction)) - float3(0.0f, 0.0f, -1.0f) + 1.0f);
        tex[semantics.dispatch_ID.xy] = float4(sp.normal, 1.0f);
    }
    else
    {
        tex[semantics.dispatch_ID.xy] = float4(BackColor(ray), 1.0f);
    }
}
