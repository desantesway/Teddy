#include "Teddy/Utils/FileGroupWatcher.h"

namespace Teddy
{
	namespace Utils
	{
		bool FileGroupWatcher::CheckOfflineChanges(std::time_t& lastTimeChecked, const std::string& filepath)
		{
			bool changed = false;

			auto ftime = std::filesystem::last_write_time(filepath);
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				ftime - std::filesystem::file_time_type::clock::now()
				+ std::chrono::system_clock::now());
			std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

			if (cftime > lastTimeChecked)
			{
				if (!m_FilesChanged.contains(filepath))
				{
					m_FilesChanged.insert(filepath);
					m_LastChangedDate = cftime;
					changed = true;
				}
			}

			return changed;
		}

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
					if (!m_FilesChanged.contains(filepath))
					{
						m_FilesChanged.insert(filepath);
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
				std::unordered_set<std::string> result = std::move(m_FilesChanged);
				m_FilesChanged.clear();
				return result;
			}

			return m_FilesChanged;
		}

		std::string FileGroupWatcher::AssetNeedsToReload(const std::string& filepath, bool changesHandled)
		{
			if (m_FilesChanged.contains(filepath))
			{
				if (changesHandled)
				{
					m_FilesChanged.erase(filepath);
				}

				return filepath;
			}
			return "";
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

		bool FileGroupWatcher::OnUpdate()
		{
			bool changed = false;

			if (m_HotReload)
			{
				for (const auto& filepath : m_FilePaths)
				{
					try
					{
						auto ftime = std::filesystem::last_write_time(filepath);
						auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
							ftime - std::filesystem::file_time_type::clock::now()
							+ std::chrono::system_clock::now());
						std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

						if (cftime > m_LastChangedDate)
						{
							if (!m_FilesChanged.contains(filepath))
							{
								m_FilesChanged.insert(filepath);
								m_LastChangedDate = cftime;
								changed = true;
							}
						}
					}
					catch (const std::filesystem::filesystem_error& e)
					{
						TED_CORE_WARN("FileGroupWatcher: Failed to query last_write_time for '{0}': {1}", filepath, e.what());
					}
				}
			}

			return changed;
		}

		void FileGroupWatcher::SetHotReload(bool enabled)
		{
			m_HotReload = enabled;
		}

		bool& FileGroupWatcher::IsHotReloading()
		{
			return m_HotReload;
		}
	}
}