#include "DirectXRaytracing.hlsli"

/* レイ生成シェーダ */
[shader("raygeneration")]
void RayGen()
{
    /* レイ発射グリッド番号の取得 */
    uint2 index = DispatchRaysIndex();

    /* レイの情報を定義 */
    RayDesc ray;
    /* 視点 */
    ray.Origin    = float3(0.0f, 0.0f, 0.0f);
    /* レイの進む方法 */
    ray.Direction = float3(0.0f, 0.0f, 1.0f);
    /* レイのヒット判定開始距離 */
    ray.TMin      = 0.0f;
    /* レイのヒット判定終了距離 */
    ray.TMax      = 10000.0f;

    Payload payload;
    
    /* レイの制御フラグ */
    uint flag           = RAY_FLAG_NONE;
    /* ヒット判定マスク */
    uint hit_mask       = 0x00FF;
    /* ヒットグループ番号のオフセット */
    uint group_offset   = 0x00;
    /* ヒットグループの何か */
    uint group_multiple = 0x01;
    /* ミスシェーダの呼び出し番号 */
    uint miss_index     = 0x00000000;

    /* レイの生成 */
    TraceRay(Scene, flag, hit_mask, group_offset, group_multiple, miss_index, ray, payload);

    /* 結果を格納 */
    Texture[index.xy] = payload.color;
}