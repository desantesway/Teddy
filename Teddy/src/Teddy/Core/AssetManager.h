#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Utils/FileWatcher.h"

#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/Font.h"

namespace Teddy
{
	enum class Boolean { False, True };

	// TODO: if new scene is being loaded only deload after loaded
	// TODO: Implement hot reloading of assets
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		static AssetManager& Get() { return *s_Instance; }

		template<typename T>
		struct AssetGroup
		{
			std::unordered_map<std::string, Weak<T>> Loaded;
			std::unordered_map<std::string, Ref<T>> DeloadBypass; // if this map holds it, the item wont be deloaded (automatically by shared_ptr)
		};

		template<typename T>
		std::unordered_set<std::string> AssetsToReload(bool changesHandled);

		template<typename T>
		bool& IsHotReloading();

		template<typename T>
		Ref<T> Load(const std::string& name);

		template<typename T>
		Ref<T> Load(const std::string& name, const std::string& filepath);

		template<typename T>
		Ref<T> Load(const int& pixels, const TextureSpecification& spec);

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
		void RemoveExpired(const std::string& name, AssetGroup<T>& map);

		template<typename T>
		void RemoveExpired(const std::string& name);

		void RemoveExpiredAll();

		void RemoveBypassAll();

		template<typename T>
		void RemoveBypassDeloading(const std::string& name);

		template<typename T>
		void RemoveBypassDeloading(const std::string& name, AssetGroup<T>& map);

		void BypassAll();
		
		template<typename T>
		void BypassDeloading(const std::string& name);

		template<typename T>
		void BypassDeloading(const std::string& name, AssetGroup<T>& map);

		template<typename T>
		Ref<T> Get(const std::string& filepath, const AssetGroup<T>& map);

		template<typename T>
		bool Exists(const std::string& filepath, const AssetGroup<T>& map) const;

		template<typename T>
		bool Exists(const std::string& filepath, const std::unordered_map<std::string, Ref<T>>& map) const;
	private:
		template<typename T>
		Ref<T> Load(const std::string& name, const AssetGroup<T>& map);

	private:
		static Utils::FileWatcher m_FileWatcher;

		AssetGroup<Shader> m_Shaders;
		AssetGroup<Texture2D> m_Textures2D;
		AssetGroup<Font> m_Fonts;
		Ref<Font> m_DefaultFont = nullptr;

	private:
		static AssetManager* s_Instance;
	};
}