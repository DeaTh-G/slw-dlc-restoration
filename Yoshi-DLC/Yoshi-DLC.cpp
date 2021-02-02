// LostMods.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Yoshi-DLC.h"

// This is an example of an exported variable
YOSHIDLC_API int nYoshiDLC=0;

// This is an example of an exported function.
YOSHIDLC_API int fnYoshiDLC(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CYoshiDLC::CYoshiDLC()
{
    return;
}