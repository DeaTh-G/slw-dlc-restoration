#include "pch.h"

HOOK(int*, __fastcall, CSharedObjInfoInitialize, ASLR(0x00666C90), char* This, void* edx, app::GameDocument* gameDocument)
{
    int* result = originalCSharedObjInfoInitialize(This, edx, gameDocument);

    const char* packFileName = app::ObjUtil::GetStagePackName(gameDocument);
    if (strncmp(packFileName, "zdlc02", 6) == 0)
    {
        int coinModel = 0;

        int packFile;
        app::ObjUtil::GetPackFile(&packFile, app::ObjUtil::GetStagePackName(gameDocument));
        app::ObjUtil::GetModelResource(&coinModel, "zdlc02_obj_yoshicoin", &packFile);

        if (coinModel)
            *(int*)(This + 0x10) = coinModel;
    }

    return result;
}

void app::CSharedObjInfo::Initialize()
{
    INSTALL_HOOK(CSharedObjInfoInitialize);
}
