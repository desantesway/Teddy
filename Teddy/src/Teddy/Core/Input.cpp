#include "TeddyPch.h"
#include "Teddy/Core/Input.h"
#include "Teddy/Core/TeddyInput.h"

namespace Teddy {
	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
		return CreateScope<TeddyInput>();
		TED_CORE_ASSERT(false, "Input.cpp - Unknown platform!");

		return nullptr;
	}
}