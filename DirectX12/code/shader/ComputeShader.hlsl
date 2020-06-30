// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer RaytracingParam : register(b0)
{
	/* カメラ位置 */
    float3 eye_pos;
	/* レイの最大距離 */
    float distance;
}

struct Ray
{
	/* 位置 */
    float3 pos;
	/* 向き */
    float3 direction;
};

struct ComputeThreadID
{
	uint3 group_thread_ID : SV_GroupThreadID;
	uint3 group_ID        : SV_GroupID;
	uint3 dispatch_ID     : SV_DispatchThreadID;
	uint group_index      : SV_GroupIndex;
};

/* 背景色の算出 */
float3 BackColor(in Ray ray)
{
	/* 0～1の範囲に */
    float t = 0.5f * ray.direction.y + 1.0f;
	/* 線形補完 */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
	/* テクスチャサイズの取得 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* アスペクト比の算出 */
    const float aspect = float(size.x) / float(size.y);
	/* UV値の算出 */
    const float2 uv = float2(semantics.dispatch_ID.xy) / float2(size);
	/* ビューポート */
    const float2 viewport = aspect * 2.0f;
	/* 左上座標 */
    float3 left_pos = eye_pos - float3(viewport, distance);
	
    Ray ray = { eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos };
	
    tex[semantics.dispatch_ID.xy] = float4(BackColor(ray), 1.0f);
}
