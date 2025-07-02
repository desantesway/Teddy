// Made by CrankyTed

#pragma once

#include <memory>

#ifdef TED_PLATFORM_WINDOWS
	#if TED_DYNAMIC_LINK
		#ifdef TED_BUILD_DLL
			#define TED_API __declspec(dllexport)
		#else
			#define TED_API __declspec(dllimport)
	#endif
	#else
		#define TED_API
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

#define TED_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Teddy {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}