#include "KeyStar2D.h"

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped) Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		
		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}

private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

KeyStar2D::KeyStar2D() 
	: Layer("Example"), m_CameraController(1920.0f / 1080.0f, true)
{

}

void KeyStar2D::OnAttach()
{
	m_SustainTexture = Teddy::Texture2D::Create("assets/textures/piano/pedal/sus.png");
}

void KeyStar2D::OnDetach()
{
}

void KeyStar2D::OnUpdate(Teddy::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		PROFILE_SCOPE("Renderer Prep");
		Teddy::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw (CPU)");
		Teddy::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
		Teddy::Renderer2D::DrawQuad({ 0.0f, -1.0f }, { 1.0f, 1.0f }, m_SquareColor);
		Teddy::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.25f, 1.0f }, {0.9f, 0.3f, 0.1f, 1.0f});
		Teddy::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_SustainTexture);
		Teddy::Renderer2D::EndScene();
	}
}

void KeyStar2D::OnEvent(Teddy::Event& event)
{
	m_CameraController.OnEvent(event);

}

void KeyStar2D::OnImGuiRender() 
{
	ImGui::Begin("Settings");
	ImGui::Text("ChangeColor");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (const auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}