#pragma once

#include "TeddyPch.h"

namespace Teddy
{
	namespace Utils
	{
		class FileGroupWatcher
		{
		public:
			FileGroupWatcher(const std::string& filepath)
				: m_LastChangedDate(std::time(nullptr))
			{
				m_FilePaths.insert(filepath);
			}

			FileGroupWatcher() = default;
			~FileGroupWatcher() = default;

			void Add(const std::string& filepath) { m_FilePaths.insert(filepath); }
			void Remove(const std::string& filepath) { m_FilePaths.erase(filepath); }

			bool CheckOfflineChanges(std::time_t& lastTimeChecked, const std::string& filepath);
			bool CheckOfflineChanges(std::time_t& lastTimeChecked);

			void StartWatching();
			void StopWatching();

			bool OnUpdate();

			void SetHotReload(bool enabled);
			bool& IsHotReloading();

			std::unordered_set<std::string> GetFilesChanged(bool changesHandled);
			std::string AssetNeedsToReload(const std::string& filepath, bool changesHandled);
		private:
			std::unordered_set<std::string> m_FilePaths;
			std::unordered_set<std::string> m_FilesChanged;
			std::time_t m_LastChangedDate = std::time(nullptr);
			bool m_HotReload = false;
		};
		
	}
}