#pragma once

#include <SDL3/SDL.h>

namespace Teddy 
{

	class GraphicsContext
	{
	public:
		virtual void* GetContext() = 0;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Shutdown() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
	
}