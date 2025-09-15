#pragma once

#include "Teddy/Renderer/RenderCommand.h"

#include "Teddy/Renderer/OrthographicCamera.h"
#include "Teddy/Renderer/Shader.h"

namespace Teddy 
{

	class Renderer 
	{
	public:
		static void Init(const std::unordered_set<std::string>& shadersToRebuild);
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		}; 

		static Scope<SceneData> s_SceneData;
	};
}