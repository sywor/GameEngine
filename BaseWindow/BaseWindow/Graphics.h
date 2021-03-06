#pragma once
#include "stdafx.h"
#include "Camera.h"

//--------------------------------- temp includes for testing

//#include "Object.h"
//#include "Loader.h"
#include "ICamera.h"
#include "XMCamera.h"
//---------------------------------






class Graphics
{
public:
	Graphics(HWND _hwnd,ICamera* _cam);
	~Graphics();
	HRESULT Update(float _deltaTime);
	HRESULT Render(float _deltaTime);

	int g_Width, g_Height;
	//---------------------------------- temporary variables for testing

	//Loader* loader;
	//Object* object;

	Vertex wall[6];

	ID3D11Device* g_Device;
	ID3D11DeviceContext* g_DeviceContext;
	IDXGISwapChain* g_SwapChain;


	ICamera* Cam;


	//----------------------------------

private:

	HRESULT InitDevice(HWND _hwnd);
	void createBackBuffer();
	void createShader(std::string _shader, std::string _shaderModel);
	void createInputLayout(ID3DBlob *_vertexBlob, ID3D11InputLayout* _layout);
	void createSampler();
	void createBuffers();
	void createRasterState();
	void createViewport();
	void createBlendState();

	cbWorld cbWorld;
	ID3D11Buffer* g_cbWorld = NULL;
	ID3D11Buffer* g_vertexBuffer;

	ID3D11RenderTargetView*  g_backBuffer = NULL;
	ID3D11VertexShader* g_vertexShader = NULL;
	ID3D11InputLayout* g_vertexLayout = NULL;
	ID3D11GeometryShader* g_geometryShader = NULL;
	ID3D11InputLayout* g_geometryLayout = NULL;
	ID3D11PixelShader* g_pixelShader = NULL;
	ID3D11InputLayout* g_pixelLayout = NULL;
	ID3D11SamplerState *samLinear = NULL;
	D3D11_VIEWPORT viewport;
	ID3D11BlendState* g_blendState = NULL;
	ID3D11Texture2D*	g_pDepthStencil = NULL;
	ID3D11DepthStencilView* g_pDepthStencilView = NULL;
	ID3D11DepthStencilState*	g_depthStencilState = NULL;

	ID3D11RasterizerState *rasterState = NULL;



};

