#pragma once

#include "Teddy/Utils/FileWatcher.h"

#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/Font.h"

namespace Teddy
{
	enum class Boolean { False, True };

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		static AssetManager& Get() { return *s_Instance; }

		template<typename T>
		struct AssetGroup
		{
			std::unordered_map<std::string, Weak<T>> Loaded;
			std::unordered_map<std::string, Ref<T>> DeloadBypass; // if this map holds it, the item wont be deloaded (automatically by shared_ptr)
		};

		void OnUpdate();

		void SetHotReloading(bool hotReload) { m_HotReload = hotReload; }

		template<typename T>
		std::unordered_set<std::string> AssetsToReload(bool changesHandled);

		template<typename T>
		std::string AssetNeedsToReload(const std::string& filepath, bool changesHandled);

		template<typename T>
		bool& IsHotReloading();

		template<typename T>
		std::vector<Ref<T>> LoadMultiple(const std::vector<std::string>& filepaths);

		template<typename T>
		Ref<T> Load(const std::string& name);

		template<typename T>
		Ref<T> Load(const std::string& name, const std::string& filepath, bool hotReload);

		template<typename T>
		Ref<T> Load(const std::string& name, const std::string& filepath);

		template<typename T>
		Ref<T> Load(const int& pixels, const TextureSpecification& spec);

		// enum class to avoid bool template specialization issues
		template<typename T>
		Ref<T> Load(const std::string& filepath, Boolean genName, bool hotReload = false)
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

			if(hotReload)
				return Load<T>(name, filepath, hotReload);
			else
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

		template<typename T>
		void RemoveExpired(const std::string& filepath, Boolean genName)
		{
			std::string name = filepath;
			if (genName == Boolean::True)
			{
				auto lastSlash = filepath.find_last_of("/\\");
				auto lastDot = filepath.find_last_of('.');
				auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
				name = filepath.substr(lastSlash, count);
			}
			RemoveExpired<T>(name);
		}

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

		bool m_HotReload = false;
	private:
		static AssetManager* s_Instance;
	};
}