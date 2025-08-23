#include "TeddyPch.h"
#include "Teddy/Renderer/Font.h"
#include "Teddy/Renderer/MSDFData.h"

#include <msdf-atlas-gen.h>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>

namespace Teddy
{

    Font::~Font() 
    {
        delete m_MSDFData;
    }

    template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
    static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
        const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
    {
        msdf_atlas::GeneratorAttributes attributes;
        attributes.config.overlapSupport = true;
        attributes.scanlinePass = true;

        msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
        generator.setAttributes(attributes);
		generator.setThreadCount(8); // TODO: Change this to half of available cpu threads
        generator.generate(glyphs.data(), (int)glyphs.size());
        msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

        TextureSpecification spec;
        spec.Width = bitmap.width;
        spec.Height = bitmap.height;
        spec.Format = ImageFormat::RGB8;
        spec.GenerateMips = false;

        Ref<Texture2D> texture = Texture2D::Create(spec);
        texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);
        return texture;
    }

	Font::Font(const std::filesystem::path& filepath)
		: m_MSDFData(new MSDFData()), m_Path(filepath.string())
	{
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();

		TED_CORE_ASSERT(ft, "Failed to initialize FreeType");

		std::string fileString = filepath.string();

        // TODO Change to: loadFontData (it reads from a mem buffer)
        if (msdfgen::FontHandle* font = loadFont(ft, fileString.c_str())) 
        {

            struct CharsetRange
            {
                uint32_t Start, End;
            };

			// from imgui_draw.cpp
            static const CharsetRange ranges[] =
            {
                { 0x0020, 0x00FF }
            };

			msdf_atlas::Charset charset;
            for(int i = 0; i < 2; i+=2)
            {
               for (uint32_t codepoint = ranges->Start; codepoint < ranges->End; codepoint++)
               {
                   charset.add(codepoint);
			   }
            }

			m_MSDFData->FontGeometry = msdf_atlas::FontGeometry(&m_MSDFData->Glyphs);
			int glyphsLoaded = m_MSDFData->FontGeometry.loadCharset(font, 1.0, charset);
            TED_CORE_INFO("Loaded {} glyphs out of {}", glyphsLoaded, charset.size());

            msdf_atlas::TightAtlasPacker atlasPacker;

            atlasPacker.setPixelRange(2.0);
            atlasPacker.setMiterLimit(1.0);
            atlasPacker.setSpacing(0);
            atlasPacker.setScale(40.0);
            int remaining = atlasPacker.pack(m_MSDFData->Glyphs.data(), (int)m_MSDFData->Glyphs.size());

            TED_CORE_ASSERT(remaining == 0);

            int width, height;
            atlasPacker.getDimensions(width, height);

            #define DEFAULT_ANGLE_THRESHOLD 3.0
            #define LCG_MULTIPLIER 6364136223846793005ull
            #define LCG_INCREMENT 1442695040888963407ull
            #define THREAD_COUNT 8

            uint64_t coloringSeed = 0;
            bool expensiveColoring = false;

            if (expensiveColoring)
            {
                msdf_atlas::Workload([&glyphs = m_MSDFData->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
                    unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
                    glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                    return true;
                    }, m_MSDFData->Glyphs.size()).finish(THREAD_COUNT);
            }
            else {
                unsigned long long glyphSeed = coloringSeed;
                for (msdf_atlas::GlyphGeometry& glyph : m_MSDFData->Glyphs)
                {
                    glyphSeed *= LCG_MULTIPLIER;
                    glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                }
            }

            m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", 
                (float)atlasPacker.getScale(), m_MSDFData->Glyphs, m_MSDFData->FontGeometry, width, height);

            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
	}

    Ref<Font> Font::GetDefault()
    {
        static Ref<Font> DefaultFont;
        if (!DefaultFont)
            DefaultFont = CreateRef<Font>("../Teddy/assets/fonts/instrument-sans/ttf/InstrumentSans-Bold.ttf");

        return DefaultFont;
    }

}