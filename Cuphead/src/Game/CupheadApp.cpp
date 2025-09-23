
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "CupheadLayer.h"
#include "../Editor/EditorLayer.h"

class Cuphead : public Teddy::Application
{
public:
	Cuphead(const Teddy::ApplicationSpecification& specification)
		: Teddy::Application(specification)
	{
#ifdef TED_DEBUG
		PushLayer(new Teddy::EditorLayer());
#else
		PushLayer(new CupheadLayer());
#endif
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