#include "Teddy/Utils/FileWatcher.h"
#include "Teddy/Renderer/Renderer.h"

#include <chrono>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace Teddy
{
	namespace Utils
	{
		namespace fs = std::filesystem;

		FileWatcher::FileWatcher(std::string LastTimeCheckedFilepath, const std::unordered_set<std::string>& filepaths)
			: m_LastTimeCheckedFilepath(LastTimeCheckedFilepath)
		{
			m_ShaderWatcher = ShaderWatcher(filepaths);
		}

		FileWatcher::FileWatcher(std::string LastTimeCheckedFilepath)
			: m_LastTimeCheckedFilepath(LastTimeCheckedFilepath)
		{
		}

		bool FileWatcher::CheckOfflineChanges()
		{

			std::string filepath = m_LastTimeCheckedFilepath;

			if (!fs::exists(filepath)) {

				fs::path path = filepath;
				fs::path parent = path.parent_path();
				if (!fs::exists(parent) && !fs::create_directory(parent))
				{
					TED_CORE_ERROR("Failed to create directory for OfflineWatcher file '{0}'", parent.string());
					return false;
				}

				YAML::Emitter out;
				out << YAML::BeginMap;

				out << YAML::Key << "OfflineWatcher" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Shader" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "LastTimeChecked" << YAML::Value 
					<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				out << YAML::EndMap;

				out << YAML::EndMap;

				out << YAML::EndMap;

				std::ofstream fout(filepath, std::ios::out | std::ios::trunc);
				if (!fout.is_open())
				{
					TED_CORE_ERROR("Failed to create OfflineWatcher file '{0}'", filepath);
					return false;
				}
				fout << out.c_str();
			}

			YAML::Node data;
			try
			{
				data = YAML::LoadFile(filepath);
			}
			catch (YAML::ParserException e)
			{
				TED_CORE_ERROR("Failed to load OfflineWatcher file '{0}'\n     {1}", filepath, e.what());
				return false;
			}

			YAML::Emitter out;
			out << YAML::BeginMap;
			out << YAML::Key << "OfflineWatcher" << YAML::Value << YAML::BeginMap;

			YAML::Node offWatchers = data["OfflineWatcher"];
			if (offWatchers)
			{
				auto shader = offWatchers["Shader"];

				out << YAML::Key << "Shader" << YAML::Value << YAML::BeginMap;

				if (shader)
				{
					auto lastTimeChecked = shader["LastTimeChecked"].as<std::time_t>();
					if (m_ShaderWatcher.CheckOfflineChanges(lastTimeChecked))
					{
						out << YAML::Key << "LastTimeChecked" << YAML::Value 
							<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					}
					else
					{
						out << YAML::Key << "LastTimeChecked" << YAML::Value << lastTimeChecked;
					}
				}
				else 
				{
					out << YAML::Key << "LastTimeChecked" << YAML::Value
						<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				}

				out << YAML::EndMap;

				// another file
			}

			out << YAML::EndMap;
			out << YAML::EndMap;

			std::ofstream fout(filepath);
			fout << out.c_str();
		}

		//TODO: Create thread
		void FileWatcher::StartWatching()
		{
			m_ShaderWatcher.StartWatching();
		}

		void FileWatcher::StopWatching()
		{
			m_ShaderWatcher.StopWatching();
		}

		void FileWatcher::CreateShaderWatching(const bool hotReload, const std::vector<std::string>& filepaths)
		{
			m_ShaderWatcher.SetHotReload(hotReload);
		}

		std::unordered_set<std::string> FileWatcher::GetShadersChanged(bool changesHandled)
		{
			return m_ShaderWatcher.GetFilesChanged(changesHandled);
		}
	}
}