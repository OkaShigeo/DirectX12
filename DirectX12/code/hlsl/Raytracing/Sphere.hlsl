#include "Sphere.hlsli"

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
    float3 center   = 0.0f;
    float radius    = 0.0f;
    float3 color    = 0.0f;
    uint material   = MATERIAL_NONE;
    float3 velocity = 0.0f;
    
    switch (semantics.group_ID.x)
    {
        case 0:
            center   = float3(0.0f, -1000.0f, 0.0f);
            radius   = 1000.0f;
            color    = float3(0.5f, 0.5f, 0.5f);
            material = MATERIAL_LAMBERT;
            break;
        case 1:
            center   = float3(0.0f, 1.0f, 0.0f);
            radius   = 1.0f;
            color    = float3(0.4f, 0.2f, 0.1f);
            material = MATERIAL_LAMBERT;
            break;
        case 2:
            center   = float3(4.0f, 1.0f, 0.0f);
            radius   = 1.0f;
            color    = float3(1.0f, 1.0f, 1.0f);
            material = MATERIAL_REFRACT;
            break;
        case 3:
            center   = float3(-4.0f, 1.0f, 0.0f);
            radius   = 1.0f;
            color    = float3(0.7f, 0.6f, 0.5f);
            material = MATERIAL_REFLECT;
            break;
        default:
            radius = 0.2f;
        
            color.r = Random(color.gb, semantics.group_ID.x);
            color.g = Random(color.br, semantics.group_ID.x);
            color.b = Random(color.rg, semantics.group_ID.x);
        
            float value = 5.0f;
            center.x = ((2.0f * Random(color.rg, semantics.group_ID.x)) - 1.0f) * value;
            center.y = radius;
            center.z = ((2.0f * Random(color.gb, semantics.group_ID.x)) - 1.0f) * value;
        
            value = 0.1f;
            velocity.x = Random(center.xy, semantics.group_ID.x) * value;
            velocity.y = Random(center.yz, semantics.group_ID.x) * value;
            velocity.z = Random(center.zx, semantics.group_ID.x) * value;
            
            float random = Random(velocity.xz, semantics.group_ID.x);
            if (random < 0.4f)
            {
                material = MATERIAL_LAMBERT;
            }
            else if (random < 0.8f)
            {
                material = MATERIAL_REFLECT;
            }
            else
            {
                material = MATERIAL_REFRACT;
            }
            break;
    }
    
    sphere[semantics.group_ID.x].center   = center;
    sphere[semantics.group_ID.x].radius   = radius;
    sphere[semantics.group_ID.x].color    = color;
    sphere[semantics.group_ID.x].material = material;
    sphere[semantics.group_ID.x].velocity = velocity;
}