#pragma once

namespace csl
{
	namespace math
	{
		struct alignas(16) Matrix34
		{
			float data[4][4];
		};
	}
}