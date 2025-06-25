#include <Teddy.h>

#include "imgui.h"

class ExampleLayer : public Teddy::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//TED_INFO("ExampleLayer::Update");
	}

	void OnEvent(Teddy::Event& event) override
	{
		//TED_TRACE("{0}", event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Example Layer");
		ImGui::Text("Hello, this is an example layer!");
		ImGui::End();
	}

};

class KeyStar : public Teddy::Application {
public:
	KeyStar() {
		PushLayer(new ExampleLayer());
	}
	~KeyStar() {
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication() {
	return new KeyStar();
} // CreateApplication