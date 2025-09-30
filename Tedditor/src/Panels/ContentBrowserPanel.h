#pragma once

#include "Teddy/Renderer/Texture.h"

namespace Teddy
{

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		Ref<Texture2D> GetIcon(std::string file);

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_TeddyIcon;
	};

}
