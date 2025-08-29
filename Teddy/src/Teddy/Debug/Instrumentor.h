#pragma once

#include "Teddy/Core/Log.h"

#include <string>

#include <optick.h>

namespace Teddy 
{

	using InstCat = uint16_t;

	namespace InstrumentorCategory
	{
		enum : InstCat
		{
			// CPU
			None =				0x00000000u,
			AI =				0x00000002u,
			Animation =			0x00000003u,
			Audio =				0x00000004u,
			Debug =				0x00000005u,
			Camera =			0x00000006u,
			Cloth =				0x00000007u,
			GameLogic =			0x00000008u,
			Input =				0x00000009u,
			Navigation =		0x0000000Au,
			Network =			0x0000000Bu,
			Physics =			0x0000000Cu,
			Rendering =			0x0000000Du,
			Scene =				0x0000000Eu,
			Script =			0x0000000Fu,
			Streaming =			0x00000010u,
			UI =				0x00000011u,
			VFX =				0x00000012u,
			Visibility =		0x00000013u,
			Wait =				0x00000014u,
			WaitEmpty =			0x00000015u,
			// IO
			IO =				0x00000016u,
			// GPU
			GPU_Cloth =			0x00000017u,
			GPU_Lighting =		0x00000018u,
			GPU_PostFX =		0x00000019u,
			GPU_Reflections =	0x0000001Au,
			GPU_Scene =			0x0000001Bu,
			GPU_Shadows =		0x0000001Cu,
			GPU_UI =			0x0000001Du,
			GPU_VFX =			0x0000001Eu,
			GPU_Water =			0x0000001Fu,
		};

	}

	namespace InstrumentorUtils
	{

		inline Optick::Category::Type TeddyCatToOptickCat(const InstCat& cat)
		{
			switch (cat)
			{
			case InstrumentorCategory::AI:				return Optick::Category::AI;
			case InstrumentorCategory::Animation:		return Optick::Category::Animation;
			case InstrumentorCategory::Audio:			return Optick::Category::Audio;
			case InstrumentorCategory::Debug:			return Optick::Category::Debug;
			case InstrumentorCategory::Camera:			return Optick::Category::Camera;
			case InstrumentorCategory::Cloth:			return Optick::Category::Cloth;
			case InstrumentorCategory::GameLogic:		return Optick::Category::GameLogic;
			case InstrumentorCategory::Input:			return Optick::Category::Input;
			case InstrumentorCategory::Navigation:		return Optick::Category::Navigation;
			case InstrumentorCategory::Network:			return Optick::Category::Network;
			case InstrumentorCategory::Physics:			return Optick::Category::Physics;
			case InstrumentorCategory::Rendering:		return Optick::Category::Rendering;
			case InstrumentorCategory::Scene:			return Optick::Category::Scene;
			case InstrumentorCategory::Script:			return Optick::Category::Script;
			case InstrumentorCategory::Streaming:		return Optick::Category::Streaming;
			case InstrumentorCategory::UI:				return Optick::Category::UI;
			case InstrumentorCategory::VFX:				return Optick::Category::VFX;
			case InstrumentorCategory::Visibility:		return Optick::Category::Visibility;
			case InstrumentorCategory::Wait:			return Optick::Category::Wait;
			case InstrumentorCategory::WaitEmpty:		return Optick::Category::WaitEmpty;
			case InstrumentorCategory::IO:				return Optick::Category::IO;
			case InstrumentorCategory::GPU_Cloth:		return Optick::Category::GPU_Cloth;
			case InstrumentorCategory::GPU_Lighting:	return Optick::Category::GPU_Lighting;
			case InstrumentorCategory::GPU_PostFX:		return Optick::Category::GPU_PostFX;
			case InstrumentorCategory::GPU_Reflections:	return Optick::Category::GPU_Reflections;
			case InstrumentorCategory::GPU_Scene:		return Optick::Category::GPU_Scene;
			case InstrumentorCategory::GPU_Shadows:		return Optick::Category::GPU_Shadows;
			case InstrumentorCategory::GPU_UI:			return Optick::Category::GPU_UI;
			case InstrumentorCategory::GPU_VFX:			return Optick::Category::GPU_VFX;
			case InstrumentorCategory::GPU_Water:		return Optick::Category::GPU_Water;
			default:									return Optick::Category::None;
			}
		}

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

		void NewFrame(const std::string& name = "Main Thread")
		{
			OPTICK_FRAME(name.c_str());
		}

		void Category(const std::string& name = "Main Thread", const InstCat& cat = InstrumentorCategory::None)
		{
			OPTICK_CATEGORY(name.c_str(), InstrumentorUtils::TeddyCatToOptickCat(cat));
		}


	private:
		Instrumentor() = default;
		~Instrumentor() = default;
	};

}

#ifdef TED_DEBUG
	#define TED_PROFILE 1
#endif

#if TED_PROFILE
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
	#define TED_PROFILE_FRAME_SCOPE(name) ::Teddy::Instrumentor::Get().NewFrame(name);
	#define TED_PROFILE_FRAME() TED_PROFILE_FRAME_SCOPE(TED_FUNC_SIG);
	#define TED_PROFILE_CATEGORY(name, cat) TED_PROFILE_SCOPE(name); \
											::Teddy::Instrumentor::Get().Category(name, cat);
	#define TED_PROFILE_CAT(cat) TED_PROFILE_CATEGORY(TED_FUNC_SIG, cat);
	#define TED_PROFILE_TAGGED(name, tag) OPTICK_TAG(name, tag);
	#define TED_PROFILE_TAG(tag) TED_PROFILE_TAGGED(TED_FUNC_SIG, tag);
#else
	#define TED_PROFILE_BEGIN_SESSION(name, filepath)
	#define TED_PROFILE_END_SESSION()
	#define TED_PROFILE_SCOPE(name)
	#define TED_PROFILE_FUNCTION()
	#define TED_PROFILE_FRAME(name)
	#define TED_PROFILE_FRAME()
	#define TED_PROFILE_CATEGORY(name, cat)
	#define TED_PROFILE_CAT(cat)
	#define TED_PROFILE_TAGGED(name, tag)
	#define TED_PROFILE_TAG(tag)
#endif