#pragma once
#include "stdafx.h"

#include "ICamera.h"
#include "XMCamera.h"

#include <vector>


#define SafeRelease(x) if(x) x->Release() ; x = NULL;

struct GraphicsJobInfo;


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

	int createTextureView(uint8_t *_data, int _sizeInBytes);
	int setMesh(void* _data, int _nrOfTriangles);

	//----------------------------------

	void AddRenderJob(GraphicsJobInfo* call);
	void releaseTexture(int index)
	{
		if (index >= 0 && index < textures.size())
		{
			SAFE_RELEASE(textures[index]);
			SAFE_DELETE(textures[index]);
		}
	}

	void releaseBuffer(int index)
	{
		if (index >= 0 && index < vertexBuffers.size())
		{
			SAFE_RELEASE(vertexBuffers[index].buffer);
			SAFE_DELETE(vertexBuffers[index].buffer);
		}
	}
private:

	HRESULT InitDevice(HWND _hwnd);
	void createBackBuffer();

	void createShader();
	void createShader(std::string _shader, std::string _shaderModel);

	void createInputLayout(ID3DBlob *_vertexBlob, ID3D11InputLayout* _layout);
	void createSampler();
	void createBuffers();
	void createRasterState();
	void createViewport();
	void createBlendState();

	template<typename IResource>
	void release(IResource* resource){SAFE_RELEASE(resource);}

	cbWorld cbWorld;
	ID3D11Buffer* g_cbWorld = NULL;
	//ID3D11Buffer* g_vertexBuffer;
	//ID3D11ShaderResourceView *texture = nullptr;
	//std::map<uint64_t, ID3D11ShaderResourceView*>	srvs;
	//std::map<uint64_t, ID3D11Buffer*>				buffers;

	struct vertexBufferStruct
	{
		ID3D11Buffer* buffer;
		int nrOfTriangles;
	};

	std::vector< GraphicsJobInfo* > jobs;
	std::vector< vertexBufferStruct > vertexBuffers;
	std::vector< ID3D11ShaderResourceView* > textures;


	ID3D11RenderTargetView*  g_backBuffer = NULL;
	ID3D11VertexShader* g_vertexShader = NULL;
	ID3D11InputLayout* g_vertexLayout = NULL;
	ID3D11PixelShader* g_pixelShader = NULL;
	ID3D11SamplerState *samLinear = NULL;
	D3D11_VIEWPORT viewport;
	ID3D11BlendState* g_blendState = NULL;
	ID3D11Texture2D*	g_pDepthStencil = NULL;
	ID3D11DepthStencilView* g_pDepthStencilView = NULL;
	ID3D11DepthStencilState*	g_depthStencilState = NULL;

	ID3D11RasterizerState *rasterState = NULL;

	int nrOfTriangles;


};

