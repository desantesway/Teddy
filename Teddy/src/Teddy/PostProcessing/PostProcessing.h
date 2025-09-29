#pragma once

#include "Teddy/Renderer/Framebuffer.h"

namespace Teddy
{

	class PostProcessing
	{
	public:
		PostProcessing();
		~PostProcessing() = default;

		static void Init();
		static void Shutdown();

		static void Bind() { m_Framebuffer->Bind(); }
		static void Unbind() { m_Framebuffer->Unbind(); }

		static void Clear() { m_Framebuffer->ClearAttachment(1, -1); }

		static void Resize(uint32_t width, uint32_t height);

		static const FramebufferSpecification& GetFramebufferSpec() { return m_FramebufferSpec; }
		static Ref<Framebuffer> GetFramebuffer() { return m_Framebuffer; }
		static PostProcessing& Get() { return *s_Instance; }

		static void Apply();
		
		enum class Effect {None = 0, ChromaticAberration};

		static bool IsEffectEnabled(Effect effect);

		static void EnableEffect(Effect effect);
		static void DisableEffect(Effect effect);

		static void SetChromaticAberrationOffset(glm::vec3 offset);
		static glm::vec3 GetChromaticAberrationOffset();

	private:
		static Ref<Framebuffer> m_Framebuffer;
		static FramebufferSpecification m_FramebufferSpec;

	private:
		static PostProcessing* s_Instance;
	};
}