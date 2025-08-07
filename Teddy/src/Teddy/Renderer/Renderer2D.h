#pragma once

#include "Teddy/Renderer/OrthographicCamera.h"
#include "Teddy/Renderer/Camera.h"

#include "Teddy/Renderer/Texture.h"

namespace Teddy 
{
	struct RenderCamera {
		glm::mat4 ViewMatrix;
		glm::mat4 Projection;
	};

	struct Quad 
	{
		Ref<Texture2D> Texture = nullptr;
		glm::vec4 Color{1.0f};
		float TilingFactor = 1.0f;
	};

	class Renderer2D 
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove this
		static void EndScene();

		static void Flush();

		static void SetQuad(const glm::mat4& transform, const glm::vec4& color,
			const float& texIndex, const float& tilingFactor);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Quad& quad, float rotation = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Quad& quad, float rotation = 0.0f);
		static void DrawQuad(const glm::mat4& transform, const Quad& quad);
	
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();
	};

}