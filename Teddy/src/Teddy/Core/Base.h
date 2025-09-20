// Made by CrankyTed

#pragma once

#include "Teddy/Core/PlatformDetection.h"

#include <memory>

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

#define TED_EXPAND_MACRO(x) x
#define TED_STRINGIFY_MACRO(x) #x

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

	template<typename T>
	using Weak = std::weak_ptr<T>;
}

#include "Teddy/Core/Log.h"
#include "Teddy/Core/Assert.h"