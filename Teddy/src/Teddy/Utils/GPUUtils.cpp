#include "Teddy/Utils/GPUUtils.h"
#include "Teddy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUtils.h"

namespace Teddy::GPUUtils
{
	uint32_t GetMaxImageUnits()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return 1;
		case RendererAPI::API::OpenGL:  return OpenGLUtils::GetMaxImageUnits();
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return 1;
	}
}