#include "DirectXRaytracing.hlsli"

[shader("closesthit")]
void ClosestHit(inout Payload payload, in Attributes attr)
{
    /* ヒットした距離を取得 */
    float distance = RayTCurrent();

    /* 色設定 */
    payload.color = float4(attr.center.x, attr.center.y, 1.0f - (attr.center.x + attr.center.y), 1.0f);
}