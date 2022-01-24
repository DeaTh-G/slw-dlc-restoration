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
            inline static FUNCTION_PTR(MoveController*, __thiscall, __ct, ASLR(0x004C3E10), MoveController* This, int a2);
            inline static FUNCTION_PTR(MoveController*, __thiscall, __dt, ASLR(0x004C3D50), MoveController* This, int a2);
            inline static FUNCTION_PTR(GOCMovement*, __thiscall, f_GetOwnerMovement, ASLR(0x0049F980), MoveController* This);
            inline static FUNCTION_PTR(void, __thiscall, f_SetFlag, ASLR(0x004C3DB0), MoveController* This, int flag);
            
        protected:
            virtual void* GetRuntimeTypeInfo() { return (void*)ASLR(0x00FD7BF8); }
            virtual int Update(const fnd::SUpdateInfo& updateInfo) { return 0; };
            virtual int OnEnter() { return 0; };
            virtual int OnLeave() { return 0; };

        public:
            virtual void Destructor(size_t deletingFlags)
            {
                __dt(this, deletingFlags);
            }

            MoveController() { __ct(this, 0); }
            MoveController(int type) { __ct(this, type); }
            GOCMovement* GetOwnerMovement() { return f_GetOwnerMovement(this); }
            void SetFlag(int flag) { return f_SetFlag(this, flag); }

            inline static FUNCTION_PTR(MoveController*, __thiscall, ResetFlag, ASLR(0x004C3DE0), MoveController* This, int a2);
        };
    }
}