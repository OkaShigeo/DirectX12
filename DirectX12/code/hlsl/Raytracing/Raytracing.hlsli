#include "Infomation.hlsli"

/* ���[�g�V�O�l�`�� */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

/* �J������� */
Camera camera : register(b0);
/* ���̂̏�� */
RWStructuredBuffer<Sphere>sphere : register(u0);
/* ���ʃe�N�X�`�� */
RWTexture2D<float4> tex : register(u1);

/* �w�i�F�̎Z�o */
float3 BackgroundColor(in Ray ray, in float3 finish_color, in float3 start_color = float3(1.0f, 1.0f, 1.0f))
{
    float time = 0.5f * (ray.direction.y + 1.0f);

    /* ���`��� */
    return lerp(start_color, finish_color, time);
}

/* ���̂Ƃ̓����蔻�� */
bool CheckHitSphere(in Ray ray, out Hit hit)
{
    uint sphere_num    = 0;
    uint sphere_stride = 0;
    sphere.GetDimensions(sphere_num, sphere_stride);
    
    hit.time = -1.0f;
    hit.pos = hit.normal = hit.color = 0.0f;
    
    float min = 0.001f;
    float max = float(0xffffffff);
    for (uint i = 0; i < sphere_num; ++i)
    {
        Hit dummy = hit;
              
        if (sphere[i].IsHit(ray, dummy, min, max) == true)
        {
            hit = dummy;
            max = hit.time;
        }
    }

    return (hit.time >= 0.0f);
}

/* �Ȗ͗l�̎擾 */
float3 GetStripedPatternColor(in float2 uv, in float3 pos, in float3 color1, in float3 color2)
{
    return 1.0f;
}