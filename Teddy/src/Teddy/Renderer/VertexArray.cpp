#include "TeddyPch.h"
#include "VertexArray.h"

#include "Renderer.h"

#if TED_PLATFORM_WINDOWS
	#include "Platform/OpenGL/OpenGLVertexArray.h"
#endif

namespace Teddy {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI()) {
		case RendererAPI::None:
			TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		#if TED_PLATFORM_WINDOWS
			case RendererAPI::OpenGL:    return new OpenGLVertexArray();
		#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (VertexArray.cpp)");
		return nullptr;
	}

}