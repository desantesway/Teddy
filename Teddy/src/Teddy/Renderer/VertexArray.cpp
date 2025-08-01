#include "TeddyPch.h"
#include "Teddy/Renderer/VertexArray.h"

#include "Teddy/Renderer/Renderer.h"

#if TED_PLATFORM_WINDOWS
	#include "Platform/OpenGL/OpenGLVertexArray.h"
#endif

namespace Teddy 
{

	Ref<VertexArray> VertexArray::Create() 
	{

		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:
				TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			#if TED_PLATFORM_WINDOWS
			case RendererAPI::API::OpenGL:    return CreateRef<OpenGLVertexArray>();
			#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (VertexArray.cpp)");
		return nullptr;
	}

}