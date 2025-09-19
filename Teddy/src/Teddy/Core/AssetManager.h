#pragma once

#include <Teddy/Core/Base.h>
#include "Teddy/Utils/FileWatcher.h"

#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/Font.h"

namespace Teddy
{
	enum class Boolean { False, True };

	// TODO: Being used counter, when 0 deload
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		static AssetManager& Get() { return *s_Instance; }

		static void Init();
		static void Shutdown();

		template<typename T>
		Ref<T> Load(const std::string& name);

		template<typename T>
		Ref<T> Load(const std::string& name, const std::string& filepath);

		// enum class to avoid bool template specialization issues
		template<typename T>
		Ref<T> Load(const std::string& filepath, Boolean genName)
		{
			std::string name = filepath;

			std::filesystem::path fontPath = std::filesystem::path(Font::GetDefaultPath());
			if (fontPath.string() == filepath)
			{
				name = "DefaultFont";
			}
			if (genName == Boolean::True)
			{
				auto lastSlash = filepath.find_last_of("/\\");
				lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
				auto lastDot = filepath.rfind('.');
				auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
				name = filepath.substr(lastSlash, count);
			}

			return Load<T>(name, filepath);
		}

		template<typename T>
		Ref<T> Load();

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