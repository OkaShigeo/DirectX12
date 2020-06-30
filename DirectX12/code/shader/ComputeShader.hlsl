// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer Info : register(b0)
{
	/* �J�����ʒu */
    float3 eye_pos;
}

struct Ray
{
	/* �ʒu */
    float3 pos;
	/* ���� */
    float3 direction;
};

struct ComputeThreadID
{
	uint3 group_thread_ID : SV_GroupThreadID;
	uint3 group_ID        : SV_GroupID;
	uint3 dispatch_ID     : SV_DispatchThreadID;
	uint group_index      : SV_GroupIndex;
};

/* �w�i�F�̎Z�o */
float3 BackColor(in Ray ray)
{
    float t = 0.5f * ray.direction.y + 1.0f;
    return (1.0f - t) + (t * float3(0.5f, 0.7f, 1.0f));
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
	/* ���_���W */
    const float3 origin = float3(0.0f, 0.0f, 1.0f);
	/* �ő�[�x */
    const float depth = 1.0f;
	
	/* �e�N�X�`���T�C�Y�̎擾 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* �A�X�y�N�g��̎Z�o */
    float aspect = float(size.x) / float(size.y);
	/* �r���[�|�[�g */
    float2 viewport = aspect * 2.0f;
	/* ������W */
    float3 left_pos = origin - float3(viewport, depth);
	/* UV�l�̎Z�o */
    float2 uv = float2(semantics.dispatch_ID.xy) / float2(size);
	
    Ray ray = { origin, left_pos + float3(viewport * uv, 0.0f) - origin };
	
    tex[semantics.dispatch_ID.xy] = float4(BackColor(ray), 1.0f);
}
