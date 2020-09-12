#include "Raytracing.hlsli"

/* �ő�X���b�h�O���[�v(X) */
#define MAX_THREAD_GROUP_X 64
/* �ő唽�ˉ� */
#define MAX_REFLECT_COUNT 10

/* �O���[�v�X���b�h���Ƃ̌v�Z����  */
groupshared float3 result[MAX_THREAD_GROUP_X];

[RootSignature(RS)]
[numthreads(MAX_THREAD_GROUP_X, 1, 1)]
void main(ComputeThreadID semantics)
{
    /* �e�N�X�`���T�C�Y�̎擾 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
    
    /* UV���W */
    float2 uv = semantics.group_ID.xy / float2(size.x, size.y);
    /* �J���[�l */
    float3 color = SimplexNoise(uv, 10);
    tex[semantics.group_ID.xy] = float4(color, 1.0f);
    return;
    
    
    
 //   /* �q�b�g��� */
 //   Hit hit =
 //   {
 //       -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE, float2(0.0f, 0.0f)
 //   };
    
	///* �e�N�X�`���T�C�Y�̎擾 */ 
 //   uint2 size = 0;
 //   tex.GetDimensions(size.x, size.y);
    
 //   /* �A�X�y�N�g�䗦 */
 //   const float aspect = float(size.x) / float(size.y);
 //   /* �r���[�|�[�g */
 //   const float2 viewport = float2(aspect * 2.0f * tan(Radian(camera.fov) / 2.0f), 2.0f * tan(Radian(camera.fov) / 2.0f));
 //   /* �W������ */
 //   const float focus_distance = length(camera.pos - camera.target);
    
 //   /* �ʒu�C�� */
 //   float3 dir = normalize(camera.pos - camera.target);
 //   float3 horizontal = viewport.x * normalize(cross(camera.up, dir)) * focus_distance;
 //   float3 vertical = viewport.y * cross(dir, normalize(cross(camera.up, dir))) * focus_distance;
    
 //   float3 left = camera.pos - ((horizontal + vertical) / 2.0f) - (dir * focus_distance);
 //   /* UV�l�̎Z�o */
 //   const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
    
 //   /* ���C */
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
    
 //   /* �J���[�l������ */
 //   result[semantics.group_thread_ID.x] = color;
    
 //   /* �O���[�v�X���b�h�̓��� */
 //   GroupMemoryBarrierWithGroupSync();
    
 //   /* �A���`�G�C���A�V���O(�����T���v������̕���)���� */
 //   for (int th_index = MAX_THREAD_GROUP_X / 2; th_index > 0; th_index /= 2)
 //   {
 //       if (semantics.group_thread_ID.x < th_index)
 //       {
 //           result[semantics.group_thread_ID.x] += result[semantics.group_thread_ID.x + th_index];
 //       }
            
 //       /* �O���[�v�X���b�h�̓��� */
 //       GroupMemoryBarrierWithGroupSync();
 //   }
    
 //   if (semantics.group_thread_ID.x == 0)
 //   {
 //       result[semantics.group_thread_ID.x] /= float(MAX_THREAD_GROUP_X);
       
 //       tex[semantics.group_ID.xy] = float4(Linear(result[semantics.group_thread_ID.x]), 1.0f);
 //   }
}