#include "TeddyPch.h"
#include "Teddy/Scene/Components.h"
#include "Teddy/Renderer/MSDFData.h"
#include <box2d/box2d.h>

#include <glm/glm.hpp>

namespace Teddy
{
    void Rigidbody2DComponent::SetPosition(TransformComponent& transform)
    {
        if (RuntimeBody)
        {
		    b2BodyId bodyId = *static_cast<b2BodyId*>(RuntimeBody);
		    b2Body_SetTransform(bodyId, { transform.Translation.x, transform.Translation.y }, b2MakeRot(transform.Rotation.z));
        }
    }

    uint64_t CollisionFilter2DComponent::GetFilterCategory(BoxCollider2DComponent& collider)
    {
        return b2Shape_GetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture)).categoryBits;
    }

    uint64_t CollisionFilter2DComponent::GetFilterMask(BoxCollider2DComponent& collider)
    {
        return b2Shape_GetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture)).maskBits;
    }

    uint64_t CollisionFilter2DComponent::GetFilterGroupIndex(BoxCollider2DComponent& collider)
    {
        return b2Shape_GetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture)).groupIndex;
    }

    void CollisionFilter2DComponent::SetFilterCategory(BoxCollider2DComponent& collider, uint64_t filter)
    {
        if (collider.RuntimeFixture)
        {
            b2Filter f = b2Shape_GetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture));
            f.categoryBits = filter;
            b2Shape_SetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture), f);
		}
    }

    void CollisionFilter2DComponent::SetFilterMask(BoxCollider2DComponent& collider, uint64_t filter)
    {
        if (collider.RuntimeFixture)
        {
            b2Filter f = b2Shape_GetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture));
            f.maskBits = filter;
            b2Shape_SetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture), f);
        }
    }

    void CollisionFilter2DComponent::SetFilterGroupIndex(BoxCollider2DComponent& collider, uint64_t filter)
    {
        if (collider.RuntimeFixture)
        {
            b2Filter f = b2Shape_GetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture));
            f.groupIndex = filter;
            b2Shape_SetFilter(*static_cast<b2ShapeId*>(collider.RuntimeFixture), f);
        }
    }

    float Rigidbody2DComponent::GetGravityScale()
    {
        return b2Body_GetGravityScale(*static_cast<b2BodyId*>(RuntimeBody));
    }

    void Rigidbody2DComponent::SetGravityScale(float scale)
    {
        b2Body_SetGravityScale(*static_cast<b2BodyId*>(RuntimeBody), scale);
    }

    glm::vec2 Rigidbody2DComponent::GetVelocity()
    {
        b2Vec2 vec = b2Body_GetLinearVelocity(*static_cast<b2BodyId*>(RuntimeBody));

		return glm::vec2(vec.x, vec.y);
    }

    void Rigidbody2DComponent::SetVelocity(float velX, float velY)
    {
        b2Body_SetLinearVelocity(*static_cast<b2BodyId*>(RuntimeBody), { velX, velY });
    }

    void Rigidbody2DComponent::SetVelocityY(float velY)
    {
        SetVelocity(GetVelocity().x, velY);
    }

    void Rigidbody2DComponent::SetVelocityX(float velX)
    {
        SetVelocity(velX, GetVelocity().y);
    }

    void Rigidbody2DComponent::ApplyForce(float forceX, float forceY, bool wake)
    {
        if (RuntimeBody)
        {
            b2Body_ApplyForceToCenter(*static_cast<b2BodyId*>(RuntimeBody), { forceX, forceY }, wake);
        }
    }

    void SpriteAnimationAtlasComponent::GenerateFrames(SpriteAnimationComponent& animation, SpriteAtlasComponent& atlas)
    {
		int index = 0;
		bool add = animation.PlayableIndicies.size() == 0;
        for (int i = 0; i < animation.Textures.size(); i++)
        {
            int maxX = atlas.SpriteWidth == 0 ? 1 : (animation.Textures[i]->GetWidth() / atlas.SpriteWidth);
            int maxY = atlas.SpriteHeight == 0 ? 1 : (animation.Textures[i]->GetHeight() / atlas.SpriteHeight);
            int x = 0;
			int y = 0;
        
            while (x < maxX && y < maxY)
            {
                this->AnimationSprites[index] = AnimationSprite{ x, y, i };
				if (add) animation.PlayableIndicies.push_back(index);
                x++;
                if(x > maxX - 1)
                {
                    x = 0;
                    y++;
				}
        
                index++;
            }
        }
        
    }

	void TextComponent::CalculateTextQuad()
	{
        TED_PROFILE_CATEGORY("Text Quad", InstrumentorCategory::Visibility);

        if (!FontAsset || !FontAsset->GetMSDFData())
			return;
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