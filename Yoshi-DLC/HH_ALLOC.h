#pragma once

static inline FUNCTION_PTR(void*, __cdecl, f_HH_ALLOC, ASLR(0x00C1B130), size_t size);
static inline FUNCTION_PTR(void*, __cdecl, f_HH_ALLOCALIGN, ASLR(0x00C1B230), size_t size, size_t alignment);
static inline FUNCTION_PTR(void, __cdecl, f_HH_FREE, ASLR(0x00C1B330), void* mem);

static void* __HH_ALLOC(size_t size)
{
	return f_HH_ALLOC(size);
}

static void* __HH_ALLOCALIGN(size_t size, size_t alignment)
{
	return f_HH_ALLOCALIGN(size, alignment);
}

static void __HH_FREE(void* mem)
{
	f_HH_FREE(mem);
}