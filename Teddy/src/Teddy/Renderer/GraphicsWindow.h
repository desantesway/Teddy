#pragma once

namespace Teddy 
{

	class GraphicsWindow
	{
	public:
		virtual ~GraphicsWindow() = default;

		virtual void* GetWindow() = 0;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void VSync(bool enabled) = 0;

		static Scope<GraphicsWindow> Create(const char* title, int width, int height);
	};

}