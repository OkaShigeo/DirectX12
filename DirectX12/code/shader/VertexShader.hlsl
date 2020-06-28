struct Input
{
    float4 pos : POSITION;
	float2 uv  : TEXCOORD;
};

struct Output
{
    float4 sv_pos : SV_POSITION;
    float4 pos    : POSITION;
    float2 uv     : TEXCOORD;
};

Output main(Input input)
{
    Output output;
    output.sv_pos = input.pos;
    output.pos    = input.pos;
    output.uv     = input.uv;

    return output;
}
