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
				Vector3 StartingPosition;
				Vector3 LaunchVector;
				float TravelTime;
				float OutOfControl;
			};

			inline static FUNCTION_PTR(int, __thiscall, Add, ASLR(0x004C1BC0), int* This, Vector3* shotDirection, float a3, const char* a4);
		};
	}
}