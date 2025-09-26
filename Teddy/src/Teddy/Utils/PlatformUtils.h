#pragma once

#include <string>

namespace Teddy
{

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static std::vector<std::string> OpenFiles(const char* filter);
	};

	class Time
	{
	public:
		static float GetTime();
	};

}
