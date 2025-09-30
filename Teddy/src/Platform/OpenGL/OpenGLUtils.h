#pragma once

#include <glad/glad.h>

namespace Teddy::OpenGLUtils
{
	uint32_t GetMaxImageUnits()
	{
		GLint maxFragmentUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragmentUnits);

		return (uint32_t)maxFragmentUnits;
	}
}