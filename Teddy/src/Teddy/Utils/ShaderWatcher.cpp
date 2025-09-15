#include "Teddy/Utils/ShaderWatcher.h"

#include "Teddy/Renderer/Renderer.h"

namespace Teddy
{
	namespace Utils
	{
		bool ShaderWatcher::CheckOfflineChanges(std::time_t& lastTimeChecked)
		{
			bool changed = false;

			for (const auto& filepath : m_FilePaths)
			{
				auto ftime = std::filesystem::last_write_time(filepath);
				auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
					ftime - std::filesystem::file_time_type::clock::now()
					+ std::chrono::system_clock::now());
				std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

				if (cftime > lastTimeChecked)
				{
					m_ShadersChanged.insert(filepath);
					m_LastChangedDate = cftime;
					changed = true;
				}

			}

			return changed;
		}

		std::unordered_set<std::string>& ShaderWatcher::GetFilesChanged(bool changesHandled)
		{
			return m_ShadersChanged;
		}

		void ShaderWatcher::StartWatching()
		{

		}

		void ShaderWatcher::StopWatching()
		{

		}

		// returns true if any shader changed
		bool ShaderWatcher::Update()
		{
			return false;
		}

		void ShaderWatcher::SetHotReload(bool enabled)
		{
			m_HotReload = enabled;
		}

		bool ShaderWatcher::IsHotReloading() const
		{
			return m_HotReload;
		}
	}
}