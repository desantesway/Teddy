
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "StartLayer.h"
#include "CupheadMainLayer.h"
#include "OverlayLayer.h"
#ifdef TED_DEBUG
#include "../Editor/EditorLayer.h"
#endif

class Cuphead : public Teddy::Application
{
public:
	Cuphead(const Teddy::ApplicationSpecification& specification)
		: Teddy::Application(specification)
	{
		PushLayer(new StartLayer());

#ifdef TED_DEBUG
		PushLayer(new Teddy::EditorLayer());
#else
		PushLayer(new CupheadLayer());
#endif

		PushLayer(new OverlayLayer());
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