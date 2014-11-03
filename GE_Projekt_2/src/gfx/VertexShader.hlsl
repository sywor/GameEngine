#include "resources.hlsl"

struct VS_INPUT
{
	float4 pos		: POSITION;
	float4 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float4 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};


PS_INPUT main(VS_INPUT _input)
{
	PS_INPUT output = (PS_INPUT)0;


	matrix ViewProjection = mul(view, projection);
	matrix worldviewproj = mul(world, ViewProjection);

	output.pos = mul(_input.pos, worldviewproj);

	//output.pos = mul(_input.pos, world);
	//output.pos = mul(output.pos, view);
	//output.pos = mul(output.pos, projection);



	output.normal = _input.pos;
	//output.normal = mul(_input.normal, world);
	//output.normal = mul(output.normal, view);
	//output.normal = mul(output.normal, projection);

	output.tex = _input.tex;

	return output;
}
