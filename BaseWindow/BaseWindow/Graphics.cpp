#include "Graphics.h"


Graphics::Graphics()
{
	createBackBuffer();
	createShader("VertexShader", "vs_5_0");
	createShader("PixelShader", "ps_5_0");
	createSampler();
	createBuffers();
}


Graphics::~Graphics()
{
}

HRESULT Graphics::Update(float _deltaTime)
{
	D3DXMATRIX world;

	cbWorld.world = world;
	cbWorld.worldInv = world;
	cbWorld.view = Cam->getViewMatrix();
	cbWorld.projection = Cam->getProjectionMatrix();
	g_DeviceContext->UpdateSubresource(g_cbWorld,0,NULL,&cbWorld,0,0);



	return S_OK;
}

HRESULT Graphics::Render(float _deltaTime)
{
	//D3DXCOLOR color = D3DXCOLOR(Cam->getCameraPosition().x, Cam->getCameraPosition().y, Cam->getCameraPosition().z,0);

	g_DeviceContext->ClearRenderTargetView(g_backBuffer, D3DXCOLOR(1.0f, 0.6f, 0.6f, 0));

	//g_DeviceContext->IASetVertexBuffers();
	//g_DeviceContext->Draw(,0);

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
		createInputLayout(shaderBlob, g_geometryLayout);

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

	g_DeviceContext->PSSetSamplers(0, 1, &samLinear);
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
