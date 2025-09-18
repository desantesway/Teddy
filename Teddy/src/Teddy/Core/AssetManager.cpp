#include "Teddy/Core/AssetManager.h"

namespace Teddy
{
	AssetManager* AssetManager::s_Instance = nullptr;

	AssetManager::AssetManager()
	{
		TED_CORE_ASSERT(!s_Instance, "Asset Manager already exists!");
		s_Instance = this;
	}

	template<typename T>
	Ref<T> AssetManager::Load(const std::string& name, const std::unordered_map<std::string, Ref<T>>& map)
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
			auto forceBuild = false; // m_FileWatcher.(Utils::FileGroupType::Shader, filepath);
			return m_Shaders[name] = Shader::Create(name, filepath, forceBuild);
		}
	}

	Ref<Shader> AssetManager::LoadShader(const std::string& filepath)
	{
		return Shader::Create(filepath, false);
	}

	Ref<Texture2D> AssetManager::LoadTexture(const std::string& filepath)
	{
		return Texture2D::Create(filepath);
	}

	Font AssetManager::LoadFont(const std::string& filepath)
	{
		return Font(filepath);
	}

	template<typename T>
	Ref<T> AssetManager::Get(const std::string& filepath, const std::unordered_map<std::string, Ref<T>>& map)
	{
		TED_CORE_ASSERT(Exists<T>(filepath, map), "file not found!");
		auto it = map.find(filepath);
		TED_CORE_ASSERT(it != map.end(), "file not found!");
		return it->second;
	}

	template<typename T>
	bool AssetManager::Exists(const std::string& filepath, const std::unordered_map<std::string, Ref<T>>& map) const
	{
		return map.find(filepath) != map.end();
	}
}