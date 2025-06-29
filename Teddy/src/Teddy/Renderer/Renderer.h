#pragma once

namespace Teddy {

	enum class RendererAPI {
		None = 0,
		OpenGL = 1,
		Vulkan = 2,
		DirectX11 = 3,
		DirectX12 = 4,
		Metal = 5
	};

	class Renderer {
	public:
		inline static void SetAPI(RendererAPI API) { s_RendererAPI = API; }
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}