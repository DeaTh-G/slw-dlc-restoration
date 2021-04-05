#pragma once

namespace app
{
    class GameMode
    {
    public:
        inline static FUNCTION_PTR(int, __stdcall, FadeIn, ASLR(0x004AC5C0), float a1, int a2);
        inline static FUNCTION_PTR(bool, __stdcall, LoadFile, ASLR(0x004AC840), const char* fileName, fnd::FileLoaderParam* loader);
    };
}