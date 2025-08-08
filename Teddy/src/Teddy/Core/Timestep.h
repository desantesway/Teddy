#pragma once

namespace Teddy 
{

	class Timestep 
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {
		}

		float GetMilliseconds() const { return m_Time; }
		float GetSeconds() const { return m_Time / 1000.0f; }

		operator float() const { return GetSeconds(); }
	private:
		float m_Time;
	};
}