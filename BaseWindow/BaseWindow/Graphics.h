#pragma once
#include "stdafx.h"
#include "Camera.h"

extern ID3D11Device* g_Device;
extern ID3D11DeviceContext* g_DeviceContext;
extern IDXGISwapChain* g_SwapChain;

extern Camera* Cam;

class Graphics
{
public:
	Graphics();
	~Graphics();
	HRESULT Update(float _deltaTime);
	HRESULT Render(float _deltaTime);

private:

	void createBackBuffer();
	void createShader(std::string _shader, std::string _shaderModel);
	void createInputLayout(ID3DBlob *_vertexBlob, ID3D11InputLayout* _layout);
	void createSampler();
	void createBuffers();


	cbWorld cbWorld;
	ID3D11Buffer* g_cbWorld = NULL;


	ID3D11RenderTargetView*  g_backBuffer = NULL;
	ID3D11VertexShader* g_vertexShader = NULL;
	ID3D11InputLayout* g_vertexLayout = NULL;
	ID3D11GeometryShader* g_geometryShader = NULL;
	ID3D11InputLayout* g_geometryLayout = NULL;
	ID3D11PixelShader* g_pixelShader = NULL;
	ID3D11InputLayout* g_pixelLayout = NULL;
	ID3D11SamplerState *samLinear = NULL;




};

