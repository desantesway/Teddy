#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Teddy
{
	class TED_API Log
	{
		public:
			static void Init();

			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;	
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define TED_CORE_ERROR(...)    ::Teddy::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TED_CORE_WARN(...)     ::Teddy::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TED_CORE_INFO(...)     ::Teddy::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TED_CORE_TRACE(...)    ::Teddy::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TED_CORE_FATAL(...)    ::Teddy::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core log macros
#define TED_ERROR(...)         ::Teddy::Log::GetClientLogger()->error(__VA_ARGS__)
#define TED_WARN(...)          ::Teddy::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TED_INFO(...)          ::Teddy::Log::GetClientLogger()->info(__VA_ARGS__)
#define TED_TRACE(...)         ::Teddy::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TED_FATAL(...)         ::Teddy::Log::GetClientLogger()->fatal(__VA_ARGS__)

