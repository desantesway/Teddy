#include "TeddyPch.h"
#include "Teddy/Core/Window.h"

#include "Teddy/Renderer/SDLWindow.h"

namespace Teddy
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<SDLWindow>(props);
		TED_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	}

}