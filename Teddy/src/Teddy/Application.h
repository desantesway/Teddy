#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Teddy{

	class TED_API Application
	{
	public:
			Application();
			virtual ~Application();

			void Run();
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}