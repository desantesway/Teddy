#include "TeddyPch.h"
#include "Teddy/Utils/PlatformUtils.h"

#include "Teddy/Core/Application.h"

#include <commdlg.h>

#define SDL_PLATFORM_WIN32 1
#include <SDL3/SDL.h>

namespace Teddy
{

	float Time::GetTime()
	{
		return SDL_GetTicks();
	}

	HWND GetSDLNativeWindow()
	{
		SDL_PropertiesID props = SDL_GetWindowProperties(
			static_cast<SDL_Window*>(
				Application::Get().GetWindow().GetNativeWindow()
				)
		);
		HWND hwnd = static_cast<HWND>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL));

		TED_CORE_ASSERT(hwnd, "Failed to get HWND from SDL properties");

		return hwnd;
	}

	std::vector<std::string> FileDialogs::OpenFiles(const char* filter)
	{
		TED_PROFILE_CAT(InstrumentorCategory::IO);

		OPENFILENAMEA ofn;
		CHAR szFile[4096] = { 0 }; // Large buffer for multiple files
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);

		HWND hwnd = GetSDLNativeWindow();
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

		std::vector<std::string> result;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			char* p = ofn.lpstrFile;
			std::string directory = p;
			p += directory.length() + 1;

			// If only one file is selected, p points to '\0'
			if (*p == '\0')
			{
				result.push_back(directory);
			}
			else
			{
				// Multiple files selected
				while (*p)
				{
					result.push_back(directory + "\\" + p);
					p += strlen(p) + 1;
				}
			}
		}
		return result;
	}

	std::string FileDialogs::OpenFile(const char* filter)
	{
		TED_PROFILE_CAT(InstrumentorCategory::IO);

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);

		HWND hwnd = GetSDLNativeWindow();
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		TED_PROFILE_CAT(InstrumentorCategory::IO);

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		HWND hwnd = GetSDLNativeWindow();
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

}
