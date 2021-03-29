#pragma once

namespace app
{
    namespace game
    {
        class MoveController
        {
        public:
            int field_04;
            int field_08;
            int field_0C;

        private:
            inline static FUNCTION_PTR(MoveController*, __thiscall, __dt, ASLR(0x004C3D50), MoveController* This, int a2);
            
        protected:
            virtual void* GetRuntimeTypeInfo() { return (void*)ASLR(0x00FD7BF8); }
            virtual int Update(const fnd::SUpdateInfo& updateInfo) { return 0; };
            virtual int OnEnter() { return 0; };
            virtual int OnLeave() { return 0; };

        public:
            inline static FUNCTION_PTR(MoveController*, __thiscall, ResetFlag, ASLR(0x004C3DE0), MoveController* This, int a2);
            
            virtual ~MoveController()
            {
                __dt(this, 0);
                FORCE_RET;
            }
        };
    }
}