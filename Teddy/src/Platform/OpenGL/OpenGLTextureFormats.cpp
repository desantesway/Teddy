#include "Platform/OpenGL/OpenGLTextureFormats.h"

#include <glad/glad.h>

namespace Teddy
{
	namespace Utils
	{
		unsigned int TeddyTextureFormatToGLDataFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:  return GL_RGB;
			case TextureFormat::RGBA8: return GL_RGBA;
			}
			TED_CORE_ASSERT(false);

			return 0;
		}

		unsigned int TeddyTextureFormatToGLInternalFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:  return GL_RGB8;
			case TextureFormat::RGBA8: return GL_RGBA8;
			}

			TED_CORE_ASSERT(false);

			return 0;
		}

		unsigned int TeddyTextureWrapFormatToGL(TextureWrapFormat format)
		{
			switch (format)
			{
			case TextureWrapFormat::REPEAT:					return GL_REPEAT;
			case TextureWrapFormat::CLAMP_TO_EDGE:			return GL_CLAMP_TO_EDGE;
			case TextureWrapFormat::CLAMP_TO_BORDER:		return GL_CLAMP_TO_BORDER;
			case TextureWrapFormat::MIRRORED_REPEAT:		return GL_MIRRORED_REPEAT;
			case TextureWrapFormat::MIRRORED_CLAMP_TO_EDGE: return GL_MIRROR_CLAMP_TO_EDGE;
			}

			TED_CORE_ASSERT(false);
			return 0;
		}

		unsigned int TeddyTextureFilterFormatToGL(TextureFilterFormat format)
		{
			switch (format)
			{
			case TextureFilterFormat::NEAREST:					return GL_NEAREST;
			case TextureFilterFormat::LINEAR:					return GL_LINEAR;
			case TextureFilterFormat::NEAREST_MIPMAP_NEAREST:	return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilterFormat::LINEAR_MIPMAP_NEAREST:	return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilterFormat::NEAREST_MIPMAP_LINEAR:	return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilterFormat::LINEAR_MIPMAP_LINEAR:		return GL_LINEAR_MIPMAP_LINEAR;
			}

			TED_CORE_ASSERT(false);
			return 0;
		}
	}
}