#include "Teddy/Core/AssetManager.h"

namespace Teddy
{
	AssetManager* AssetManager::s_Instance = nullptr;

	Utils::FileWatcher AssetManager::m_FileWatcher = Utils::FileWatcher("assets/tmp/filewatcher.tedwatch");

	AssetManager::AssetManager()
	{
		TED_CORE_ASSERT(!s_Instance, "Asset Manager already exists!");
		s_Instance = this;
	}

	template<typename T>
	Ref<T> AssetManager::Load(const std::string& name, const AssetGroup<T>& map)
	{
		if (Exists<T>(name, map))
		{
			return Get<T>(name, map);
		}
		else
		{
			TED_CORE_ASSERT(false, "Asset not found with that name!");
			return nullptr;
		}
	}

	template<typename T>
	Ref<T> AssetManager::Load(const std::string& name)
	{
		return Load<T>(name, m_Shaders);
	}

	// Shaders
	template<>
	Ref<Shader> AssetManager::Load<Shader>(const std::string& name, const std::string& filepath)
	{
		if (Exists<Shader>(name, m_Shaders))
		{
			Ref<Shader> shader = Get<Shader>(name, m_Shaders);
			if (shader->GetPath() != filepath)
			{
				TED_CORE_ASSERT(false, "Shader already exists with that name but different filepath");
			}
			
			return shader;
		}
		else
		{
			auto forceBuild = m_FileWatcher.CheckOfflineChanges(Utils::FileGroupType::Shader, filepath);
			return m_Shaders.LoadedAssets[name] = Shader::Create(name, filepath, forceBuild);
		}
	}

	// Fonts
	template<>
	Ref<Font> AssetManager::Load<Font>(const std::string& name, const std::string& filepath)
	{

		if (Exists<Font>(name, m_Fonts))
		{
			Ref<Font> font = Get<Font>(name, m_Fonts);
			if (font->GetPath() != filepath)
			{
				TED_CORE_ASSERT(false, "Font already exists with that name but different filepath");
			}

			return font;
		}
		else
		{
			//auto forceBuild = m_FileWatcher.CheckOfflineChanges(Utils::FileGroupType::Font, filepath); //TODO: implement cache for fonts
			return m_Fonts.LoadedAssets[name] = CreateRef<Font>(filepath);
		}
	}

	template<>
	Ref<Font> AssetManager::Load<Font>()
	{
		std::string name = "DefaultFont";
		std::string filepath = Font::GetDefaultPath();

		return Load<Font>(name, filepath);
	}

	// Texture2D
	template<>
	Ref<Texture2D> AssetManager::Load<Texture2D>(const std::string& name, const std::string& filepath)
	{
		if (Exists<Texture2D>(name, m_Textures2D))
		{
			Ref<Texture2D> texture = Get<Texture2D>(name, m_Textures2D);
			if (texture->GetPath() != filepath)
			{
				TED_CORE_ASSERT(false, "Texture2D already exists with that name but different filepath");
			}

			return texture;
		}
		else
		{
			return m_Textures2D.LoadedAssets[name] = Texture2D::Create(filepath);
		}
	}

	template<>
	Ref<Texture2D> AssetManager::Load<Texture2D>(const TextureSpecification& spec)
	{
		std::string name = "Texture2D_" + std::to_string(spec.Width) + "x" + std::to_string(spec.Height) 
			+ "_" + std::to_string((int)spec.Format) + "_" + std::to_string((int)spec.Filter) + "_" + std::to_string((int)spec.Wrap) 
			+ "-" + std::to_string(spec.GenerateMips);

		if (Exists<Texture2D>(name, m_Textures2D))
		{
			Ref<Texture2D> texture = Get<Texture2D>(name, m_Textures2D);

			return texture;
		}
		else
		{
			return m_Textures2D.LoadedAssets[name] = Texture2D::Create(spec);
		}
	}

	template<typename T>
	Ref<T> AssetManager::Get(const std::string& filepath, const AssetGroup<T>& map)
	{
		TED_CORE_ASSERT(Exists<T>(filepath, map), "file not found!");
		auto it = map.LoadedAssets.find(filepath);
		TED_CORE_ASSERT(it != map.LoadedAssets.end(), "file not found!");
		return it->second;
	}

	template<typename T>
	bool AssetManager::Exists(const std::string& filepath, const AssetGroup<T>& map) const
	{
		return map.LoadedAssets.find(filepath) != map.LoadedAssets.end();
	}
}