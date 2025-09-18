#include "Teddy/Renderer/Shader.h"

#include "Teddy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Teddy 
{

	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath, const bool& forceBuild)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, filepath, forceBuild);
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath, const bool& forceBuild)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath, forceBuild);
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    TED_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		TED_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}