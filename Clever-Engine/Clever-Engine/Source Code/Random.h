#pragma once

#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "Globals.h"

namespace Random
{
	static math::LCG lcgRand;

	inline uint32 GetRandomUint()
	{
		return (uint32)lcgRand.Int();
	}
}