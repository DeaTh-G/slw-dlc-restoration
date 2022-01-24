#pragma once

namespace app
{
    namespace fnd
    {
        class BaseObject
        {
        public:
            virtual void Destructor(size_t deletingFlags) {};
        };
    }
}