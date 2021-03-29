#pragma once

namespace csl
{
    namespace fnd
    {
        class IAllocator
        {
        public:			
            virtual ~IAllocator() = default;

            virtual void* Alloc(size_t size, int alignment) = 0;
            virtual void Free(void* loc) = 0;
        };
    }
}