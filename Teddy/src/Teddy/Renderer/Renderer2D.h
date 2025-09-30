#pragma once

#include "Teddy/Renderer/Camera.h"

#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/EditorCamera.h"
#include "Teddy/Scene/Components.h"

#include "Teddy/Renderer/Font.h"

namespace Teddy 
{
	struct RenderCamera {
		glm::mat4 ViewMatrix;
		glm::mat4 Projection;
	};

	template<typename T>
	struct BashRenderResource;

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
		static void DrawCircleLine(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

		static void SetQuad(const glm::mat4& transform, const glm::vec4& color,
			const float& texIndex, const float& tilingFactor,
			const SpriteAtlasComponent& atlas, const float& atlasWidth, const float& atlasHeight, 
			int blendMode, int entityID);
		static void SetQuad(const glm::mat4& transform, const glm::vec4& color,
			const float& texIndex, const float& tilingFactor, int blendMode, int entityID = -1);

		static void DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite, 
			const Camera& camera, const TransformComponent& cameraTransform,
			int entityID = -1);
		static void DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite, 
			const Camera& camera, const TransformComponent& cameraTransform,
			const SpriteAtlasComponent& atlas, int entityID = -1);
		static void DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID = -1);
		static void DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite,
			const SpriteAtlasComponent& atlas, int entityID = -1);

		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		template<typename T>
		static void ReloadShader(BashRenderResource<T>& resource);

		static void DrawString(const TextComponent& component, TransformComponent& transform, int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float width);
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
		static void NextBatch();
		static void StartBatch();

		enum class ShaderType {None = 0, Quad, Circle, Text};

		static ShaderType m_LastShaderDrawn;
	};

}