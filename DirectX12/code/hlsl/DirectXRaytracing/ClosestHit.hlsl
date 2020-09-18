#include "DirectXRaytracing.hlsli"

[shader("closesthit")]
void ClosestHit(inout Payload payload, in Attributes attr)
{
    /* ƒqƒbƒg‚µ‚½‹——£‚ğæ“¾ */
    float distance = RayTCurrent();

    /* Fİ’è */
    payload.color = float4(attr.center.x, attr.center.y, 1.0f - (attr.center.x + attr.center.y), 1.0f);
}