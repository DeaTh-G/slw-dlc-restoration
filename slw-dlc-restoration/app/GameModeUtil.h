#pragma once

/* Hippity hoppity, this code is now my property
In seriousness, this code belongs to https://github.com/Radfordhound/OpenLW/blob/master/Sonic2010/build/main/src/System/GameMode/GameMode.h#L18 */

namespace app
{
    namespace GameModeUtil // TODO: Is this actually a class?
    {
        template<typename T>
        struct Event : public TTinyFsmEvent<T, Event<T>>
        {
            union
            {
                int m_integer;
                float m_float;
                fnd::Message* m_message;
                // TODO: m_object??
            };

            // TODO: Is this constructor actually a thing?
            inline Event(int signal) :
                TTinyFsmEvent<T, Event<T>>(signal) {}

            // TODO: Is this constructor actually a thing?
            inline Event(int signal, float deltaTime) :
                TTinyFsmEvent<T, Event<T>>(signal), m_float(deltaTime) {}

            // TODO: Is this constructor actually a thing?
            inline Event(int signal, fnd::Message& msg) :
                TTinyFsmEvent<T, Event<T>>(signal), m_message(&msg) {}

        public:
            typedef Event<T> this_t;

            // TODO: Is this function actually a thing?
            inline int getInt() const
            {
                return m_integer;
            }

            // TODO: Is this function actually a thing?
            inline float getDeltaTime() const
            {
                return m_float;
            }

            // TODO: Is this function actually a thing?
            inline fnd::Message& getMessage() const
            {
                return *m_message;
            }

            // TODO: Is this function actually a thing?
            inline static this_t CreateSignal(int signal)
            {
                return this_t(signal);
            }

            // TODO: Is this function actually a thing?
            inline static this_t CreateSuper()
            {
                return CreateSignal(SIGNAL_SUPER);
            }

            // TODO: Is this function actually a thing?
            inline static this_t CreateInit()
            {
                return CreateSignal(SIGNAL_INIT);
            }

            // TODO: Is this function actually a thing?
            inline static this_t CreateEnter()
            {
                return CreateSignal(SIGNAL_ENTER);
            }

            // TODO: Is this function actually a thing?
            inline static this_t CreateLeave()
            {
                return CreateSignal(SIGNAL_LEAVE);
            }

            inline static this_t CreateUpdate(float deltaTime)
            {
                return this_t(SIGNAL_UPDATE, deltaTime);
            }

            inline static this_t CreateMessage(fnd::Message& msg)
            {
                return this_t(SIGNAL_MESSAGE, msg);
            }
        };
    }
}