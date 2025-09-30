#pragma once

#include "TeddyPch.h"
#include "Teddy/Utils/FileGroupWatcher.h"

namespace Teddy
{
	namespace Utils
	{

		enum class FileGroupType
		{
			None = 0,
			Shader,
			Texture2D,
			Font
		};

		class FileWatcher
		{
		public:
			FileWatcher() = default;
			FileWatcher(std::string LastTimeCheckedFilepath);
			FileWatcher(std::string LastTimeCheckedFilepath, const std::unordered_map<FileGroupType, FileGroupWatcher>& fileGroups);
			~FileWatcher() = default;

			bool& IsHotReloading(FileGroupType type);

			void CreateOfflineFile(std::string& filepath);

			bool CheckOfflineChanges(const FileGroupType& type, const std::string& offFilepath);

			void Add(FileGroupType type, const std::string& filepath);
			void Remove(FileGroupType type, const std::string& filepath);

			void Watch();

			void StartWatching();
			void StopWatching();
			void StartWatching(FileGroupType type);
			void StopWatching(FileGroupType type);

			std::unordered_set<std::string> AssetsToReload(FileGroupType type, bool changesHandled);
			std::string AssetNeedsToReload(FileGroupType type, const std::string& filepath, bool changesHandled);
		private:
			const std::string m_LastTimeCheckedFilepath;

			std::unordered_map<FileGroupType, FileGroupWatcher> m_FilesWatcher;
		};
		
	}
}