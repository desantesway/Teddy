#pragma once

#include "Teddy/Core/Base.h"

namespace Teddy
{

	struct FramebufferSpecification
	{
		uint32_t Width = 1920;
		uint32_t Height = 1080;
		uint32_t Samples = 1; // Multisampling
		bool SwapChainTarget = false; // If true, this framebuffer is used as a swapchain target
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}