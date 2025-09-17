#pragma once

#include "Teddy/Utils/FileGroupWatcher.h"

#include <unordered_map>
#include <string>

namespace Teddy
{
	namespace Utils
	{

		enum class FileGroupType
		{
			None = 0,
			Shader,

			// keep texture last
			Texture
		};

		class FileWatcher
		{
		public:
			FileWatcher() = default;
			FileWatcher(std::string LastTimeCheckedFilepath);
			FileWatcher(std::string LastTimeCheckedFilepath, const std::unordered_map<FileGroupType, FileGroupWatcher>& fileGroups);
			~FileWatcher() = default;

			bool CheckOfflineChanges();

			void Watch();

			void StartWatching();
			void StopWatching();
			void StartWatching(FileGroupType type);
			void StopWatching(FileGroupType type);

			// makes m_ShadersChanged empty if changesHandled is true
			std::unordered_set<std::string> GetFileGroupChanged(FileGroupType type, bool changesHandled);
		private:
			const std::string m_LastTimeCheckedFilepath;

			std::unordered_map<FileGroupType, FileGroupWatcher> m_FilesWatcher;
		};
		
	}
}