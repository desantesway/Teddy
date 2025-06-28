#pragma once

#include "teddyPch.h"

namespace Teddy {
	class TED_API Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
		std::string m_VertexSrc;
		std::string m_FragmentSrc;
		void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
		void CheckCompileErrors(uint32_t shader, const std::string& type);
	};
}