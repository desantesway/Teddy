
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "Sandbox2D.h"

class Sandbox : public Teddy::Application 
{
public:
	Sandbox(Teddy::ApplicationCommandLineArgs args)
		: Application("Sandbox", args)
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
	return new Sandbox(args);
}