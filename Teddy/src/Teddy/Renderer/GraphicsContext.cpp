#include "teddyPch.h"
#include "Teddy/Renderer/GraphicsContext.h"

#include "Teddy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Teddy 
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<SDL_Window*>(window));
		}
		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
} // namespace Teddy