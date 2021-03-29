#pragma once

namespace app
{
    namespace xgame
    {
        class MsgTakeObject : public fnd::MessageNew
        {
            // Make this private
        public:
            inline static FUNCTION_PTR(void, __thiscall, f_SetShapeUserID, ASLR(0x00536700), MsgTakeObject* This, char a1);

        public:
            int EType;
            int field_1C;
            char field_20;
            char field_21;
            short field_22;
            int ShapeID;

            MsgTakeObject(int a1) : MessageNew()
            {
                Type = fnd::PROC_MSG_TAKE_OBJECT;
                EType = a1;
                field_1C = -1;
                field_20 = 0;
                field_21 = 0;
                field_22 = 0;
                ShapeID = 0;
            }

            void SetShapeUserID(int a1) { f_SetShapeUserID(this, a1); }
        };
    }
}