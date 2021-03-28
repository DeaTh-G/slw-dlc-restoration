// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LOSTMODS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LOSTMODS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef YOSHIDLC_EXPORTS
#define YOSHIDLC_API __declspec(dllexport)
#else
#define YOSHIDLC_API __declspec(dllimport)
#endif

// This class is exported from the dll
class YOSHIDLC_API CYoshiDLC {
public:
    CYoshiDLC(void);
    // TODO: add your methods here.
};

extern YOSHIDLC_API int nYoshiDLC;

YOSHIDLC_API int fnYoshiDLC(void);