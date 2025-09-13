#pragma once

#include <glad/glad.h>

namespace Teddy
{
	enum class TextureFilterFormat
	{
		None = 0,

		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};

	enum class TextureWrapFormat
	{
		None = 0,

		REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		MIRRORED_CLAMP_TO_EDGE
	};

	enum class TextureFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	namespace Utils
	{
		// TODO: API abstraction
		static GLenum TeddyTextureFormatToGLDataFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:  return GL_RGB;
			case TextureFormat::RGBA8: return GL_RGBA;
			}
			TED_CORE_ASSERT(false);

			return 0;
		}

		static GLenum TeddyTextureFormatToGLInternalFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:  return GL_RGB8;
			case TextureFormat::RGBA8: return GL_RGBA8;
			}

			TED_CORE_ASSERT(false);

			return 0;
		}

		static GLenum TeddyTextureWrapFormatToGL(TextureWrapFormat format)
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

		static GLenum TeddyTextureFilterFormatToGL(TextureFilterFormat format)
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