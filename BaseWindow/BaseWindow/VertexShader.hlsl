#include "resources.hlsl"

struct VS_INPUT
{
	float4 pos		: SV_POSITION;
	float4 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};



VS_INPUT main(VS_INPUT _input)
{
	VS_INPUT output = (VS_INPUT)0;

	output.pos = mul(_input.pos,world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.normal = mul(_input.normal, world);
	output.normal = mul(output.normal, view);
	output.normal = mul(output.normal, projection);

	output.tex = _input.tex;

	return output;
}