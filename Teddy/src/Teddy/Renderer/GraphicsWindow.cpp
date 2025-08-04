#include "TeddyPch.h"
#include "Teddy/Renderer/GraphicsWindow.h"

#include "Teddy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLWindow.h"

namespace Teddy 
{
	Scope<GraphicsWindow> GraphicsWindow::Create(const char* title, int width, int height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLWindow>(title, width, height);
		}
		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}