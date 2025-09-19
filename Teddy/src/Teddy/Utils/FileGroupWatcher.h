#pragma once

#include <string>
#include <chrono>

namespace Teddy
{
	namespace Utils
	{
		class FileGroupWatcher
		{
		public:
			// TODO: This will communicate with AssetManager to reload shaders and get the shader filepaths 
			// (not receiving them as an argument like it is now)
			FileGroupWatcher(const std::unordered_set<std::string>& filepaths)
				: m_FilePaths(filepaths), m_LastChangedDate(std::time(nullptr))
			{
			}

			FileGroupWatcher() = default;
			~FileGroupWatcher() = default;

			bool CheckOfflineChanges(std::time_t& lastTimeChecked, const std::string& filepath);
			bool CheckOfflineChanges(std::time_t& lastTimeChecked);

			void StartWatching();
			void StopWatching();

			// returns true if any shader changed
			bool OnUpdate();

			void SetHotReload(bool enabled);
			bool IsHotReloading() const;

			std::unordered_set<std::string> GetFilesChanged(bool changesHandled);
		private:
			std::unordered_set<std::string> m_FilePaths;
			std::unordered_set<std::string> m_ShadersChanged;
			std::time_t m_LastChangedDate = std::time(nullptr);
			bool m_HotReload = false;
		};
		
	}
}