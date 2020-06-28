Texture2D<float4>tex : register(t0);
SamplerState smp     : register(s0);

struct Output
{
    float4 sv_pos : SV_POSITION;
    float4 pos    : POSITION;
    float2 uv     : TEXCOORD;
};

float4 main(Output output) : SV_TARGET
{
	return tex.Sample(smp, output.uv);
}
