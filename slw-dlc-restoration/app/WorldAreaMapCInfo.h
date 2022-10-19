#pragma once

namespace app
{
    class WorldAreaMapCInfo
    {
    public:
        class Event
        {
        public:
            int m_Unk1{};
            int m_Unk2{};
            int m_Unk3{};

            Event(int a1, int a2, int a3)
            {
                m_Unk1 = a1;
                m_Unk2 = a2;
                m_Unk3 = a3;
            }
        };

    private:
        inline static FUNCTION_PTR(int, __thiscall, ms_fpAddEvent, ASLR(0x009479C0), WorldAreaMapCInfo*, Event*);

    public:
        void AddEvent (Event* in_event) { ms_fpAddEvent(this, in_event); }
    };
}