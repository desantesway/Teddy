
#include <Teddy.h>

// --- Entry Point --------------------------
#include "Teddy/Core/EntryPoint.h"
// ------------------------------------------

#include "KeyStar2D.h"

class KeyStar : public Teddy::Application 
{
public:
	KeyStar() {
		//PushLayer(new ExampleLayer());
		PushLayer(new KeyStar2D());
	}
	~KeyStar() {
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication() 
{
	return new KeyStar();
} // CreateApplication