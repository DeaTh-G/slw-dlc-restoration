#include "pch.h"
#include "LostCodeLoader.h"

extern "C"
{
    void _declspec(dllexport) __cdecl Init(ModInfo* modInfo)
    {
        LoadLibraryA("nvapi.dll");
    }
}