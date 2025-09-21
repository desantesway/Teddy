#pragma once

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
	
}