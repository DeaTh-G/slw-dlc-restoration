#pragma once

namespace app
{
    namespace game
    {
        class GravityManager
        {
        private:
            static void* staticClass() { return (void*)ASLR(0x00FD7C28); }
            inline static FUNCTION_PTR(int*, __thiscall, f_GetObjectAtPoint, ASLR(0x004CE5E0), GravityManager* This, int* a2);

        public:
            static GravityManager* GetService(GameDocument* gameDocument)
            {
                void* managerClass = staticClass();
                GravityManager* service = (GravityManager*)GameDocument::GetServiceByClass(gameDocument, managerClass);
                return service;
            }

            int* GetObjectAtPoint(int* a2) { return f_GetObjectAtPoint(this, a2); }
        };
    }
}