
#include "Teddy/Renderer/Buffer.h"

#include "Teddy/Renderer/Renderer.h"

#if TED_PLATFORM_WINDOWS
	#include "Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace Teddy {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
			#if TED_PLATFORM_WINDOWS
					case RendererAPI::API::OpenGL:    return CreateRef<OpenGLVertexBuffer>(size);
			#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (buffer.cpp)");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:    
				TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			#if TED_PLATFORM_WINDOWS
			case RendererAPI::API::OpenGL:    return CreateRef<OpenGLVertexBuffer>(vertices, size);
			#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (buffer.cpp)");
		return nullptr;	
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count){
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			#if TED_PLATFORM_WINDOWS
			case RendererAPI::API::OpenGL:    return CreateRef<OpenGLIndexBuffer>(indices, count);
			#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (buffer.cpp)");
		return nullptr;
	}
}