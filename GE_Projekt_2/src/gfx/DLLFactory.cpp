#include "DLLFactory.h"

RenderInterface* Renderer::RenderAPI::createRenderer(HWND _hwnd)
{
	return new RenderInterface(_hwnd);
}

void Renderer::RenderAPI::deleteRenderer(RenderInterface* _renderer)
{
	SAFE_DELETE(_renderer);
	//_renderer->~RenderInterface();
}