#pragma once

#include "RenderInterface.h"
#include <Windows.h>
/* Rendering Domain (this is possibly superfluous) */

namespace Renderer
{
	class RenderAPI
	{
	public:
		/* This is a test function, it is useless */
		static __declspec(dllexport) void print();

		/*	Instantiate a new renderer with version number

		#_dxVersion: version number request { DX11, DX10 }

		#returns
		RenderInterface* on success
		nullptr on invalid version request
		nullptr on (target environment / version request) incompatability*/
		static __declspec(dllexport) RenderInterface* createRenderer(HWND _hwnd);

		/*	Shut down and delete rendering instance

		#_renderer: instance to be deleted*/
		static __declspec(dllexport) void deleteRenderer(RenderInterface* _renderer);
	};
}


