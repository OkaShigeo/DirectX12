#include "Raytracing.hlsli"

/* 最大スレッドグループ(X) */
#define MAX_THREAD_GROUP_X 64
/* 最大反射回数 */
#define MAX_REFLECT_COUNT 10

/* グループスレッドごとの計算結果  */
groupshared float3 result[MAX_THREAD_GROUP_X];

[RootSignature(RS)]
[numthreads(MAX_THREAD_GROUP_X, 1, 1)]
void main(ComputeThreadID semantics)
{
    /* テクスチャサイズの取得 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
    
    /* UV座標 */
    float2 uv = semantics.group_ID.xy / float2(size.x, size.y);
    /* カラー値 */
    float3 color = SimplexNoise(uv, 10);
    tex[semantics.group_ID.xy] = float4(color, 1.0f);
    return;
    
    
    
 //   /* ヒット情報 */
 //   Hit hit =
 //   {
 //       -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE, float2(0.0f, 0.0f)
 //   };
    
	///* テクスチャサイズの取得 */ 
 //   uint2 size = 0;
 //   tex.GetDimensions(size.x, size.y);
    
 //   /* アスペクト比率 */
 //   const float aspect = float(size.x) / float(size.y);
 //   /* ビューポート */
 //   const float2 viewport = float2(aspect * 2.0f * tan(Radian(camera.fov) / 2.0f), 2.0f * tan(Radian(camera.fov) / 2.0f));
 //   /* 集束距離 */
 //   const float focus_distance = length(camera.pos - camera.target);
    
 //   /* 位置修正 */
 //   float3 dir = normalize(camera.pos - camera.target);
 //   float3 horizontal = viewport.x * normalize(cross(camera.up, dir)) * focus_distance;
 //   float3 vertical = viewport.y * cross(dir, normalize(cross(camera.up, dir))) * focus_distance;
    
 //   float3 left = camera.pos - ((horizontal + vertical) / 2.0f) - (dir * focus_distance);
 //   /* UV値の算出 */
 //   const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
    
 //   /* レイ */
 //   Ray ray =
 //   {
 //       camera.pos + GetDepthOfFieldOffset(camera, uv), left + (horizontal * uv.x) + (vertical * uv.y) - ray.pos, Random(uv, semantics.group_index)
 //   };
    
 //   float3 color = 1.0f;
 //   for (uint i = 0; i < MAX_REFLECT_COUNT; ++i)
 //   {
 //       if (CheckHitSphere(ray, hit) == true)
 //       {
 //           float3 value = 0.5f * (hit.normal + 1.0f);
 //           switch (hit.material)
 //           {
 //               case MATERIAL_LAMBERT:
 //                   value = LambertReflect(ray, hit, 0.5f);
 //                   break;
 //               case MATERIAL_REFLECT:
 //                   value = SpecularReflection(ray, hit, 0.5f);
 //                   break;
 //               case MATERIAL_REFRACT:
 //                   value = Refraction(ray, hit, 1.5f);
 //                   break;
 //               default:
 //                   break;
 //           }
            
 //           color *= value;
 //       }
 //       else
 //       {
 //           color *= BackgroundColor(ray, float3(0.6f, 0.8f, 1.0f));
 //           break;
 //       }
 //   }
    
 //   /* カラー値を決定 */
 //   result[semantics.group_thread_ID.x] = color;
    
 //   /* グループスレッドの同期 */
 //   GroupMemoryBarrierWithGroupSync();
    
 //   /* アンチエイリアシング(複数サンプルからの平均)処理 */
 //   for (int th_index = MAX_THREAD_GROUP_X / 2; th_index > 0; th_index /= 2)
 //   {
 //       if (semantics.group_thread_ID.x < th_index)
 //       {
 //           result[semantics.group_thread_ID.x] += result[semantics.group_thread_ID.x + th_index];
 //       }
            
 //       /* グループスレッドの同期 */
 //       GroupMemoryBarrierWithGroupSync();
 //   }
    
 //   if (semantics.group_thread_ID.x == 0)
 //   {
 //       result[semantics.group_thread_ID.x] /= float(MAX_THREAD_GROUP_X);
       
 //       tex[semantics.group_ID.xy] = float4(Linear(result[semantics.group_thread_ID.x]), 1.0f);
 //   }
}