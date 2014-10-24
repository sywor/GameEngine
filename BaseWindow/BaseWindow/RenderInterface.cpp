#include "RenderInterface.h"


RenderInterface::RenderInterface(HWND _hwnd)
{

	graphics = new Graphics(_hwnd,Cam);

	Cam->init(graphics->g_Width, graphics->g_Height);
	Cam->setPositionAndView(0.0f, 10.0f, 0.0f, 0.0f, 0.0f);


}


RenderInterface::~RenderInterface()
{
}

void RenderInterface::update(float _deltaTime)
{
	graphics->Update(_deltaTime);
}

void RenderInterface::render(float _deltaTime)
{
	graphics->Render(_deltaTime);
}