
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "Sandbox2D.h"

class Sandbox : public Teddy::Application 
{
public:
	Sandbox(const Teddy::ApplicationSpecification& specification)
		: Teddy::Application(specification)
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox() 
	{
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication(Teddy::ApplicationCommandLineArgs args)
	
{
	ApplicationSpecification spec;

	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Tedditor";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}