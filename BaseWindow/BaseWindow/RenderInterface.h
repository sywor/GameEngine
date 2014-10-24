#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "ButtonInput.h"



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

	void update(float _deltaTime);
	void render(float _deltaTime);
};

