// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer RaytracingParam : register(b0)
{
	/* �J�����ʒu */
    float3 eye_pos;
	/* ���C�̍ő勗�� */
    float distance;
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
	/* 0�`1�͈̔͂� */
    float t = 0.5f * ray.direction.y + 1.0f;
	/* ���`�⊮ */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
	/* �e�N�X�`���T�C�Y�̎擾 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* �A�X�y�N�g��̎Z�o */
    const float aspect = float(size.x) / float(size.y);
	/* UV�l�̎Z�o */
    const float2 uv = float2(semantics.dispatch_ID.xy) / float2(size);
	/* �r���[�|�[�g */
    const float2 viewport = aspect * 2.0f;
	/* ������W */
    float3 left_pos = eye_pos - float3(viewport, distance);
	
    Ray ray = { eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos };
	
    tex[semantics.dispatch_ID.xy] = float4(BackColor(ray), 1.0f);
}
