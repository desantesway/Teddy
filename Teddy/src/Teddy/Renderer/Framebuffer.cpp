#include "TeddyPch.h"
#include "Teddy/Renderer/Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include "Teddy/Renderer/Renderer.h"

namespace Teddy
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLFramebuffer>(spec);
		}
		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}