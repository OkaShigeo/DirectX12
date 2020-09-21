#include "DirectXRaytracing.hlsli"

/* ���C�g���[�V���O��� */
cbuffer RaytracingInfo : register(b0, space0)
{
    /* ���_ */
    float3 eye;
    /* ���C�̐i�ޕ��� */
    float3 direction;
    /* �q�b�g����J�n���� */
    float tmin;
    /* �q�b�g����I������ */
    float tmax;
}

/* ���C�����V�F�[�_ */
[shader("raygeneration")]
void RayGen()
{
    /* ���C�̑S�O���b�h */
    uint3 size = DispatchRaysDimensions();
    /* ���C���˃O���b�h�ԍ��̎擾 */
    uint2 index = DispatchRaysIndex();
    
    float aspect = float(size.x) / float(size.y);
    float2 uv = float2(index) / float2(size.xy);
   
    float3 target = float3(0.0f, 0.0f, 0.0f);
    float3 up     = float3(0.0f, 1.0f, 0.0f);
    
    float3 z = normalize(eye - target);
    float3 x = normalize(cross(up, z));
    float3 y = cross(x, z);
    
    float theta = 90.0f * acos(-1.0f) / 180.0f;
    float height_half = tan(theta / 2.0f);
    float width_half = aspect * height_half;
    
    float3 u = 2.0f * width_half * x;
    float3 v = 2.0f * height_half * y;
    
    float3 a = eye - ((u + v) / 2.0f) - z;
    
    float3 b = a + (u * uv.x) + (v + uv.y) - eye;
    
    
    float2 crd = float2(index.xy);
    float2 dims = float2(size.xy);

    float2 d = ((crd / dims) * 2.f - 1.f);
    float aspectRatio = dims.x / dims.y;
    

    /* ���C�̏����` */
    RayDesc ray;
    /* ���_ */
    ray.Origin = float3(0.0f, 0.0f, -2.0f);
    /* ���C�̐i�ޕ��@ */
    ray.Direction = normalize(float3(d.x * aspectRatio, -d.y, 1));
    /* ���C�̃q�b�g����J�n���� */
    ray.TMin      = 0.0f;
    /* ���C�̃q�b�g����I������ */
    ray.TMax      = 10000;

    Payload payload;
    
    /* ���C�̐���t���O */
    uint flag           = RAY_FLAG_NONE;
    /* �q�b�g����}�X�N */
    uint hit_mask       = 0xFF;
    /* �q�b�g�O���[�v�ԍ��̃I�t�Z�b�g */
    uint group_offset   = 0;
    /* �q�b�g�O���[�v�̉��� */
    uint group_multiple = 1;
    /* �~�X�V�F�[�_�̌Ăяo���ԍ� */
    uint miss_index     = 0;

    /* ���C�̐��� */
    TraceRay(Scene, flag, hit_mask, group_offset, group_multiple, miss_index, ray, payload);

    /* ���ʂ��i�[ */
    Texture[index.xy] = payload.color;
}