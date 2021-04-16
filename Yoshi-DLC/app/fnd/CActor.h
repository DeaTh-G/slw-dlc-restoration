#pragma once

namespace app
{
    namespace fnd
    {
        struct SUpdateInfo
        {
            float deltaTime;
            unsigned int frame;
        };

        class CActor : public csl::ut::NonCopyable
        {
        private:
            inline static FUNCTION_PTR(CActor*, __thiscall, __dt, ASLR(0x0049A520), CActor* This, size_t a2);
            inline static FUNCTION_PTR(char, __thiscall, f_SendMessageImm, ASLR(0x0049A470), CActor* This, int a2, fnd::Message* msg);

        public:
            int field_04[6]{};

            virtual ~CActor() { __dt(this, 0); }

            char SendMessageImm(int actorID, fnd::Message* msg) { return f_SendMessageImm(this, actorID, msg); }

            virtual size_t ForEach(int& traverser) = 0;
        
        protected:
            virtual bool PreProcessMessage(Message& message) { return false; }
            virtual bool ProcessMessage(Message& message) { return PreProcessMessage(message); }
            virtual void Update(const SUpdateInfo& updateInfo) { };
            virtual bool ActorProc(int id, void* data) = 0;
        };
    }
}
