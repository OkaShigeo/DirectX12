#include "DirectXRaytracing.hlsli"

/* レイトレーシング情報 */
cbuffer RaytracingInfo : register(b0, space0)
{
    /* 視点 */
    float3 eye;
    /* レイの進む方向 */
    float3 direction;
    /* ヒット判定開始距離 */
    float tmin;
    /* ヒット判定終了距離 */
    float tmax;
}

/* レイ生成シェーダ */
[shader("raygeneration")]
void RayGen()
{
    /* レイの全グリッド */
    uint3 size = DispatchRaysDimensions();
    /* レイ発射グリッド番号の取得 */
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
    

    /* レイの情報を定義 */
    RayDesc ray;
    /* 視点 */
    ray.Origin = float3(0.0f, 0.0f, -2.0f);
    /* レイの進む方法 */
    ray.Direction = normalize(float3(d.x * aspectRatio, -d.y, 1));
    /* レイのヒット判定開始距離 */
    ray.TMin      = 0.0f;
    /* レイのヒット判定終了距離 */
    ray.TMax      = 10000;

    Payload payload;
    
    /* レイの制御フラグ */
    uint flag           = RAY_FLAG_NONE;
    /* ヒット判定マスク */
    uint hit_mask       = 0xFF;
    /* ヒットグループ番号のオフセット */
    uint group_offset   = 0;
    /* ヒットグループの何か */
    uint group_multiple = 1;
    /* ミスシェーダの呼び出し番号 */
    uint miss_index     = 0;

    /* レイの生成 */
    TraceRay(Scene, flag, hit_mask, group_offset, group_multiple, miss_index, ray, payload);

    /* 結果を格納 */
    Texture[index.xy] = payload.color;
}