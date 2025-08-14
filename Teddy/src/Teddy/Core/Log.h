#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "Teddy/Core/Base.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace Teddy
{
	class Log
	{
		public:
			static void Init();

			static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		private:
			static Ref<spdlog::logger> s_CoreLogger;	
			static Ref<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define TED_CORE_ERROR(...)    ::Teddy::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TED_CORE_WARN(...)     ::Teddy::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TED_CORE_INFO(...)     ::Teddy::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TED_CORE_TRACE(...)    ::Teddy::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TED_CORE_CRITICAL(...) ::Teddy::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Core log macros
#define TED_ERROR(...)         ::Teddy::Log::GetClientLogger()->error(__VA_ARGS__)
#define TED_WARN(...)          ::Teddy::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TED_INFO(...)          ::Teddy::Log::GetClientLogger()->info(__VA_ARGS__)
#define TED_TRACE(...)         ::Teddy::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TED_CRITICAL(...)      ::Teddy::Log::GetClientLogger()->critical(__VA_ARGS__)

