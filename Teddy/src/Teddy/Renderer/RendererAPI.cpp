#include "TeddyPch.h"
#include "Teddy/Renderer/RendererAPI.h"

#ifdef TED_PLATFORM_WINDOWS
	#include "Platform/OpenGL/OpenGLRendererAPI.h"
#endif

namespace Teddy 
{

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}
		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}