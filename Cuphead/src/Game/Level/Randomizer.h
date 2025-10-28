#pragma once

#include "teddyPch.h"

namespace Cuphead
{
	class Randomizer
	{
	public:
		Randomizer();
		~Randomizer() = default;

		static Randomizer& Get() { return *s_Instance; }

		int RandomInt(int min, int max);
		float RandomFloat(float min, float max);
		bool RandomBool(float trueProbability = 0.5f);
	private:
		static std::mt19937 m_Rng;

	private:
		static Randomizer* s_Instance;
	};
}