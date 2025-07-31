#pragma once
// Mouse codes for Teddy

namespace Teddy
{
	typedef enum class MouseCode : uint8_t
	{
		Left		= 0x00000000u,
		Middle		= 0x00000001u,
		Right		= 0x00000002u,
		X1			= 0x00000003u,
		X2			= 0x00000004u,
		Unknown		= 0x00000005u,
	};

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}