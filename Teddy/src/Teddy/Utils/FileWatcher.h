#pragma once

#include "Teddy/Utils/ShaderWatcher.h"

#include <unordered_set>
#include <string>

namespace Teddy
{
	namespace Utils
	{

		class FileWatcher
		{
		public:
			FileWatcher() = default;
			FileWatcher(std::string LastTimeCheckedFilepath);
			FileWatcher(std::string LastTimeCheckedFilepath, const std::unordered_set<std::string>& filepaths);
			~FileWatcher() = default;

			bool CheckOfflineChanges();

			void Run();

			void StartWatching();
			void StopWatching();

			void CreateShaderWatching(const bool hotReload, const std::vector<std::string>& filepaths);
			// makes m_ShadersChanged empty if changesHandled is true
			std::unordered_set<std::string> GetShadersChanged(bool changesHandled);
		private:
			const std::string m_LastTimeCheckedFilepath;

			ShaderWatcher m_ShaderWatcher; // TODO: vector of file watchers (for different file types too)
		};
		
	}
}