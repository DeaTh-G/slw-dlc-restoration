#pragma once

namespace app
{
	HOOK(int*, __fastcall, CSharedObjInfoInitialize, ASLR(0x00666C90), char* This, void* edx, GameDocument* gameDocument)
	{
		int* result = originalCSharedObjInfoInitialize(This, edx, gameDocument);

		const char* packFileName = ObjUtil::GetStagePackName(gameDocument);
		if (strncmp(packFileName, "zdlc02", 6) == 0)
		{
			int coinModel = 0;

			int packFile;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));
			ObjUtil::GetModelResource(&coinModel, "zdlc02_obj_yoshicoin", &packFile);

			if (coinModel)
				*(int*)(This + 0x10) = coinModel;
		}

		return result;
	}

	class CSharedObjInfo
	{
	public:
		static void Initialize()
		{
			INSTALL_HOOK(CSharedObjInfoInitialize);
		}
	};
}