#include "teddyPch.h"
#include "Teddy/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Teddy 
{

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}