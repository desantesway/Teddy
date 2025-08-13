
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
		Tedditor(ApplicationCommandLineArgs args)
			: Application("Tedditor", args) // Initialize the base Application with a name
		{
			PushLayer(new EditorLayer());
		}

		~Tedditor() 
		{

		}
	};


	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Tedditor(args);
	}
}
