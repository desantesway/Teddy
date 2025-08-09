#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Core/Log.h"
#include <filesystem>

#ifdef TED_ENABLE_ASSERTS

	#define TED_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { TED##type##ERROR(msg, __VA_ARGS__); TED_DEBUGBREAK(); } }
	#define TED_INTERNAL_ASSERT_WITH_MSG(type, check, ...) TED_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define TED_INTERNAL_ASSERT_NO_MSG(type, check) TED_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", TED_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__), __LINE__)

	#define TED_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define TED_INTERNAL_ASSERT_GET_MACRO(...) TED_EXPAND_MACRO( TED_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TED_INTERNAL_ASSERT_WITH_MSG, TED_INTERNAL_ASSERT_NO_MSG) )

	#define TED_ASSERT(x, ...) { if(!(x)) { TED_ERROR("Assertion Failed: {0}", __VA_ARGS__); TED_DEBUGBREAK(); } }
	#define TED_CORE_ASSERT(x, ...) { if(!(x)) { TED_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); TED_DEBUGBREAK(); } }
	#else
	#define TED_ASSERT(x, ...)
	#define TED_CORE_ASSERT(x, ...)

#endif