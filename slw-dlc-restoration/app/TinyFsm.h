#pragma once
#include <cstddef>

/* Hippity hoppity, this code is now my property
In seriousness, this code belongs to https://github.com/Radfordhound/OpenLW/blob/master/Sonic2010/build/main/src/OpenLW/TinyFsm.h*/

namespace app
{
    namespace fnd
    {
        struct Message;
    }

    template<typename T, typename _event_t>
    class TTinyFsmEvent
    {
        int m_sig;

    public:
        typedef _event_t event_t;

        int getSignal() const
        {
            return m_sig;
        }

        // TODO: Is this constructor actually a thing?
        inline TTinyFsmEvent(int signal) : m_sig(signal) {}
    };

    enum TiFsmBasicEventSignal // TODO: This name was guessed. Is it good?
    {
        // TODO: These names were guessed. Are they good?

        SIGNAL_SUPER = -1,
        SIGNAL_INIT = -2,
        SIGNAL_ENTER = -3,
        SIGNAL_LEAVE = -4,

        SIGNAL_UPDATE = 0,
        SIGNAL_MESSAGE = 1
    };

    template<typename T>
    class TiFsmBasicEvent : public TTinyFsmEvent<T, TiFsmBasicEvent<T>>
    {
        union
        {
            int m_integer;
            float m_float;
            fnd::Message* m_message;
            // TODO: m_object??
        };

        // TODO: Is this constructor actually a thing?
        inline TiFsmBasicEvent(int signal) :
            TTinyFsmEvent<T, TiFsmBasicEvent<T>>(signal) {}

        // TODO: Is this constructor actually a thing?
        inline TiFsmBasicEvent(int signal, float deltaTime) :
            TTinyFsmEvent<T, TiFsmBasicEvent<T>>(signal), m_float(deltaTime) {}

        // TODO: Is this constructor actually a thing?
        inline TiFsmBasicEvent(int signal, fnd::Message& msg) :
            TTinyFsmEvent<T, TiFsmBasicEvent<T>>(signal), m_message(&msg) {}

    public:
        typedef TiFsmBasicEvent<T> this_t;

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

    template<typename T, typename _event_t = TiFsmBasicEvent<T>, bool hierarchical = true>
    class TTinyFsm;

    template<typename T, typename event_t = TiFsmBasicEvent<T>>
    class TTinyFsmState
    {
    public:
        typedef TTinyFsmState<T, event_t> this_t;
        typedef this_t(T::* event_func)(const event_t& e);

    private:
        event_func m_delegate;

    public:
        // TODO: Is this function actually a thing?
        inline bool IsValid() const
        {
            return (m_delegate != nullptr);
        }

        // TODO: Is this function actually a thing?
        inline void Clear()
        {
            m_delegate = nullptr;
        }

        // TODO: Is this function actually a thing?
        inline this_t Call(T* obj, const event_t& e)
        {
            return (obj->*m_delegate)(e);
        }

        // TODO: Is this function actually a thing?
        inline bool operator==(const this_t& other) const
        {
            return (m_delegate == other.m_delegate);
        }

        // TODO: Is this function actually a thing?
        inline bool operator!=(const this_t& other) const
        {
            return (m_delegate != other.m_delegate);
        }

        // TODO: Is this function actually a thing?
        inline bool operator==(const event_func& eventFunc) const
        {
            return (m_delegate == eventFunc);
        }

        // TODO: Is this function actually a thing?
        inline bool operator!=(const event_func& eventFunc) const
        {
            return (m_delegate != eventFunc);
        }

        inline TTinyFsmState() :
            m_delegate(nullptr) {}

        // TODO: Is this constructor actually a thing?
        inline TTinyFsmState(event_func eventFunc) :
            m_delegate(eventFunc) {}
    };

    template<typename T, typename _event_t, bool hierarchical>
    class TTinyFsm
    {
    public:
        typedef TTinyFsm<T, _event_t, hierarchical> this_t;
        typedef TTinyFsmState<T, _event_t> state_t;
        typedef _event_t event_t;

    private:
        // TODO: I might have gotten the m_cur and m_src names mixed up actually lol.
        // Regardless this is functionally still accurate to what LW actually does.
        state_t m_cur;
        state_t m_src;

        // TODO: Is this function actually a thing?
        inline state_t Trigger(state_t state, const event_t& e)
        {
            return state.Call(static_cast<T*>(this), e);
        }

        // TODO: Is this function actually a thing?
        inline state_t Super(state_t state)
        {
            return Trigger(state, event_t::CreateSuper());
        }

        // TODO: Is this function actually a thing?
        inline state_t Init(state_t state)
        {
            return Trigger(state, event_t::CreateInit());
        }

        // TODO: Is this function actually a thing?
        inline void Enter(state_t state)
        {
            Trigger(state, event_t::CreateEnter());
        }

        // TODO: Is this function actually a thing?
        inline void Leave(state_t state)
        {
            Trigger(state, event_t::CreateLeave());
        }

        state_t _top(const event_t& event) // TODO: Is this actually defined by the user of this class?
        {
            return state_t(); // TODO: Is this correct?
        }

        // TODO: Is this function actually a thing? It appears to
        // always be inlined in the Wii U version but is present on
        // PC, so we have no symbols for it.
        inline void UnknownFunc()
        {
            while (Init(m_src) == nullptr)
            {
                Enter(m_src);
            }
        }

    public:
        state_t FSM_TOP() const
        {
            return state_t(&this_t::_top); // TODO: Is this correct?
        }

        void FSM_INIT(state_t initialState)
        {
            m_src = initialState;
        }

        void FSM_TRAN(state_t state)
        {
            if (hierarchical)
            {
                for (state_t tinyFsmState = m_cur; tinyFsmState != m_src;
                    tinyFsmState = Super(tinyFsmState))
                {
                    Leave(tinyFsmState);
                }

                if (m_cur == state)
                {
                    Leave(m_cur);
                    Enter(state);
                }
                else
                {
                    // TODO: It seems the "stateStack" stuff is actually, like, a csl implace move array or something??

                    // Call the current state and all of its base states.
                    state_t stateStack[5];
                    state_t tinyFsmState = m_cur;
                    int i = 0;

                    while (tinyFsmState != FSM_TOP())
                    {
                        stateStack[i++] = tinyFsmState;
                        tinyFsmState = Super(tinyFsmState);
                    }

                    // Call the new state and all of its base states.
                    state_t stateStack2[5];
                    state_t tinyFsmState2 = state;
                    int i2 = 0;

                    while (tinyFsmState2 != FSM_TOP())
                    {
                        stateStack2[i2++] = tinyFsmState2;
                        tinyFsmState2 = Super(tinyFsmState2);
                    }

                    state_t* firstUnique1 = &stateStack[i];
                    state_t* firstUnique2 = &stateStack2[i2];

                    while (firstUnique1 != stateStack && firstUnique2 != stateStack2 &&
                        *firstUnique1 == nullptr || firstUnique1 == firstUnique2)
                    {
                        --firstUnique1;
                        --firstUnique2;
                    }

                    state_t* curPtr = stateStack;
                    while (curPtr != firstUnique1)
                    {
                        Leave(*(curPtr++));
                    }

                    while (firstUnique2 != stateStack2)
                    {
                        Enter(*(firstUnique2--));
                    }

                    // TODO: Destructor of the implace move array or whatever goes here.
                }

                m_src = state;
                UnknownFunc();
            }
            else
            {
                Leave(m_src);
                Enter(state);
                m_src = state;
            }
        }

        void FSM_SETSTATE(state_t state)
        {
            m_cur = m_src;
            FSM_TRAN(state);
        }

        void InitFSM()
        {
            if (hierarchical)
            {
                // Call super event on all source states.
                state_t stateStack[5];
                state_t tinyFsmState = m_src;
                int i = 0;

                while (tinyFsmState != FSM_TOP())
                {
                    stateStack[i++] = tinyFsmState;
                    tinyFsmState = Super(tinyFsmState);
                }

                // Call enter event on all source states in reverse order.
                state_t* curPtr = &stateStack[i];
                while (curPtr-- != stateStack)
                {
                    Enter(*curPtr);
                }

                UnknownFunc();
            }
            else
            {
                Enter(m_src);
            }
        }

        void DispatchFSM(const event_t& e)
        {
            if (hierarchical)
            {
                m_cur = m_src;
                while (m_cur.IsValid())
                {
                    state_t tinyFsmState = Trigger(m_cur, e);
                    if (!tinyFsmState.IsValid()) break;

                    m_cur = Super(m_cur);
                }
            }
            else
            {
                Trigger(m_src, e);
            }
        }

        void ShutdownFSM()
        {
            if (hierarchical)
            {
                // TODO (Check 0x02a7f190 to implement this)
                __debugbreak();
            }
            else
            {
                if (m_src != FSM_TOP())
                {
                    Leave(m_src);
                    m_src = FSM_TOP();
                }
            }
        }

        TTinyFsm(state_t param_1) :
            m_cur(FSM_TOP()),
            m_src(param_1) {}

        virtual ~TTinyFsm()
        {
            // TODO: Does the "default" virtual destructor have a body?
        }
    };
}
