#pragma once

namespace app
{
    namespace xgame
    {
        class MsgTakeObject : public fnd::Message
        {
        public:
            enum class EType : int {
                RING,
                RING_2,
                SUPER_RING,
                ONE_UP,
                RED_RING,
                SPEED_UP,
                SLOW_DOWN,
                INVINCIBILITY,
                WARP,
                BARRIER,
                WATER_BARRIER,
                THUNDER_BARRIER,
                RED_RING_RADAR,
                UNKNOWN_1,
                UNKNOWN_2,
                PHANTOM_BOMB,
                YOSHI_ONE_UP,
                YOSHI_COIN,
                ZELDA_ONE_UP
            };

        private:
            inline static FUNCTION_PTR(void, __thiscall, f_SetShapeUserID, ASLR(0x00536700), MsgTakeObject* This, char a1);

        public:
            EType TakeType;
            int field_1C;
            char field_20;
            char field_21;
            short field_22;
            int ShapeID;

            MsgTakeObject(EType type) : Message()
            {
                Type = fnd::PROC_MSG_TAKE_OBJECT;
                TakeType = type;
                field_1C = -1;
                field_20 = 0;
                field_21 = 0;
                field_22 = 0;
                ShapeID = 0;
            }

            void SetShapeUserID(int a1) { f_SetShapeUserID(this, a1); }
            int IsValidUserID() { return field_21; }
        };
    }
}