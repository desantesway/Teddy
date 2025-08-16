#pragma once

#include "Teddy/Renderer/OrthographicCamera.h"
#include "Teddy/Renderer/Camera.h"

#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/EditorCamera.h"
#include "Teddy/Scene/Components.h"

namespace Teddy 
{
	struct RenderCamera {
		glm::mat4 ViewMatrix;
		glm::mat4 Projection;
	};

	class Renderer2D 
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

		static void SetQuad(const glm::mat4& transform, const glm::vec4& color,
			const float& texIndex, const float& tilingFactor, int entityID = -1);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, 
			const SpriteRendererComponent& sprite, float rotation = 0.0f, int entityID = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, 
			const SpriteRendererComponent& sprite, float rotation = 0.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const SpriteRendererComponent& sprite, int entityID = -1);
	
		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		float GetLineWidth();
		void SetLineWidth(float width);

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
		static void StartBatch();
		static void NextBatch();
	};

}