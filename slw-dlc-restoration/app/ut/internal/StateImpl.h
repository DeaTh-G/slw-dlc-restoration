#pragma once

namespace app
{
    namespace ut
    {
        namespace internal
        {
            class StateImpl;

            struct StateDescImpl
            {
                const char* Name;
                void* StateCreator;
                int field_08;
            };
        }
    }
}