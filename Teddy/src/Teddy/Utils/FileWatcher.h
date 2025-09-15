#pragma once

#include "Teddy/Utils/ShaderWatcher.h"

#include <vector>
#include <string>

namespace Teddy
{
	namespace Utils
	{

		class FileWatcher
		{
		public:
			FileWatcher() = default;
			FileWatcher(std::string& LastTimeCheckedFilepath);
			~FileWatcher() = default;

			void CheckOfflineChanges();

			void Run();

			void StartWatching();
			void StopWatching();

			void CreateShaderWatching(const bool hotReload, const std::vector<std::string>& filepaths);
			// makes m_ShadersChanged empty if changesHandled is true
			std::vector<std::string> GetShadersChanged(bool changesHandled);
		private:
			std::string m_LastTimeCheckedFilepath;

			ShaderWatcher m_ShaderWatcher; // TODO: vector of file watchers (for different file types too)
		};
		
	}
}