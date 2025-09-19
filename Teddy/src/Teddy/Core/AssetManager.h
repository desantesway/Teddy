#pragma once

#include <Teddy/Core/Base.h>
#include "Teddy/Utils/FileWatcher.h"

#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/Font.h"

namespace Teddy
{

	enum class FileGroupType
	{
		None = 0,
		Shader,
		Texture2D,
		Font
	};

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		static AssetManager& Get() { return *s_Instance; }

		static void Init();
		static void Shutdown();

		// Should be added before loading, because of filewatch
		template<typename T>
		void Add(const std::string& name, const std::string& path);

		template<typename T>
		Ref<T> Load(const std::string& name);

		template<typename T>
		Ref<T> Load(const std::string& name, const std::string& filepath);

		//Ref<Shader> LoadShader(const std::string& name, const std::string& filepath);
		//Ref<Texture2D> LoadTexture2D(const std::string& filepath);

		// Deload

		template<typename T>
		Ref<T> Get(const std::string& filepath, const std::unordered_map<std::string, Ref<T>>& map);

		template<typename T>
		bool Exists(const std::string& filepath, const std::unordered_map<std::string, Ref<T>>& map) const;
	private:
		template<typename T>
		Ref<T> Load(const std::string& name, const std::unordered_map<std::string, Ref<T>>& map);

	private:
		static Utils::FileWatcher m_FileWatcher;

		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures2D;
		std::unordered_map<std::string, Ref<Font>> m_Fonts;


	private:
		static AssetManager* s_Instance;
	};
}