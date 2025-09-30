#pragma once	

#include "TeddyPch.h"
#include "Teddy/Renderer/SDLWindow.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Core/CodeConverter.h"

namespace Teddy 
{
	// TODO: stop doing window watch and change renderer to a second thread
	bool SDLWindow::EventWatcher(void* data, SDL_Event* event) 
	{
		TED_PROFILE_CAT(InstrumentorCategory::Input);

		static bool pendingExpose = false;

		//if (event->type == SDL_EVENT_WINDOW_RESIZED ) 
		//{
		//	SDLWindow* window = static_cast<SDLWindow*>(data);
		//	WindowResizeEvent e(event->window.data1, event->window.data2);
		//	window->m_Data.EventCallback(e);
		//}
		//else if (event->type == SDL_EVENT_WINDOW_MOVED)
		//{
		//	SDLWindow* window = static_cast<SDLWindow*>(data);
		//	WindowMovedEvent e;
		//	window->m_Data.EventCallback(e);
		//}

		return true;
	}

	// TODO: Window minimize
	void SDLWindow::SDLEvents() 
	{
		TED_PROFILE_CAT(InstrumentorCategory::Input);

		TED_PROFILE_FUNCTION();

        SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
				case SDL_EVENT_QUIT: 
				{
					WindowCloseEvent e;
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_WINDOW_RESIZED:
				{
					WindowResizeEvent e(event.window.data1, event.window.data2);
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{
					KeyPressedEvent e(SDLToTeddyKey(event.key.key), event.key.repeat);
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_KEY_UP:
				{
					KeyReleasedEvent e(SDLToTeddyKey(event.key.key));
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					MouseCode button = SDLToTeddyMouse(event.button.button);
					TED_CORE_ASSERT(static_cast<int>(button) != -1, "Unknown mouse button pressed");
					MouseButtonPressedEvent e(button);
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					MouseCode button = SDLToTeddyMouse(event.button.button);
					TED_CORE_ASSERT(static_cast<int>(button) != -1, "Unknown mouse button pressed");
					MouseButtonReleasedEvent e(button);
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
					MouseMovedEvent e(event.motion.x, event.motion.y);
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_MOUSE_WHEEL:
				{
					MouseScrolledEvent e(event.wheel.x, event.wheel.y);
					m_Data.EventCallback(e);
					break;
				}
				case SDL_EVENT_TEXT_INPUT: 
				{
					KeyTypedEvent e(event.text.text);
					m_Data.EventCallback(e);
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
}
