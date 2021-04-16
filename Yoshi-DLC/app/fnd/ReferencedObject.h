#pragma once

namespace app
{
    namespace fnd
    {
        class ReferencedObject : public BaseObject
        {
        private:
            inline static FUNCTION_PTR(ReferencedObject*, __cdecl, f_new, ASLR(0x0048D4A0), size_t size, csl::fnd::IAllocator* pAllocator);
            inline static FUNCTION_PTR(void, __cdecl, f_delete, ASLR(0x0048D480), void* obj);

        public:
            short field_04;
            short field_06;
            csl::fnd::IAllocator* pAllocator;

        public:
            void* operator new(size_t size, csl::fnd::IAllocator* pAllocator)
            {
                return f_new(size, pAllocator);
            }

            void operator delete(void* obj)
            {
                f_delete(obj);
            }
        };
    }
}