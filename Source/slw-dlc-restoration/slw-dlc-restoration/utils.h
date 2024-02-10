#pragma once
#include "include/detours.h"

#define HOOK(returnType, callingConvention, functionName, location, ...) \
    typedef returnType callingConvention functionName(__VA_ARGS__); \
    functionName* original##functionName = (functionName*)(location); \
    returnType callingConvention implOf##functionName(__VA_ARGS__)

#define INSTALL_HOOK(functionName) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourAttach((void**)&original##functionName, implOf##functionName); \
        DetourTransactionCommit(); \
    }

#define WRITE_MEMORY(location, type, ...) \
    { \
        const type data[] = { __VA_ARGS__ }; \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
        memcpy((void*)(location), data, sizeof(data)); \
        VirtualProtect((void*)(location), sizeof(data), oldProtect, NULL); \
    }

#define WRITE_NOP(location, count) \
    { \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
        for (size_t i = 0; i < (size_t)(count); i++) \
            *((uint8_t*)(location) + i) = 0x90; \
        VirtualProtect((void*)(location), (size_t)(count), oldProtect, &oldProtect); \
    }

#define WRITE_FUNCTION(location, func)\
    { \
        DWORD oldProtect; \
        void* address = reinterpret_cast<void*>(func); \
        VirtualProtect(reinterpret_cast<void*>(location), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); \
        memcpy(reinterpret_cast<void*>(location), &address, sizeof(void*)); \
        VirtualProtect(reinterpret_cast<void*>(location), sizeof(void*), oldProtect, NULL); \
    }

#define WRITE_CALL(location, function) \
    { \
        WRITE_MEMORY(location, uint8_t, 0xE8); \
        WRITE_MEMORY(location + 1, uint32_t, (uint32_t)(function) - (size_t)(location) - 5); \
    }

#define WRITE_OBJECT_CLASS_CREATE(location, objectCls)\
    { \
        if (objectCls.fpCreateInfo) \
        { \
            WRITE_FUNCTION(location + 47, *(void**)&objectCls.fpCreateInfo); \
            if (objectCls.fpCreateObject) \
            { \
                WRITE_FUNCTION(location + 52, *(void**)&objectCls.fpCreateObject); \
            } \
        } \
        else \
        { \
            if (objectCls.fpCreateObject) \
            { \
                WRITE_FUNCTION(location + 49, *(void**)&objectCls.fpCreateObject); \
            } \
        } \
    }