#pragma once

namespace app
{
    namespace fnd
    {
        class FileLoaderParam
        {
        public:
            int field_00;
            int field_04;
            int field_08;
            int field_0C;
            int field_10;
            int field_14;

        private:
            inline static FUNCTION_PTR(FileLoaderParam*, __thiscall, __ct, ASLR(0x00490290), FileLoaderParam* This);
            inline static FUNCTION_PTR(int, __thiscall, f_SetMultiLanguageAttr, ASLR(0x004902B0), FileLoaderParam* This, bool a2, int a3);

        public:
            FileLoaderParam() { __ct(this); }
            int SetMultiLanguageAttr(bool a2, int a3) { return f_SetMultiLanguageAttr(this, a2, a3); }
        };

        class FileLoader
        {
        public:
            inline static FUNCTION_PTR(void, __thiscall, LoadFile, ASLR(0x00490D90), FileLoader* This, int* a2, const char* fileName, FileLoaderParam* loaderParam);
        };

    }
}