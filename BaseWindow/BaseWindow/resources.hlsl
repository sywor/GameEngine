#define PI 3.14159265f

SamplerState samLinear	: register(s0);

cbuffer cbWorld			: register(b0)
{
	matrix	world;			//world matrix
	matrix	view;
	matrix	projection;
	matrix	worldInv;		//world inverse matrix
};

//cbuffer cbEveryFrame	: register(b3)
//{
//	float	timeStep;
//	float	gameTime;
//};