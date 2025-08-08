// Made by CrankyTed

#pragma once

#include <memory>

#include "Teddy/Core/PlatformDetection.h"

#ifdef TED_DEBUG
	#define TED_ENABLE_ASSERTS
	#if defined(TED_PLATFORM_WINDOWS)
		#define TED_DEBUGBREAK() __debugbreak()
	#elif defined(TED_PLATFORM_LINUX)
		#include <signal.h>
		#define TED_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define TED_DEBUGBREAK()
#endif

#ifdef TED_ENABLE_ASSERTS
#define TED_ASSERT(x, ...) { if(!(x)) { TED_ERROR("Assertion Failed: {0}", __VA_ARGS__); TED_DEBUGBREAK(); } }
#define TED_CORE_ASSERT(x, ...) { if(!(x)) { TED_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); TED_DEBUGBREAK(); } }
#else
#define TED_ASSERT(x, ...)
#define TED_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define TED_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Teddy 
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}