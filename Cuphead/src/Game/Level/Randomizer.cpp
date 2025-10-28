#include "Randomizer.h"

namespace Cuphead
{
	std::mt19937 Randomizer::m_Rng;
	Randomizer* Randomizer::s_Instance;

	Randomizer::Randomizer()
	{
		TED_CORE_ASSERT(!s_Instance, "Randomizer already exists!");
		s_Instance = this;

		std::random_device rd;
		m_Rng = std::mt19937(rd());
	}

	int Randomizer::RandomInt(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(m_Rng);
	}

	float Randomizer::RandomFloat(float min, float max)
	{
		std::uniform_real_distribution<float> realDistribution(min, max);
		return realDistribution(m_Rng);
	}

	bool Randomizer::RandomBool(float trueProbability)
	{
		std::bernoulli_distribution dist(trueProbability);
		return dist(m_Rng);
	}
}