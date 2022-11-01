#include "pch.h"

void app::worldmap::CWorldMapStateModeSelect::Constructor()
{
    WRITE_MEMORY_WITH_TYPE(ASLR(0x00951092), uint32_t, 5);
}

void app::worldmap::CWorldMapStateModeSelect::AddCallback()
{
    WRITE_MEMORY_WITH_TYPE(ASLR(0x00951341), uint32_t, 6);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x00951272), uint32_t, (uint32_t)&m_Positions[1]);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x009512B8), uint32_t, (uint32_t)&m_Positions[sizeof(m_Positions) / sizeof(m_Positions[0])]);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x009513A8), uint32_t, (uint32_t)&m_IconNames[2]);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x00951437), uint32_t, (uint32_t)&m_IconNames[sizeof(m_IconNames) / sizeof(m_IconNames[0])]);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x009512D7), uint32_t, 6);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x0095144A), uint32_t, (uint32_t)&m_IconNames[2]);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x009514C4), uint32_t, (uint32_t)&m_IconNames[sizeof(m_IconNames) / sizeof(m_IconNames[0])]);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x00951552), uint32_t, 6);
}

void app::worldmap::CWorldMapStateModeSelect::ProcMsgHudKeyUpdate()
{
    WRITE_MEMORY_WITH_TYPE(ASLR(0x009517B5), uint8_t, 6);
    WRITE_MEMORY_WITH_TYPE(ASLR(0x009517D3), uint32_t, 5);

}