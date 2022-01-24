#pragma once

namespace app
{
    namespace animation
    {
        template <class T>
        class AnimCallbackBridge
        {
        public:
            int field_04;
            short field_06;
            short Allocator;
            T* GameObject;
            void* field_10;
            int field_14;

        private:
            virtual ~AnimCallbackBridge() {};
            virtual int Callback(int a1, int a2, int a3) { return f_Callback(this, a1, a2, a3); };

            inline static FUNCTION_PTR(int, __thiscall, f_Callback, ASLR(0x006F4EB0), void* This, int a2, int a3, int a4);
        };
    }
}