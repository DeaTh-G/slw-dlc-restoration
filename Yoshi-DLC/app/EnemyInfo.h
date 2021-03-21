#pragma once

namespace app
{
	class EnemyInfo : public CObjInfo
	{
	private:
		inline static FUNCTION_PTR(void, __thiscall, f_Load, ASLR(0x00671D90), EnemyInfo* This, int& loader);

	public:
		const char* PacfileName;
		const char* field_14{};

		void Load(int& loader) override
		{
			f_Load(this, loader);
		}

		inline static FUNCTION_PTR(int*, __thiscall, GetEnemyPackfile, ASLR(0x00672480), EnemyInfo* This, int* a2);
		inline static FUNCTION_PTR(int*, __thiscall, GetModelResource, ASLR(0x006725F0), EnemyInfo* This, int* a2, const char* name);
		inline static FUNCTION_PTR(int*, __thiscall, GetSkeletonResource, ASLR(0x00672690), EnemyInfo* This, int* a2, const char* name);
		inline static FUNCTION_PTR(int*, __thiscall, GetAnimationResource, ASLR(0x00672640), EnemyInfo* This, int* a2, const char* name);
		inline static FUNCTION_PTR(int*, __thiscall, GetTexSrtAnimationResource, ASLR(0x00672730), EnemyInfo* This, int* a2, const char* name);
	};
}