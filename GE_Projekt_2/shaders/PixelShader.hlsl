#include "resources.hlsl"

struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float4 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};

Texture2D tex		: register(t0);

float4 main(PS_INPUT _input) : SV_TARGET
{

	float4 color = tex.Sample(samLinear, _input.tex);

	if (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0)
		return  float4(1, 1, 1, 1.0f);
	else
		return color;

}
