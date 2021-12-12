#pragma once

namespace app
{
    namespace game
    {
        class alignas(16) MoveCharacterRigidBody : public MoveController
        {
        public:
            struct RangeChecker
            {
                csl::math::Vector3 field_00;
                float field_10;
                float field_14;
                int field_18;
                int field_1C;
            };

            struct Description
            {
                int field_00;
                int field_04;

                Description(int a1, int a2)
                {
                    field_00 = a1;
                    field_04 = a2;
                }
            };

            struct FunctionPair
            {
                void* field_00;
                void* field_04;
            };

            struct unkStruct
            {
                int field_00;
                float field_04;
            };

            int field_10;
            PathEvaluator field_14;
            int field_24;
            int field_28;
            int field_2C;
            MovementUtility::SideviewInterpolateInfo field_30;
            csl::math::Vector3 field_60;
            float field_70;
            float field_74;
            int field_78;
            int field_7C;
            RangeChecker field_80;
            BarrierChecker field_A0;
            FunctionPair field_E0;
            unkStruct field_E8;
            int field_F0;
            FunctionPair field_F4;
            unkStruct field_FC;
            int field_104;
            char field_108;
            char field_109;
            char field_10A;
            char field_10B;
            int field_10C;

        private:
            inline static FUNCTION_PTR(MoveBound*, __thiscall, __ct, ASLR(0x004C6130), MoveCharacterRigidBody* This);
            inline static FUNCTION_PTR(int, __thiscall, f_Update, ASLR(0x004C6BC0), MoveController* This, const fnd::SUpdateInfo& updateInfo);
            inline static FUNCTION_PTR(int, __thiscall, f_OnEnter, ASLR(0x004C6B80), MoveController* This);
            inline static FUNCTION_PTR(MoveBound*, __thiscall, __dt, ASLR(0x004C6090), MoveCharacterRigidBody* This, int a2);

            inline static FUNCTION_PTR(void, __thiscall, f_Setup, ASLR(0x004C61C0), MoveCharacterRigidBody* This, Description* a2);

        protected:
            void* GetRuntimeTypeInfo() override { return (void*)ASLR(0x004C5ED0); }

        public:
            MoveCharacterRigidBody()
            {
                __ct(this);
            }

            int Update(const fnd::SUpdateInfo& updateInfo) override
            {
                return f_Update(this, updateInfo);
            }
            
            int OnEnter() override { return f_OnEnter(this); }
            
            void Destructor(size_t deletingFlags) override
            {
                __dt(this, deletingFlags);
            }

            void Setup(Description* a1)
            {
                f_Setup(this, a1);
            }

            void SetMoveRangeBasePosition(csl::math::Vector3& position)
            {
                field_80.field_00 = position;
                field_109 &= ~(1 << 3);
            }

            void SetMoveRange(float range)
            {
                field_80.field_10 = range * range;
                field_80.field_14 = range * range;
            }

            void SetNotifyMoveRangeOutCallback(FunctionPair a2, unkStruct a3, int a4)
            {
                field_F4 = a2;
                field_FC = a3;
                field_104 = a4;
            }
        };
    }
}