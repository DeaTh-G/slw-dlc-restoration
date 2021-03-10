#pragma once

namespace app
{
	namespace game
	{
		class GOCLauncher
		{
		public:
			struct ShotInfo
			{
				csl::math::Vector3 StartingPosition;
				csl::math::Vector3 LaunchVector;
				float TravelTime;
				float OutOfControl;
			};

			inline static FUNCTION_PTR(int, __thiscall, Add, ASLR(0x004C1BC0), int* This, csl::math::Vector3* shotDirection, float a3, const char* a4);
		};
	}
}