#include "pch.h"
#include "PrizeData.h"

void slw_dlc_restoration::PrizeData::InstallPatches()
{
    // General Patches
    // Change the reward type value in PrizeData::SetRadicon() to 2 so that its treated as a radicon reward.
    // (This function sets this value to 0 on both Wii U and PC. I am not yet sure why the radicon rewards don't work without this patch.)
    WRITE_MEMORY(ASLR(0x00952B8E), byte, 2);
}
