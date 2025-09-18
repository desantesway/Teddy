#pragma once

#include <Teddy/Core/Base.h>

namespace Teddy
{
	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init();
		static void Shutdown();
	private:
		Ref<AssetManager> m_Instance;
	};
}