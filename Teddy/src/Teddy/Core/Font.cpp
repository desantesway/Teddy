#include "TeddyPch.h"
#include "Teddy/Core/Font.h"

#define MSDFGEN_USE_LIBPNG

#include <msdf-atlas-gen.h>

namespace Teddy
{
	Font::Font(const std::filesystem::path& filepath)
	{
        if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) 
        {
			std::string fileString = filepath.string();
            if (msdfgen::FontHandle* font = loadFont(ft, fileString.c_str())) 
            {
                msdfgen::Shape shape;
                if (msdfgen::loadGlyph(shape, font, 'A', msdfgen::FONT_SCALING_EM_NORMALIZED))
                {
                    shape.normalize();
                    //                      max. angle
                    msdfgen::edgeColoringSimple(shape, 3.0);
                    //          output width, height
                    msdfgen::Bitmap<float, 3> msdf(32, 32);
                    //                            scale, translation (in em's)
                    msdfgen::SDFTransformation t(msdfgen::Projection(32.0, msdfgen::Vector2(0.125, 0.125)), msdfgen::Range(0.125));
                    msdfgen::generateMSDF(msdf, shape, t);
                }
                msdfgen::destroyFont(font);
            }
            msdfgen::deinitializeFreetype(ft);
        }
	}

}