
#include "Buffer.h"

#include "Renderer.h"

#if TED_PLATFORM_WINDOWS
	#include "Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace Teddy {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:    
				TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			#if TED_PLATFORM_WINDOWS
			case RendererAPI::OpenGL:    return new OpenGLVertexBuffer(vertices, size);
			#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (buffer.cpp)");
		return nullptr;	
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size){
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			#if TED_PLATFORM_WINDOWS
			case RendererAPI::OpenGL:    return new OpenGLIndexBuffer(indices, size);
			#endif
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI (buffer.cpp)");
		return nullptr;
	}
}