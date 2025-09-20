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

		FileWatcher::FileWatcher(std::string LastTimeCheckedFilepath, const std::unordered_map<FileGroupType, FileGroupWatcher>& fileGroups)
			: m_LastTimeCheckedFilepath(LastTimeCheckedFilepath)
		{
			m_FilesWatcher = fileGroups;
		}

		FileWatcher::FileWatcher(std::string LastTimeCheckedFilepath)
			: m_LastTimeCheckedFilepath(LastTimeCheckedFilepath)
		{
		}
		
		bool& FileWatcher::IsHotReloading(FileGroupType type)
		{
			if (m_FilesWatcher.find(type) == m_FilesWatcher.end())
			{
				m_FilesWatcher[type] = FileGroupWatcher();
				static bool shaderHotReloading = false;
				return shaderHotReloading;
			}
			else
			{
				return m_FilesWatcher.at(type).IsHotReloading();
			}
		}

		void FileWatcher::Remove(FileGroupType type, const std::string& filepath)
		{
			if (m_FilesWatcher.find(type) == m_FilesWatcher.end())
			{
				TED_CORE_ASSERT(false, "FileGroupType not found");
			}
			else
			{
				m_FilesWatcher.at(type).Remove(filepath);
			}
		}

		void FileWatcher::Add(FileGroupType type, const std::string& filepath)
		{
			if (m_FilesWatcher.find(type) == m_FilesWatcher.end())
			{
				m_FilesWatcher[type] = FileGroupWatcher(filepath);
			}
			else 
			{
				m_FilesWatcher.at(type).Add(filepath);
			}
		}

		void FileWatcher::CreateOfflineFile(std::string& filepath)
		{
			if (!fs::exists(filepath)) {

				fs::path path = filepath;
				fs::path parent = path.parent_path();
				if (!fs::exists(parent) && !fs::create_directory(parent))
				{
					TED_CORE_ERROR("Failed to create directory for OfflineWatcher file '{0}'", parent.string());
				}

				YAML::Emitter out;
				out << YAML::BeginMap;

				out << YAML::Key << "OfflineWatcher" << YAML::Value << YAML::BeginMap;

				for (auto& [key, value] : m_FilesWatcher)
				{
					out << YAML::Key << static_cast<int>(key) << YAML::Value << YAML::BeginMap;
					out << YAML::EndMap;
				}

				out << YAML::EndMap;

				out << YAML::EndMap;

				std::ofstream fout(filepath, std::ios::out | std::ios::trunc);
				if (!fout.is_open())
				{
					TED_CORE_ERROR("Failed to create OfflineWatcher file '{0}'", filepath);
				}
				fout << out.c_str();
			}
		}

		bool FileWatcher::CheckOfflineChanges(const FileGroupType& type, const std::string& offFilepath)
		{

			std::string filepath = m_LastTimeCheckedFilepath;

			CreateOfflineFile(filepath);

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

			bool fileChanged = false;
			YAML::Node offWatchers = data["OfflineWatcher"];
			if (offWatchers && offWatchers.IsMap())
			{
				bool typeFound = false;
				for (auto it = offWatchers.begin(); it != offWatchers.end(); ++it)
				{
					int fileType = it->first.as<int>();

					out << YAML::Key << fileType << YAML::Value << YAML::BeginMap;

					auto fileGroup = offWatchers[fileType];

					if (fileType == static_cast<int>(type))
					{
						typeFound = true;
					}

					if (fileGroup && fileGroup.IsMap())
					{
						bool pathFound = false;
						for (auto fileOffline = fileGroup.begin(); fileOffline != fileGroup.end(); ++fileOffline)
						{
							std::string path = fileOffline->first.as<std::string>();
							std::time_t lastTimeChecked = fileOffline->second.as<std::time_t>();
							if (fileType == static_cast<int>(type) && path == offFilepath)
							{
								pathFound = true;
								if (m_FilesWatcher[type].CheckOfflineChanges(lastTimeChecked, offFilepath))
								{
									fileChanged = true;
									out << YAML::Key << path << YAML::Value
										<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
								}
								else
								{
									out << YAML::Key << path << YAML::Value << lastTimeChecked;
								}
							}
							else
							{
								out << YAML::Key << path << YAML::Value << lastTimeChecked;
							}
						}

						if (!pathFound)
						{
							out << YAML::Key << offFilepath << YAML::Value
								<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
						}
					}

					out << YAML::EndMap;
				}

				if (!typeFound)
				{
					out << YAML::Key << static_cast<int>(type) << YAML::Value << YAML::BeginMap;
					out << YAML::Key << offFilepath << YAML::Value
						<< std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					out << YAML::EndMap;
				}
			}
			else 
			{
				TED_CORE_ASSERT(false, "OfflineWatcher file is corrupted");
			}

			out << YAML::EndMap;
			out << YAML::EndMap;

			std::ofstream fout(filepath);
			fout << out.c_str();

			return fileChanged;
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

		std::unordered_set<std::string> FileWatcher::AssetsToReload(FileGroupType type, bool changesHandled)
		{
			return m_FilesWatcher[type].GetFilesChanged(changesHandled);
		}
	}
}