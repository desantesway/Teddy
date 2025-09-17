#include "Teddy/Utils/FileGroupWatcher.h"

#include "Teddy/Renderer/Renderer.h"

namespace Teddy
{
	namespace Utils
	{
		bool FileGroupWatcher::CheckOfflineChanges(std::time_t& lastTimeChecked)
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
					if (!m_ShadersChanged.contains(filepath))
					{
						m_ShadersChanged.insert(filepath);
						m_LastChangedDate = cftime;
						changed = true;
					}
				}

			}

			return changed;
		}

		std::unordered_set<std::string> FileGroupWatcher::GetFilesChanged(bool changesHandled)
		{
			if (changesHandled)
			{
				std::unordered_set<std::string> result = std::move(m_ShadersChanged);
				m_ShadersChanged.clear();
				return result;
			}

			return m_ShadersChanged;
		}

		void FileGroupWatcher::StartWatching()
		{
			m_HotReload = true;
			m_LastChangedDate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}

		void FileGroupWatcher::StopWatching()
		{
			m_HotReload = false;
		}

		// returns true if any shader changed
		bool FileGroupWatcher::OnUpdate()
		{
			bool changed = false;

			if (m_HotReload)
			{
				for (const auto& filepath : m_FilePaths)
				{
					auto ftime = std::filesystem::last_write_time(filepath);
					auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
						ftime - std::filesystem::file_time_type::clock::now()
						+ std::chrono::system_clock::now());
					std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

					if (cftime > m_LastChangedDate)
					{
						if (!m_ShadersChanged.contains(filepath))
						{
							m_ShadersChanged.insert(filepath);
							m_LastChangedDate = cftime;
							changed = true;
						}
					}
				}
			}

			return changed;
		}

		void FileGroupWatcher::SetHotReload(bool enabled)
		{
			m_HotReload = enabled;
		}

		bool FileGroupWatcher::IsHotReloading() const
		{
			return m_HotReload;
		}
	}
}