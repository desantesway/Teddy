#pragma once

#include "Teddy/Renderer/Framebuffer.h"

namespace Teddy
{

	class PostProcessing
	{
	public:
		static void Init();
		static void Shutdown();

		static void Apply(Ref<Framebuffer> framebuffer);
	};
}