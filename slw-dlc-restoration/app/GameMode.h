#pragma once

namespace app
{
    class GameMode : public fnd::ReferencedObject
    {
    protected:
        void* m_pGame{};
        void* m_Unk1{};
        const char* m_pName{};
        GameDocument* m_pDocument{};
        void* m_Unk2{};

    private:
        inline static FUNCTION_PTR(int, __thiscall, f_SendMessageImm, ASLR(0x004AC510), GameMode* This, int a2, fnd::Message* msg);
		inline static FUNCTION_PTR(void, __thiscall, ms_fpOnEnter, ASLR(0x004AC800), GameMode*, void*);
		inline static FUNCTION_PTR(void, __thiscall, ms_fpOnLeave, ASLR(0x004AC820), GameMode*, void*);
		inline static FUNCTION_PTR(void, __thiscall, ms_fpDefaultGameLoop, ASLR(0x004AC9B0), GameMode*, const fnd::SUpdateInfo&);

    public:
        inline static FUNCTION_PTR(int, __stdcall, FadeIn, ASLR(0x004AC5C0), float a1, int a2);
        inline static FUNCTION_PTR(int, __stdcall, WhiteOut, ASLR(0x004AC590), float a1, int a2);
        inline static FUNCTION_PTR(bool, __stdcall, IsFadeFinished, ASLR(0x004AC5F0), int a2);
        inline static FUNCTION_PTR(bool, __stdcall, LoadFile, ASLR(0x004AC840), const char* fileName, fnd::FileLoaderParam* loader);
    
        int SendMessageImm(int a2, fnd::Message* msg) { return f_SendMessageImm(this, a2, msg); }
    };
}