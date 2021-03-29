#pragma once

namespace app
{
    class EnemyManager
    {
    public:
        inline static FUNCTION_PTR(void*, __cdecl, staticClass, ASLR(0x00728500));
        inline static FUNCTION_PTR(void, __thiscall, CreateDeadEffect, ASLR(0x00728630), void* enemyManager, enemy::DeadEffectCInfo* effectInfo);

        static void* GetService(GameDocument* gameDocument)
        {
            void* managerClass = staticClass();
            void* service = GameDocument::GetServiceByClass(gameDocument, managerClass);
            return service;
        }
    };
}