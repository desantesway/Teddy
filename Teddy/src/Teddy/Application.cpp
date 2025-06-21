#include "teddy_pch.h"
#include "Application.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Log.h"

namespace Teddy {

	Application::Application(){
	
	}
	Application::~Application(){
	
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			TED_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			TED_TRACE(e);
		}
		while (true);
	}
}