#include "Teddy/Utils/FileWatcher.h"
#include "Teddy/Renderer/Renderer.h"

#include <chrono>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace Teddy
{
	namespace Utils
	{
		// TODO: implement with single file
		namespace fs = std::filesystem;

		FileWatcher::FileWatcher(std::string LastTimeCheckedFilepath, const std::unordered_map<FileGroupType, FileGroupWatcher>& fileGroups)
			: m_LastTimeCheckedFilepath(LastTimeCheckedFilepath)
		{
			m_FilesWatcher = fileGroups;
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

				for (auto& [key, _] : m_FilesWatcher)
				{
					out << YAML::Key << static_cast<int>(key) << YAML::Value << YAML::BeginMap;
					out << YAML::Key << "LastTimeChecked" << YAML::Value 
						<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					out << YAML::EndMap;
				}

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

				for (auto& [key, value] : m_FilesWatcher)
				{

					auto fileGroup = offWatchers[static_cast<int>(key)];

					out << YAML::Key << static_cast<int>(key) << YAML::Value << YAML::BeginMap;

					if (fileGroup)
					{
						auto lastTimeChecked = fileGroup["LastTimeChecked"].as<std::time_t>();
						if (value.CheckOfflineChanges(lastTimeChecked))
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

				}
			}

			out << YAML::EndMap;
			out << YAML::EndMap;

			std::ofstream fout(filepath);
			fout << out.c_str();
		}

		// TODO: Create a thread, implement Asset Manager first
		void FileWatcher::Watch()
		{
			while (true)
			{
				bool updated = false;

				YAML::Emitter out;
				out << YAML::BeginMap;
				out << YAML::Key << "OfflineWatcher" << YAML::Value << YAML::BeginMap;

				for (auto& [key, value] : m_FilesWatcher)
				{
					out << YAML::Key << static_cast<int>(key) << YAML::Value << YAML::BeginMap;

					if (value.OnUpdate())
					{
						updated = true;

						TED_CORE_INFO("Detected changes {}", (int)key);
						value.GetFilesChanged(true);
					}

					out << YAML::Key << "LastTimeChecked" << YAML::Value
						<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

					out << YAML::EndMap;
				}

				out << YAML::EndMap;
				out << YAML::EndMap;

				if (updated)
				{
					std::ofstream fout(m_LastTimeCheckedFilepath);
					fout << out.c_str();
				}
			}
		}

		void FileWatcher::StartWatching(FileGroupType type)
		{
			m_FilesWatcher[type].StartWatching();
		}

		void FileWatcher::StartWatching()
		{
			for (auto& [key, value] : m_FilesWatcher)
			{
				value.StartWatching();
			}
		}

		void FileWatcher::StopWatching(FileGroupType type)
		{
			m_FilesWatcher[type].StopWatching();
		}

		void FileWatcher::StopWatching()
		{
			// get thread and stop that thread
			for (auto& [key, value] : m_FilesWatcher)
			{
				value.StopWatching();
			}
		}

		std::unordered_set<std::string> FileWatcher::GetFileGroupChanged(FileGroupType type, bool changesHandled)
		{
			return m_FilesWatcher[type].GetFilesChanged(changesHandled);
		}
	}
}