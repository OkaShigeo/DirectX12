#include "DirectXRaytracing.hlsli"

[shader("miss")]
void Miss(inout Payload payload)
{
    /* ”wŒi‚Ì•`‰æ */
    payload.color = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.6f, 0.8f, 1.0f, 1.0f), RayTCurrent());
}