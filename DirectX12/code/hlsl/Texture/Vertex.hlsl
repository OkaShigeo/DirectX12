#include "Texture.hlsli"

[RootSignature(RS)]
Output main(in Input input)
{
	Output output;
	output.sv_pos = input.pos;
	output.pos    = input.pos;
	output.uv     = input.uv;

	return output;
}
