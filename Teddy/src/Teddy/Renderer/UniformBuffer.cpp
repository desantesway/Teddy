#include "TeddyPch.h"
#include "Teddy/Renderer/UniformBuffer.h"

#include "Teddy/Renderer/Renderer.h"

#ifdef TED_PLATFORM_WINDOWS
	#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#endif


namespace Teddy
{

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
