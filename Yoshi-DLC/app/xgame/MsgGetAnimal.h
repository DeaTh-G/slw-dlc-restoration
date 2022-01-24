#pragma once

namespace app
{
    namespace xgame
    {
        class MsgGetAnimal : public fnd::Message
        {
        public:
            int Count{};

            MsgGetAnimal() : Message()
            {
                Type = fnd::PROC_MSG_GET_ANIMAL;
            }

            MsgGetAnimal(int count) : MsgGetAnimal()
            {
                Count = count;
            }
        };
    }
}