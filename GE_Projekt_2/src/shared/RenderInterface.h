#pragma once
#include "..\gfx\stdafx.h"
#include "..\gfx\Graphics.h"
#include "..\gfx\ButtonInput.h"



class RenderInterface
{
public:

	ICamera* Cam = new XMCamera();
	ButtonInput* buttonInput = new ButtonInput();

	Graphics *graphics;

	RenderInterface(HWND _hwnd);
	~RenderInterface();

	ICamera* getCamera(){ return Cam; }
	ButtonInput* getButtonInput(){ return buttonInput; }

	virtual void update(float _deltaTime){ graphics->Update(_deltaTime); }
	virtual void render(float _deltaTime){ graphics->Render(_deltaTime); }
	virtual int setMesh(void* _data, int _nrOfTriangles)
	{
		return graphics->setMesh(_data,_nrOfTriangles);
	}

	virtual int addTexture(uint8_t *_data, int _sizeInBytes)
	{ 
		return graphics->createTextureView(_data, _sizeInBytes);
	}

	virtual void AddRenderJob(GraphicsJobInfo* call)
	{
		graphics->AddRenderJob( call );
	}

	virtual void release(void* _iResource) {  }
};

