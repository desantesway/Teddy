#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Renderer/Texture.h"

namespace Teddy
{
	struct MSDFData;

	// TODO: Implement name
	class Font
	{
	public:
		Font() = default;
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_MSDFData; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static const std::string GetDefaultPath();
		static Ref<Font> GetDefault();

		std::string& GetPath() { return m_Path; }

	private:
		MSDFData* m_MSDFData;
		Ref<Texture2D> m_AtlasTexture;

		std::string m_Path;
		inline static const std::string m_DefaultPath = "../Teddy/assets/fonts/instrument-sans/ttf/InstrumentSans-Bold.ttf";
	};

}