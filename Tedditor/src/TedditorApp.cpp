
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "EditorLayer.h"

namespace Teddy
{
	class Tedditor : public Application
	{
	public:
		Tedditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		~Tedditor() 
		{

		}
	};


	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Tedditor";
		spec.CommandLineArgs = args;

		return new Tedditor(spec);
	}
}
