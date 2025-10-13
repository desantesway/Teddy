#pragma once

#include "Teddy/Core/ButtonInteraction.h"
#include "Teddy/Scene/SceneCamera.h"
#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/Font.h"
#include "Teddy/Core/AssetManager.h"

namespace Teddy
{

	class ScriptableEntity;

	struct IDComponent
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(const UUID& id) : ID(id) {}
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation =  glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 })
								* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
								* glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteAtlasComponent 
	{
		int X = 0;
		int Y = 0;
		int SpriteWidth = 0;
		int SpriteHeight = 0;

		SpriteAtlasComponent() = default;
		SpriteAtlasComponent(const SpriteAtlasComponent&) = default;
		SpriteAtlasComponent(const float& xI, const float& yI, const float& width, const float& height)
			: X(xI), Y(yI), SpriteWidth(width), SpriteHeight(height) 
		{
		}
	};

	enum class BlendingMode { None = 0, Multiply = 1, Invert = 2, InvertAlpha = 3 };

	struct AnimationSprite
	{
		int X = 0;
		int Y = 0;
		int TextureIndex = 0;
	};

	struct SpriteAnimationComponent
	{
		glm::vec4 Color{ 1.0f };
		std::vector<Ref<Texture2D>> Textures;
		float TilingFactor = 1.0f;
		bool IsBackground = false;
		bool OriginalAspectRatio = true;

		int TextureIndex = 0;
		std::vector<int> PlayableIndicies; // for putting multiple animations in one component (up, left, right, etc)
		float Timer = 0.0f;
		float FrameTime = 0.1f;
		float InitialFrameTime = FrameTime;
		float FinalFrameTime = FrameTime;
		bool Loop = true;
		bool PingPong = false;
		bool Pause = false;
		bool Reverse = false;

		BlendingMode BlendMode = BlendingMode::None;

		SpriteAnimationComponent() = default;
		SpriteAnimationComponent(const SpriteAnimationComponent&) = default;
		SpriteAnimationComponent(float initFrame, float frame, float finalFrame)
			: InitialFrameTime(initFrame), FrameTime(frame), FinalFrameTime(finalFrame)
		{
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
		bool IsBackground = false;
		bool OriginalAspectRatio = true;

		BlendingMode BlendMode = BlendingMode::None;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {
		}
		SpriteRendererComponent(SpriteAnimationComponent& animation)
			: Color(animation.Color), Texture(animation.Textures[animation.TextureIndex]), 
			TilingFactor(animation.TilingFactor), OriginalAspectRatio(animation.OriginalAspectRatio), 
			IsBackground(animation.IsBackground), BlendMode(animation.BlendMode)
		{ }
	};

	struct SpriteAnimationAtlasComponent // TODO: OnRemove
	{
		std::unordered_map<int, AnimationSprite> AnimationSprites;
		int Index = 0;

		void GenerateFrames(SpriteAnimationComponent& animation, SpriteAtlasComponent& atlas);

		SpriteAnimationAtlasComponent() = default;
		SpriteAnimationAtlasComponent(const SpriteAnimationAtlasComponent&) = default;
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera; 
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)() = nullptr;
		void (*DestroyScript)(NativeScriptComponent*) = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	
	};

	// Physics
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Kinematic, Dynamic };
		BodyType Type = BodyType::Static;

		float GravityScale = 1.0f;
		bool FixedRotation = false;

		void* RuntimeBody = nullptr;

		void ApplyForce(float forceX, float forceY, bool wake);

		float GetGravityScale();
		void SetGravityScale(float scale);

		glm::vec2 GetVelocity();
		void SetVelocity(float velX, float velY);
		void SetVelocityY(float velY);
		void SetVelocityX(float velX);

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO: move into physics material in the future (maybe)
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;

		bool EnableContactEvents = false;
		bool EnableSensorEvents = false;

		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		// TODO: move into physics material in the future (maybe)
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;

		bool EnableContactEvents = false;
		bool EnableSensorEvents = false;

		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	struct Sensor2DComponent // TODO: Serialization
	{
		struct SensorData
		{
			glm::vec2 Offset = { 0.0f, 0.0f };
			glm::vec2 Size = { 0.5f, 0.5f };
			float Rotation = 0.0f;

			void* RuntimeFixture = nullptr;

			SensorData() = default;
			SensorData(const SensorData&) = default;
			SensorData(const glm::vec2& offset, const glm::vec2& size, float rotation)
				: Offset(offset), Size(size), Rotation(rotation) {}
		};
		
		std::unordered_map<std::string, SensorData> Sensors;

		Sensor2DComponent() = default;
		Sensor2DComponent(const Sensor2DComponent&) = default;
	};

	struct TextComponent
	{
		std::string TextString;
		Ref<Font> FontAsset = AssetManager::Get().Load<Font>();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;

		glm::vec4 BackgroundColor{ 0.0f };

		glm::vec4 OutlineColor{ 0.0f };
		float OutlineThickness = 0.0f;

		TransformComponent TextQuad;

		enum class AlignmentType { None = 0, Center, TopLeft, TopRight, BottomLeft, BottomRight, LeftCenter, RightCenter };
		AlignmentType TextAlignment = AlignmentType::None;

		TextComponent() = default;

		TextComponent(const TextComponent&) = default;
		void CalculateTextQuad();

		void SetString(std::string string) { TextString = string; CalculateTextQuad();}
	};

	struct ButtonComponent
	{
		bool Hovered = false;

		void SetHovered(bool isHovered) { Hovered = isHovered; }
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent, SpriteAtlasComponent, 
		SpriteAnimationComponent,CircleRendererComponent, CameraComponent, NativeScriptComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent, Sensor2DComponent,
		TextComponent, ButtonComponent, SpriteAnimationAtlasComponent>;
}