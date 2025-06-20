#pragma once

#include "Core.h"

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