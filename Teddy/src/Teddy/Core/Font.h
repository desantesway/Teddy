#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Renderer/Texture.h"

namespace Teddy
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
	private:
		MSDFData* m_MSDFData;
		Ref<Texture2D> m_AtlasTexture;
	};

}