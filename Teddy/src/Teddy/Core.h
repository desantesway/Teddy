// Made by CrankyTed

#pragma once

#ifdef TED_PLATFORM_WINDOWS
	#ifdef TED_BUILD_DLL
		#define TED_API __declspec(dllexport)
	#else
		#define TED_API __declspec(dllimport)
	#endif
#else
	#error "Teddy only supports Windows platform."
#endif