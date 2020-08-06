#include "Texture.hlsli"

float4 main(in Output output) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);

	//return tex.Sample(smp, output.uv);
}
