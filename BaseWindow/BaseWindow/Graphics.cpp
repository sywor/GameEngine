#include "Graphics.h"


Graphics::Graphics(HWND _hwnd, ICamera* _cam)
{
	Cam = _cam;

	if (FAILED(InitDevice(_hwnd)))
	{
		MessageBox(NULL, "Error creating devices", "RenderDX11 Error", S_OK);
	}

	createBackBuffer();
	createShader("VertexShader", "vs_5_0");
	createShader("PixelShader", "ps_5_0");
	createSampler();
	createBuffers();
	createRasterState();
	createViewport();
	createBlendState();

	//------------------------------ temp variables for testing

	//object = new Object();
	//loader = new Loader();

	//loader->LoadObject("Bunker.obj",0,0,0,1,object,1,1,1);

	//object->InitBuffers("purple.jpg","cracked.jpg");

	for (int i = 0; i < 6; i++)
	{
		wall[i].normal	= VECTOR4(0, 0, 1, 1);
		wall[i].texC	= VECTOR2(i, i);
	}
	const float size	= 50.0f;
	const float depth	= -10.0f;
	wall[0].pos = VECTOR4(1.0 * size,	1.0 * size,		depth, 1);
	wall[1].pos = VECTOR4(1.0 * size,	1.0 * -size,	depth, 1);
	wall[2].pos = VECTOR4(1.0 * -size,	1.0 * -size,	depth, 1);
	wall[3].pos = VECTOR4(1.0 * size,	1.0 * size,		depth, 1);
	wall[4].pos = VECTOR4(1.0 * -size,	1.0 * -size,	depth, 1);
	wall[5].pos = VECTOR4(1.0 * -size,	1.0 * size,		depth, 1);

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


	//------------------------------

}


Graphics::~Graphics()
{
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
	//D3DXCOLOR color = D3DXCOLOR(Cam->getCameraPosition().x, Cam->getCameraPosition().y, Cam->getCameraPosition().z,0);
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
	g_DeviceContext->Draw(6,0);

	//g_DeviceContext->IASetVertexBuffers(0, 1, object->getBuffer(), &strides,&offset );
	//g_DeviceContext->Draw(3000,0);



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
	std::string path = "";
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
	if (_shaderModel == "gs_5_0")
	{
		result = g_Device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &g_geometryShader);
		if (FAILED(result))
		{
			std::string text = "Failed to create " + _shader;
			MessageBox(NULL, text.c_str(), "Shader Error", MB_OK);
			PostQuitMessage(0);

		}
		//createInputLayout(shaderBlob, g_geometryLayout);

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