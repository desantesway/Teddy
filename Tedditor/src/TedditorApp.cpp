
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "TedditorLayer.h"

class Sandbox : public Teddy::Application 
{
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication() 
{
	return new Sandbox();
} // CreateApplication