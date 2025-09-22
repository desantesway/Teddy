
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "Cuphead2D.h"

class Cuphead : public Teddy::Application
{
public:
	Cuphead(const Teddy::ApplicationSpecification& specification)
		: Teddy::Application(specification)
	{
		PushLayer(new Cuphead2D());
	}
	~Cuphead()
	{
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication(Teddy::ApplicationCommandLineArgs args)
	
{
	ApplicationSpecification spec;

	spec.Name = "Cuphead Remake";
	spec.WorkingDirectory = "";
	spec.CommandLineArgs = args;

	return new Cuphead(spec);
}