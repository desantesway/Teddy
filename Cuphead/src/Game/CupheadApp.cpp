#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "StartLayer.h"
#include "CupheadMainLayer.h"
#include "OverlayLayer.h"
#include "EndLayer.h"
#ifdef TED_DEBUG
	#include "../Editor/EditorLayer.h"
#endif

class Cuphead : public Teddy::Application
{
public:
	Cuphead(const Teddy::ApplicationSpecification& specification)
		: Teddy::Application(specification)
	{
		
#ifdef TED_DEBUG
		PushLayer(Teddy::CreateRef<Teddy::EditorLayer>());
#else
		PushLayer(Teddy::CreateRef<StartLayer>());
		PushLayer(Teddy::CreateRef<CupheadLayer>());
		PushLayer(Teddy::CreateRef<OverlayLayer>());
		PushLayer(Teddy::CreateRef<EndLayer>());
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