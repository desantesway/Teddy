#pragma once
// Mouse codes for Teddy

namespace Teddy
{
	using MouseCode = uint8_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			Left = 0x00000000u,
			Middle = 0x00000001u,
			Right = 0x00000002u,
			X1 = 0x00000003u,
			X2 = 0x00000004u,
			Unknown = 0x00000005u,
		};
	}
}