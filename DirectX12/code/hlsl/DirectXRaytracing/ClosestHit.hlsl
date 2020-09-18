#include "DirectXRaytracing.hlsli"

[shader("closesthit")]
void ClosestHit(inout Payload payload, in Attributes attr)
{
    /* �q�b�g�����������擾 */
    float distance = RayTCurrent();

    /* �F�ݒ� */
    payload.color = float4(attr.center.x, attr.center.y, 1.0f - (attr.center.x + attr.center.y), 1.0f);
}