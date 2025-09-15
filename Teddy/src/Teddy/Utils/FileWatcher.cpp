#include "Teddy/Utils/FileWatcher.h"
#include "Teddy/Renderer/Renderer.h"

#include <chrono>

namespace Teddy
{
	namespace Utils
	{
		FileWatcher::FileWatcher(std::string& LastTimeCheckedFilepath)
			: m_LastTimeCheckedFilepath(LastTimeCheckedFilepath)
		{
		}

		void FileWatcher::CheckOfflineChanges()
		{
			// open file get last time checked
			std::time_t lastTimeChecked;
			std::time_t present;
			m_ShaderWatcher.CheckOfflineChanges(lastTimeChecked, present);
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
	}
}