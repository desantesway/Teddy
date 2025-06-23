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


#ifdef TED_DEBUG
	#define TED_ENABLE_ASSERTS
#endif

#ifdef TED_ENABLE_ASSERTS
#define TED_ASSERT(x, ...) { if(!(x)) { TED_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define TED_CORE_ASSERT(x, ...) { if(!(x)) { TED_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define TED_ASSERT(x, ...)
#define TED_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)