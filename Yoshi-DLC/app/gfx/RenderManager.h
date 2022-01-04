#pragma once

namespace app
{
	namespace gfx
	{
		class RenderManager
		{
		public:
			void SetLocalLightRadius(short a1, float a2, float a3, float a4, float a5)
			{
				int result = *(int*)(*(int*)this + 16 + 114396);
				*(float*)(result + 48 * *((short*)(result + 2 * a1 + 35136)) + 22880) = a2;
				*(float*)(result + 48 * *((short*)(result + 2 * a1 + 35136)) + 22884) = a3;
				*(float*)(result + 48 * *((short*)(result + 2 * a1 + 35136)) + 22888) = a4;
				*(float*)(result + 48 * *((short*)(result + 2 * a1 + 35136)) + 22892) = a5;
			}
		};
	}
}