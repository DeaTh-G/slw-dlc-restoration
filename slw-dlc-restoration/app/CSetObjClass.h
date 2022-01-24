#pragma once

namespace app
{
    inline static FUNCTION_PTR(void*, __cdecl, AddParamFloat, ASLR(0x00615F2D),
        void* a1, const char* paramName, const char* paramDesc, int byteOffset, float a5, float a6, float a7, float a8);

    class CSetObjClass
    {
    private:
        inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x00843EC0),
            void* This, const char* objName, const char* objType, void* ctor, void* infoCtor,
            void* paramMap, int a7, int a8, const char* categoryName, int a10, float defaultRangeIn, float defaultRangeOut);

    public:
        CSetObjClass(const char* objName, const char* objType, void* ctor, void* infoCtor, void* paramMap,
            int a7, int a8, const char* categoryName, int a10, float defaultRangeIn, float defaultRangeOut)
        {
            __ct(this, objName, objType, ctor, infoCtor, paramMap, a7, a8, categoryName, a10, defaultRangeIn, defaultRangeOut);
        }
    };
}