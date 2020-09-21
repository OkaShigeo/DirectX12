#include "DirectXRaytracing.hlsli"

[shader("miss")]
void Miss(inout Payload payload)
{
    float3 ray_pos = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
    float time = 0.5f * (WorldRayDirection().y + 1.0f);
    
    /* ”wŒi‚Ì•`‰æ */
    //payload.color = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.6f, 0.8f, 1.0f, 1.0f), time);
    payload.color = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), time);
}