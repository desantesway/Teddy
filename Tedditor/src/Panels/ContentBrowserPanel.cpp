#include "TeddyPch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace Teddy
{

	// Once we have projects, change this
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/Directory.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/File.png");
		m_TeddyIcon = Texture2D::Create("resources/icons/contentBrowser/Teddy.png");
	}

	Ref<Texture2D> ContentBrowserPanel::GetIcon(std::string file)
	{
		if (file.ends_with(".teddy"))
			return m_TeddyIcon;
		return m_FileIcon;
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : GetIcon(filenameString);
			ImGui::ImageButton(filenameString.c_str(), (ImTextureID)icon->GetRendererID(), 
				{ thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}
			
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}
