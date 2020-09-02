#include "Infomation.hlsli"

/* ルートシグネチャ */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

/* カメラ情報 */
Camera camera : register(b0);
/* 球体の情報 */
RWStructuredBuffer<Sphere>sphere : register(u0);
/* 結果テクスチャ */
RWTexture2D<float4> tex : register(u1);

/* 背景色の算出 */
float3 BackgroundColor(in Ray ray, in float3 finish_color, in float3 start_color = float3(1.0f, 1.0f, 1.0f))
{
    float time = 0.5f * (ray.direction.y + 1.0f);

    /* 線形補間 */
    return lerp(start_color, finish_color, time);
}

/* 縞模様の取得 */
float3 GetStripedPatternColor(in float2 uv, in float3 pos, in float3 color1 = float3(0.0f, 0.0f, 0.0f), in float3 color2 = float3(1.0f, 1.0f, 1.0f))
{
    float sine = sin(10.0f * pos.x) * sin(10.0f * pos.y) * sin(10.0f * pos.z);
    if (sine < 0.0f)
    {
        return color1;
    }
    
    return color2;
}

/* 球体との当たり判定 */
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
            
            if (hit.material == MATERIAL_LAMBERT)
            {
                hit.color = GetStripedPatternColor(hit.uv, hit.pos);
            }
        }
    }

    return (hit.time >= 0.0f);
}
