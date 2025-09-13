#pragma once

#include "Teddy/Renderer/TextureFormats.h"

namespace Teddy
{
	namespace Utils
	{
		unsigned int TeddyTextureFormatToGLDataFormat(TextureFormat format);
		unsigned int TeddyTextureFormatToGLInternalFormat(TextureFormat format);
		unsigned int TeddyTextureWrapFormatToGL(TextureWrapFormat format);
		unsigned int TeddyTextureFilterFormatToGL(TextureFilterFormat format);
	}
}