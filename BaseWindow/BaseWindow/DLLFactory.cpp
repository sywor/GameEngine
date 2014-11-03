#include "DLLFactory.h"

RenderInterface* createRenderer(HWND _hwnd)
{
	return new RenderInterface(_hwnd);
}