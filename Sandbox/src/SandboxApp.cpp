
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
		PushLayer(CreateRef<Sandbox2D>());
	}
	~Sandbox() 
	{
		// Cleanup code can go here
	}
};


Teddy::Scope<Teddy::Application> Teddy::CreateApplication(Teddy::ApplicationCommandLineArgs args)
	
{
	ApplicationSpecification spec;

	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Tedditor";
	spec.CommandLineArgs = args;

	return CreateScope<Sandbox>(spec);
}