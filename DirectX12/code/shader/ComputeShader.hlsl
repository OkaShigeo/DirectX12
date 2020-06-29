// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer Info : register(b0)
{
	/* カメラ位置 */
    float3 eye_pos;
}

struct ComputeThreadID
{
	uint3 group_thread_ID : SV_GroupThreadID;
	uint3 group_ID        : SV_GroupID;
	uint3 dispatch_ID     : SV_DispatchThreadID;
	uint group_index      : SV_GroupIndex;
};

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	
	tex[semantics.dispatch_ID.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);
}
