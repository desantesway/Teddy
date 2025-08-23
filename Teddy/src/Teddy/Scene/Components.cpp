#include "teddyPch.h"
#include "Teddy/Scene/Components.h"
#include "Teddy/Renderer/MSDFData.h"

namespace Teddy
{
	void TextComponent::CalculateTextQuad()
	{
        const auto& fontGeometry = FontAsset->GetMSDFData()->FontGeometry;
        const auto& metrics = fontGeometry.getMetrics();

        double x = 0.0;
        double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
        double y = 0.0;
        const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

        // Initialize bounding box
        glm::vec2 minBound(std::numeric_limits<float>::max());
        glm::vec2 maxBound(std::numeric_limits<float>::lowest());

        for (size_t i = 0; i < TextString.size(); i++)
        {
            char character = TextString[i];

            if (character == '\r')
                continue;

            if (character == '\n')
            {
                x = 0;
                y -= fsScale * metrics.lineHeight + LineSpacing;
                continue;
            }

            if (character == ' ')
            {
                float advance = spaceGlyphAdvance;
                if (i < TextString.size() - 1)
                {
                    char nextCharacter = TextString[i + 1];
                    double dAdvance;
                    fontGeometry.getAdvance(dAdvance, character, nextCharacter);
                    advance = (float)dAdvance;
                }

                x += fsScale * advance + Kerning;
                continue;
            }

            if (character == '\t')
            {
                x += 4.0f * (fsScale * spaceGlyphAdvance + Kerning);
                continue;
            }

            auto glyph = fontGeometry.getGlyph(character);

            if (!glyph)
                glyph = fontGeometry.getGlyph('?');
            if (!glyph)
                return;

            double pl, pb, pr, pt;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);
            glm::vec2 quadMin((float)pl, (float)pb);
            glm::vec2 quadMax((float)pr, (float)pt);

            quadMin *= fsScale, quadMax *= fsScale;
            quadMin += glm::vec2(x, y);
            quadMax += glm::vec2(x, y);

            minBound = glm::min(minBound, quadMin);
            maxBound = glm::max(maxBound, quadMax);

            if (i < TextString.size() - 1)
            {
                double advance = glyph->getAdvance();
                char nextCharacter = TextString[i + 1];
                fontGeometry.getAdvance(advance, character, nextCharacter);

                x += fsScale * advance + Kerning;
            }
        }

        glm::vec2 textSize = maxBound - minBound;
        glm::vec2 center = (minBound + maxBound) * 0.5f;

        TextQuad.Translation = glm::vec3(center, 0.0f);
        TextQuad.Scale = glm::vec3(textSize, 1.0f);
	}
}