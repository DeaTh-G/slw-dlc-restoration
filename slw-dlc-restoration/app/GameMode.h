#pragma once

namespace app
{
    class GameMode
    {
    private:
        inline static FUNCTION_PTR(int, __thiscall, f_SendMessageImm, ASLR(0x004AC510), GameMode* This, int a2, fnd::Message* msg);

    public:
        inline static FUNCTION_PTR(int, __stdcall, FadeIn, ASLR(0x004AC5C0), float a1, int a2);
        inline static FUNCTION_PTR(int, __stdcall, WhiteOut, ASLR(0x004AC590), float a1, int a2);
        inline static FUNCTION_PTR(bool, __stdcall, IsFadeFinished, ASLR(0x004AC5F0), int a2);
        inline static FUNCTION_PTR(bool, __stdcall, LoadFile, ASLR(0x004AC840), const char* fileName, fnd::FileLoaderParam* loader);
    
        int SendMessageImm(int a2, fnd::Message* msg) { return f_SendMessageImm(this, a2, msg); }
    };
}