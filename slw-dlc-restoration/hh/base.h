#pragma once

namespace hh
{
    namespace base
    {
        class CRefCountObject
        {
        public:
            int RefCount{};

            virtual void Destructor(size_t deletingFlags) {}
        };
        
        inline static FUNCTION_PTR(int*, __cdecl, GetUserMemoryAllocator, ASLR(0x00C1E3B0), int* a1);
    }
}