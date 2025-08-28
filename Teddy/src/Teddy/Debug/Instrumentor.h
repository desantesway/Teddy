#pragma once

#include "Teddy/Core/Log.h"

#include <string>

#include <optick.h>

namespace Teddy 
{

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession()
		{
			OPTICK_START_CAPTURE();
		}

		void EndSession(const std::string& filepath = "results.opt")
		{
			OPTICK_STOP_CAPTURE();
			OPTICK_SAVE_CAPTURE(filepath.c_str());
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor() = default;
		~Instrumentor() = default;
	};

	namespace InstrumentorUtils 
	{

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;

			while (srcIndex < N && dstIndex < N - 1) // Ensure we don't overflow the destination
			{
				size_t matchIndex = 0;
				// Check for the substring we want to remove
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;

				if (matchIndex == K - 1)
					srcIndex += matchIndex; // Skip the matched substring

				// Copy and replace characters
				if (srcIndex < N) {
					result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				}
				srcIndex++;
			}

			result.Data[dstIndex] = '\0';

			return result;
		}
	}
}

#ifdef TED_DEBUG
	#define TED_PROFILE 1
#endif

#if TED_PROFILE
	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define TED_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define TED_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define TED_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define TED_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define TED_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define TED_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define TED_FUNC_SIG __func__
	#else
		#define TED_FUNC_SIG "TED_FUNC_SIG unknown!"
#endif

	#define TED_PROFILE_BEGIN_SESSION() ::Teddy::Instrumentor::Get().BeginSession()
	#define TED_PROFILE_END_SESSION(filepath) ::Teddy::Instrumentor::Get().EndSession(filepath)
	#define TED_PROFILE_SCOPE_LINE2(name, line)  static constexpr auto fixedName##line = ::Teddy::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
												OPTICK_EVENT(fixedName##line.Data);
	#define TED_PROFILE_SCOPE_LINE(name, line) TED_PROFILE_SCOPE_LINE2(name, line)
	#define TED_PROFILE_SCOPE(name) TED_PROFILE_SCOPE_LINE(name, __LINE__)
	#define TED_PROFILE_FUNCTION() TED_PROFILE_SCOPE(TED_FUNC_SIG)
	#define TED_PROFILE_FRAME(name)  OPTICK_FRAME(name);
#else
	#define TED_PROFILE_BEGIN_SESSION(name, filepath)
	#define TED_PROFILE_END_SESSION()
	#define TED_PROFILE_SCOPE(name)
	#define TED_PROFILE_FUNCTION()
#endif