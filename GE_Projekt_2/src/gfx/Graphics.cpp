#include "Graphics.h"

#include <comdef.h>
#include <wincodec.h>

#include "WICTextureLoader\WICTextureLoader.h"

bool IsError(HRESULT _hr)
{
	if (FAILED(_hr))
	{
		_com_error err(_hr);
		LPCTSTR errMsg = err.ErrorMessage();
		MessageBox(NULL, errMsg, "SRV Error", MB_OK);
		return true;
	}
	return false;
}

char* FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	if (featureLevel == D3D_FEATURE_LEVEL_11_0)
		return "11.0";
	if (featureLevel == D3D_FEATURE_LEVEL_10_1)
		return "10.1";
	if (featureLevel == D3D_FEATURE_LEVEL_10_0)
		return "10.0";

	return "Unknown";
}

template< class ShaderClass >
std::string GetLatestProfile(ID3D11Device* g_Device);

template<>
std::string GetLatestProfile<ID3D11VertexShader>(ID3D11Device* g_Device)
{
	assert(g_Device);

	// Query the current feature level:
	D3D_FEATURE_LEVEL featureLevel = g_Device->GetFeatureLevel();

	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
	{
								   return "vs_5_0";
	}
		break;
	case D3D_FEATURE_LEVEL_10_1:
	{
								   return "vs_4_1";
	}
		break;
	case D3D_FEATURE_LEVEL_10_0:
	{
								   return "vs_4_0";
	}
		break;
	case D3D_FEATURE_LEVEL_9_3:
	{
								  return "vs_4_0_level_9_3";
	}
		break;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:
	{
								  return "vs_4_0_level_9_1";
	}
		break;
	} // switch( featureLevel )

	return "";
}

template<>
std::string GetLatestProfile<ID3D11PixelShader>(ID3D11Device* g_Device)
{
	assert(g_Device);

	// Query the current feature level:
	D3D_FEATURE_LEVEL featureLevel = g_Device->GetFeatureLevel();
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
	{
								   return "ps_5_0";
	}
		break;
	case D3D_FEATURE_LEVEL_10_1:
	{
								   return "ps_4_1";
	}
		break;
	case D3D_FEATURE_LEVEL_10_0:
	{
								   return "ps_4_0";
	}
		break;
	case D3D_FEATURE_LEVEL_9_3:
	{
								  return "ps_4_0_level_9_3";
	}
		break;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:
	{
								  return "ps_4_0_level_9_1";
	}
		break;
	}
	return "";
}

template< class ShaderClass >
ShaderClass* CreateShader(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage, ID3D11Device* g_Device);

template<>
ID3D11VertexShader* CreateShader<ID3D11VertexShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage, ID3D11Device* g_Device)
{
	HRESULT hr = S_OK;
	assert(g_Device);
	assert(pShaderBlob);

	ID3D11VertexShader* pVertexShader = nullptr;
	hr = g_Device->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &pVertexShader);

	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create vertex shader", "Shader Error", MB_OK);
	}

	return pVertexShader;
}

template<>
ID3D11PixelShader* CreateShader<ID3D11PixelShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage, ID3D11Device* g_Device)
{
	HRESULT hr = S_OK;
	assert(g_Device);
	assert(pShaderBlob);

	ID3D11PixelShader* pPixelShader = nullptr;
	hr = g_Device->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &pPixelShader);

	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create pixel shader", "Shader Error", MB_OK);
	}

	return pPixelShader;
}

template< class ShaderClass >
ShaderClass* LoadShader(const std::wstring& fileName, const std::string& entryPoint, const std::string& _profile, ID3D11Device* g_Device)
{
	ID3DBlob* pShaderBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	ShaderClass* pShader = nullptr;

	std::string profile = _profile;
	if (profile == "latest")
	{
		profile = GetLatestProfile<ShaderClass>(g_Device);
	}

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(fileName.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), profile.c_str(),
		flags, 0, &pShaderBlob, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			std::string errorMessage = (char*)pErrorBlob->GetBufferPointer();
			//OutputDebugStringA(errorMessage.c_str());

			MessageBox(NULL, errorMessage.c_str(), "Shader Error", MB_OK);


			SafeRelease(pShaderBlob);
			SafeRelease(pErrorBlob);
		}

		return false;
	}

	pShader = CreateShader<ShaderClass>(pShaderBlob, nullptr, g_Device);

	SafeRelease(pShaderBlob);
	SafeRelease(pErrorBlob);

	return pShader;
}

void Graphics::createShader()
{

	g_vertexShader = LoadShader<ID3D11VertexShader>(L"shaders/VertexShader.hlsl", "main", "latest", g_Device);
	g_pixelShader = LoadShader<ID3D11PixelShader>(L"shaders/PixelShader.hlsl", "main", "latest", g_Device);

	if (g_vertexShader == NULL || g_pixelShader == NULL)
	{
		MessageBox(NULL, "Failed to create one or more shaders", "Shader Error", MB_OK);
		PostQuitMessage(0);
	}
}

Graphics::Graphics(HWND _hwnd, ICamera* _cam)
{
	
	Cam = _cam;

	if (FAILED(InitDevice(_hwnd)))
	{
		MessageBox(NULL, "Error creating devices", "RenderDX11 Error", S_OK);
	}

	createBackBuffer();
	//createShader();

	createShader("VertexShader","vs_5_0");
	createShader("PixelShader", "ps_5_0");

	createSampler();
	createBuffers();
	createRasterState();
	createViewport();
	createBlendState();

	//------------------------------ temp variables for testing

	for (int i = 0; i < 6; i++)
	{
		wall[i].normal	= VECTOR4(0, 0, 1, 1);
		wall[i].texC	= VECTOR2(i, i);
	}
	const float size	= 5;
	const float depth	= 10.0f;
	wall[0].pos = VECTOR4(1.0 * size,	1.0 * size,		depth, 1);
	wall[1].pos = VECTOR4(1.0 * size,	1.0 * -size,	depth, 1);
	wall[2].pos = VECTOR4(1.0 * -size,	1.0 * -size,	depth, 1);
	wall[3].pos = VECTOR4(1.0 * size,	1.0 * size,		depth, 1);
	wall[4].pos = VECTOR4(1.0 * -size,	1.0 * -size,	depth, 1);
	wall[5].pos = VECTOR4(1.0 * -size,	1.0 * size,		depth, 1);

	wall[0].texC = VECTOR2(1, 1);
	wall[1].texC = VECTOR2(1, 0);
	wall[2].texC = VECTOR2(0, 0);
	wall[3].texC = VECTOR2(1, 1);
	wall[4].texC = VECTOR2(0, 0);
	wall[5].texC = VECTOR2(0, 1);


	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(Vertex) * 6;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = S_OK;
	result = g_Device->CreateBuffer(&bufferDesc, NULL, &g_vertexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating dynamic vertex buffer", "RenderDX11 Error", S_OK);
	}


	D3D11_MAPPED_SUBRESOURCE updateData;
	ZeroMemory(&updateData, sizeof(updateData));

	if (!FAILED(g_DeviceContext->Map(g_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &updateData)))
		memcpy(updateData.pData, &wall[0], sizeof(Vertex)* 6);

	g_DeviceContext->Unmap(g_vertexBuffer, 0);

	nrOfTriangles = 6;
	//------------------------------

}

template<typename IResource>
void releaseMappedResources(std::map<uint64_t, IResource*> &_resourceMap)
{
	std::map<uint64_t, IResource*>::iterator itr = _resourceMap.begin();
	while (itr != _resourceMap.end())
	{
		SAFE_RELEASE(itr->second);
		SAFE_DELETE(itr->second);
		itr++;
	}
	_resourceMap.clear();
}
Graphics::~Graphics()
{
	releaseMappedResources(srvs);
	releaseMappedResources(buffers);
}

HRESULT Graphics::Update(float _deltaTime)
{
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX transpose = XMMatrixTranspose(world);
	XMFLOAT4X4 wt;
	XMStoreFloat4x4(&wt, transpose);

	cbWorld.world = wt;
	cbWorld.worldInv = wt;
	cbWorld.view = Cam->getViewMatrix();
	cbWorld.projection = Cam->getProjectionMatrix();
	g_DeviceContext->UpdateSubresource(g_cbWorld,0,NULL,&cbWorld,0,0);

	return S_OK;
}

HRESULT Graphics::Render(float _deltaTime)
{
	g_DeviceContext->PSSetShaderResources(0, 1, &srvs[42]);

	g_DeviceContext->VSSetShader(g_vertexShader,NULL,0);
	g_DeviceContext->PSSetShader(g_pixelShader,NULL,0);
	g_DeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	g_DeviceContext->ClearRenderTargetView(g_backBuffer,COLOR(0.0f, 0.0f, 0.0f, 0));

	g_DeviceContext->IASetInputLayout(g_vertexLayout);

	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_DeviceContext->RSSetState(rasterState);

	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	g_DeviceContext->OMSetRenderTargets(1, &g_backBuffer,NULL);


	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	g_DeviceContext->OMSetBlendState(g_blendState, blendFactor, 0xffffffff);


	g_DeviceContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &strides, &offset);
	g_DeviceContext->Draw(nrOfTriangles,0);


	// Presenting swapchain
	if (FAILED(g_SwapChain->Present(0, 0)))
		return E_FAIL;

	return S_OK;
}

void Graphics::createBackBuffer()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		MessageBox(NULL, "Failed Making Constant Buffer", "Create Buffer", MB_OK);
	// create shader unordered access view on back buffer for compute shader to write into texture
	hr = g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_backBuffer);
	//pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = g_Width;
	descDepth.Height = g_Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_Device->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr))
		MessageBox(NULL, "Failed Making Depth Stencil", "Create Buffer", MB_OK);

	// depth stencil state
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = g_Device->CreateDepthStencilState(&depthStencilDesc, &g_depthStencilState);
	if (FAILED(hr))
		MessageBox(NULL, "Failed Making Depth Stencil", "Create Buffer", MB_OK);

	g_DeviceContext->OMSetDepthStencilState(g_depthStencilState, 1);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		MessageBox(NULL, "Failed Making Depth Stencil", "Create Buffer", MB_OK);

	g_DeviceContext->OMSetRenderTargets(1, &g_backBuffer, g_pDepthStencilView);


}

void Graphics::createShader(std::string _shader, std::string _shaderModel)
{
	HRESULT result = S_OK;
	ID3DBlob* shaderBlob = NULL;
	std::string path = "shaders/";
	std::string file = path + _shader + ".cso";

	std::wstring wfile;
	wfile.assign(file.begin(), file.end());
	size_t shaderSize;

	result = D3DReadFileToBlob(wfile.c_str(), &shaderBlob);
	if (FAILED(result))
	{
		std::string text = "Failed to load precompiled shader " + _shader;
		MessageBox(NULL, text.c_str(), "Shader Error", MB_OK);
		PostQuitMessage(0);

	}

	if (_shaderModel == "vs_5_0")
	{

		result = g_Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &g_vertexShader);
		if (FAILED(result))
		{
			std::string text = "Failed to create " + _shader;
			MessageBox(NULL, text.c_str(), "Shader Error", MB_OK);
			PostQuitMessage(0);

		}
		createInputLayout(shaderBlob, g_vertexLayout);
		shaderBlob->Release();
	}
	if (_shaderModel == "ps_5_0")
	{
		result = g_Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &g_pixelShader);
		if (FAILED(result))
		{
			std::string text = "Failed to create " + _shader;
			MessageBox(NULL, text.c_str(), "Shader Error", MB_OK);
			PostQuitMessage(0);

		}
		shaderBlob->Release();
	}

}

void Graphics::createInputLayout(ID3DBlob *_vertexBlob, ID3D11InputLayout* _layout)
{
	HRESULT result = S_OK;
	ID3D11ShaderReflection* vertexShaderReflection = NULL;
	result = D3DReflect(_vertexBlob->GetBufferPointer(), _vertexBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection);
	if (FAILED(result))
		MessageBox(NULL, "FAIL", "Shader Error", MB_OK);
	D3D11_SHADER_DESC shaderDesc;
	result = vertexShaderReflection->GetDesc(&shaderDesc);
	if (FAILED(result))
		MessageBox(NULL, "FAIL", "Shader Error", MB_OK);


	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	byte animSlot = 255;

	for (UINT32 i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		result = vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);
		if (FAILED(result))
			MessageBox(NULL, "FAIL", "Shader Error", MB_OK);
		std::string semantic = paramDesc.SemanticName;
		if (animSlot == 255)
		{
			if (semantic == "PERINST")
				animSlot = 2;
			else if (semantic == "ANIM")
				animSlot = 1;
		}
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		if (semantic == "PERINST")
		{
			if (animSlot != 1)
				elementDesc.InputSlot = 1;
			else
				elementDesc.InputSlot = 2;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}
		else if (semantic == "ANIM")
		{
			elementDesc.InputSlot = animSlot;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
		}
		else
		{
			elementDesc.InputSlot = 0;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
		}





		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		inputLayoutDesc.push_back(elementDesc);
	}

	result = g_Device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), _vertexBlob->GetBufferPointer(), _vertexBlob->GetBufferSize(), &_layout);
	if (FAILED(result))
	{
		std::string text = "Failed to create inputLayout with index: ";
		MessageBox(NULL, text.c_str(), "Shader Error", MB_OK);
		PostQuitMessage(0);
	}
	g_vertexLayout = _layout;
	vertexShaderReflection->Release();
}

void Graphics::createSampler()
{
	HRESULT result;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = -D3D11_FLOAT32_MAX;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = g_Device->CreateSamplerState(&sampDesc, &samLinear);
	if (FAILED(result))
		MessageBox(NULL, "FAIL", "Sampler Error", MB_OK);

	g_DeviceContext->PSSetSamplers(1, 1, &samLinear);
}

void Graphics::createBuffers()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	// CPU writable, should be updated per frame
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	if (sizeof(cbWorld) % 16 < 16)
	{
		cbDesc.ByteWidth = (int)((sizeof(cbWorld) / 16) + 1) * 16;
	}
	else
	{
		cbDesc.ByteWidth = sizeof(cbWorld);
	}


	hr = g_Device->CreateBuffer(&cbDesc, NULL, &g_cbWorld);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed Making Constant Buffer", "Create Buffer", MB_OK);
	}

	g_DeviceContext->VSSetConstantBuffers(0, 1, &g_cbWorld);
	g_DeviceContext->GSSetConstantBuffers(0, 1, &g_cbWorld);
	g_DeviceContext->PSSetConstantBuffers(0, 1, &g_cbWorld);
}

void Graphics::createRasterState()
{

	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determinatie how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = g_Device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		MessageBox(NULL, "Failed Making Constant Buffer", "Create Buffer", MB_OK);

	}
}

void Graphics::createViewport()
{

	// Setup the viewport for rendering.
	viewport.Width = (float)WIDTH;
	viewport.Height = (float)HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	g_DeviceContext->RSSetViewports(1, &viewport);
}

void Graphics::createBlendState()
{
	HRESULT result = S_OK;


	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = g_Device->CreateBlendState(&blendDesc, &g_blendState);
	if (FAILED(result))
		MessageBox(NULL, "Failed Making Blendstate", "Create Blendstate", MB_OK);

}

void Graphics::createTextureView(uint8_t *_data, int _sizeInBytes)
{
	HRESULT hr = CoInitialize(NULL);
	ID3D11ShaderResourceView* srv = nullptr;
	if (IsError(hr))
	{
	}


	hr = CreateWICTextureFromMemory(g_Device, g_DeviceContext, (const uint8_t*)&_data[0], (size_t)_sizeInBytes, nullptr, &srv, NULL);

	if (IsError(hr))
	{
	}

	srvs[42] = srv;
}

HRESULT Graphics::InitDevice(HWND _hwnd)
{
	HRESULT hr = S_OK;;

	RECT rc;
	GetClientRect(_hwnd, &rc);
	g_Width = rc.right - rc.left;
	g_Height = rc.bottom - rc.top;


	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = g_Width;
	sd.BufferDesc.Height = g_Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	sd.OutputWindow = _hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&sd,
			&g_SwapChain,
			&g_Device,
			&initiatedFeatureLevel,
			&g_DeviceContext);

		if (SUCCEEDED(hr))
		{
			char title[256];
			sprintf_s(
				title,
				sizeof(title),
				"Basic d3dx%s window",
				FeatureLevelToString(initiatedFeatureLevel)
				);
			SetWindowText(_hwnd, title);

			break;
		}
	}
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void Graphics::setMesh(void* _data, int _nrOfTriangles)
{

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(Vertex)* _nrOfTriangles;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = S_OK;
	result = g_Device->CreateBuffer(&bufferDesc, NULL, &g_vertexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating dynamic vertex buffer", "RenderDX11 Error", S_OK);
	}

	D3D11_MAPPED_SUBRESOURCE updateData;
	ZeroMemory(&updateData, sizeof(updateData));

	if (!FAILED(g_DeviceContext->Map(g_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &updateData)))
		memcpy(updateData.pData, _data, sizeof(Vertex)* 6);

	g_DeviceContext->Unmap(g_vertexBuffer, 0);

	nrOfTriangles = _nrOfTriangles;
	//---------------------
}

void Graphics::AddRenderJob(GraphicsJobInfo* call)
{
	jobs.push_back( call );
}

