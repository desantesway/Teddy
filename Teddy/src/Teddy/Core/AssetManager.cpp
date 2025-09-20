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

	void AssetManager::OnUpdate()
	{
		m_FileWatcher.Watch(); // TODO: Comment this, this should be in the constructor and stop on the destructor
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

	template<>
	std::unordered_set<std::string> AssetManager::AssetsToReload<Shader>(bool changesHandled) { return m_FileWatcher.AssetsToReload(Utils::FileGroupType::Shader, changesHandled); }

	template<>
	std::unordered_set<std::string> AssetManager::AssetsToReload<Texture2D>(bool changesHandled) { return m_FileWatcher.AssetsToReload(Utils::FileGroupType::Texture2D, changesHandled); }

	template<>
	std::unordered_set<std::string> AssetManager::AssetsToReload<Font>(bool changesHandled) { return m_FileWatcher.AssetsToReload(Utils::FileGroupType::Font, changesHandled); }

	template<>
	bool& AssetManager::IsHotReloading<Shader>() { return m_FileWatcher.IsHotReloading(Utils::FileGroupType::Shader); }

	template<>
	bool& AssetManager::IsHotReloading<Font>() { return m_FileWatcher.IsHotReloading(Utils::FileGroupType::Font); }

	template<>
	bool& AssetManager::IsHotReloading<Texture2D>() { return m_FileWatcher.IsHotReloading(Utils::FileGroupType::Texture2D); }

	template<typename T>
	void AssetManager::RemoveExpired(const std::string& name, AssetGroup<T>& map)
	{
		if (auto it = map.Loaded.find(name); it != map.Loaded.end() && it->second.expired())
			map.Loaded.erase(it);
	}

	// Shaders
	template<>
	void AssetManager::RemoveExpired<Shader>(const std::string& name) 
	{ 
		if (m_Shaders.Loaded[name].expired())
		{
			m_Shaders.Loaded.erase(name);
		}
	}

	template<>
	Ref<Shader> AssetManager::Load<Shader>(const std::string& name)
	{
		return Load<Shader>(name, m_Shaders);
	}

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
			m_FileWatcher.Add(Utils::FileGroupType::Shader, filepath);
			auto forceBuild = m_FileWatcher.CheckOfflineChanges(Utils::FileGroupType::Shader, filepath);
			auto shader = Shader::Create(name, filepath, forceBuild);
			m_Shaders.Loaded[name] = shader;
			return shader;
		}
	}

	template<>
	Ref<Shader> AssetManager::Load<Shader>(const std::string& name, const std::string& filepath, bool hotReload)
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
			m_FileWatcher.Add(Utils::FileGroupType::Shader, filepath);
			auto shader = Shader::Create(name, filepath, hotReload);
			m_Shaders.Loaded[name] = shader;
			return shader;
		}
	}

	// Fonts
	template<>
	void AssetManager::RemoveExpired<Font>(const std::string& name) 
	{ 
		RemoveExpired<Font>(name, m_Fonts); 
	}

	template<>
	Ref<Font> AssetManager::Load<Font>(const std::string& name)
	{
		return Load<Font>(name, m_Fonts);
	}

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
		else if (name == "DefaultFont")
		{
			if (m_DefaultFont) return m_DefaultFont;
			auto font = CreateRef<Font>(filepath);
			m_DefaultFont = font;
			return font;
		}
		else
		{
			//auto forceBuild = m_FileWatcher.CheckOfflineChanges(Utils::FileGroupType::Font, filepath); //TODO: implement cache for fonts
			auto font = CreateRef<Font>(filepath);
			m_Fonts.Loaded[name] = font;
			return font;
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
	void AssetManager::RemoveExpired<Texture2D>(const std::string& name) { RemoveExpired<Texture2D>(name, m_Textures2D); }

	template<>
	Ref<Texture2D> AssetManager::Load<Texture2D>(const std::string& name)
	{
		return Load<Texture2D>(name, m_Textures2D);
	}

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
			auto texture = Texture2D::Create(filepath);
			m_Textures2D.Loaded[name] = texture;
			return texture;
		}
	}

	template<>
	Ref<Texture2D> AssetManager::Load<Texture2D>(const int& pixels, const TextureSpecification& spec)
	{
		//std::string name = "Texture2D_" + std::to_string(spec.Width) + "x" + std::to_string(spec.Height) + "x" + std::to_string(pixels)
		//	+ "_" + std::to_string((int)spec.Format) + "_" + std::to_string((int)spec.Filter) + "_" + std::to_string((int)spec.Wrap) 
		//	+ "-" + std::to_string(spec.GenerateMips);
		//
		//if (Exists<Texture2D>(name, m_Textures2D))
		//{
		//	Ref<Texture2D> texture = Get<Texture2D>(name, m_Textures2D);
		//
		//	return texture;
		//}
		//else
		//{
		auto texture = Texture2D::Create(spec);
		//m_Textures2D.Loaded[name] = texture;
		return texture;
		//}
	}

	// Deload

	// Shaders
	template<>
	void AssetManager::RemoveBypassDeloading<Shader>(const std::string& name)
	{
		RemoveBypassDeloading(name, m_Shaders);
	}

	template<>
	void AssetManager::BypassDeloading<Shader>(const std::string& name)
	{
		BypassDeloading(name, m_Shaders);
	}

	// Fonts
	template<>
	void AssetManager::RemoveBypassDeloading<Font>(const std::string& name)
	{
		RemoveBypassDeloading(name, m_Fonts);
	}

	template<>
	void AssetManager::BypassDeloading<Font>(const std::string& name)
	{
		BypassDeloading(name, m_Fonts);
	}

	// Texture2D
	template<>
	void AssetManager::RemoveBypassDeloading<Texture2D>(const std::string& name)
	{
		RemoveBypassDeloading(name, m_Textures2D);
	}

	template<>
	void AssetManager::BypassDeloading<Texture2D>(const std::string& name)
	{
		BypassDeloading(name, m_Textures2D);
	}

	//
	void AssetManager::RemoveExpiredAll()
	{
		AssetGroup<Shader> newShader;
		for (auto& [name, shader] : m_Shaders.Loaded)
		{
			if (!m_Shaders.Loaded[name].expired())
				newShader.Loaded[name] = m_Shaders.Loaded[name];
			else
			{
				m_FileWatcher.Remove(Utils::FileGroupType::Shader, shader.lock()->GetPath());
			}
		}
		m_Shaders = newShader;
		AssetGroup<Texture2D> newTexture2D;
		for (auto& [name, texture] : m_Textures2D.Loaded)
		{
			if (!m_Textures2D.Loaded[name].expired())
				newTexture2D.Loaded[name] = m_Textures2D.Loaded[name];
		}
		m_Textures2D = newTexture2D;
		AssetGroup<Font> newFont;
		for (auto& [name, font] : m_Fonts.Loaded)
		{
			if (!m_Fonts.Loaded[name].expired())
				newFont.Loaded[name] = m_Fonts.Loaded[name];
		}
		m_Fonts = newFont;
	}

	template<typename T>
	void AssetManager::RemoveBypassDeloading(const std::string& name, AssetGroup<T>& map)
	{ 
		TED_CORE_ASSERT(Exists<T>(name, map.DeloadBypass), "file not found!");
		map.DeloadBypass.erase(name);
	}

	void AssetManager::RemoveBypassAll()
	{
		if (m_Shaders.DeloadBypass.size() > 0) m_Shaders.DeloadBypass.clear();
		if (m_Textures2D.DeloadBypass.size() > 0) m_Textures2D.DeloadBypass.clear();
		if (m_Fonts.DeloadBypass.size() > 0) m_Fonts.DeloadBypass.clear();
	}

	template<typename T>
	void AssetManager::BypassDeloading(const std::string& name, AssetGroup<T>& map)
	{
		if(Exists<T>(name, map))
			map.DeloadBypass[name] = map.Loaded[name].lock();
	}

	void AssetManager::BypassAll()
	{
		for (auto& [name, shader] : m_Shaders.Loaded)
		{
			BypassDeloading<Shader>(name);
		}
		for (auto& [name, texture] : m_Textures2D.Loaded)
		{
			BypassDeloading<Texture2D>(name);
		}
		for (auto& [name, font] : m_Fonts.Loaded)
		{
			BypassDeloading<Font>(name);
		}
	}

	template<typename T>
	Ref<T> AssetManager::Get(const std::string& filepath, const AssetGroup<T>& map)
	{
		TED_CORE_ASSERT(Exists<T>(filepath, map), "file not found!");
		auto it = map.Loaded.find(filepath);
		TED_CORE_ASSERT(it != map.Loaded.end(), "file not found!");
		return it->second.lock();
	}

	template<typename T>
	bool AssetManager::Exists(const std::string& filepath, const AssetGroup<T>& map) const
	{
		bool alive = map.Loaded.find(filepath) != map.Loaded.end();
		if(alive)
			alive = !map.Loaded.at(filepath).expired();
		return alive;
	}

	template<typename T>
	bool AssetManager::Exists(const std::string& filepath, const std::unordered_map<std::string, Ref<T>>& map) const
	{
		return map.find(filepath) != map.end();
	}
}