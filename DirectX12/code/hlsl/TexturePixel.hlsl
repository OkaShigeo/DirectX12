#include "Texture.hlsli"

float4 main(in Output output) : SV_TARGET
{
	return tex.Sample(smp, output.uv);
}
