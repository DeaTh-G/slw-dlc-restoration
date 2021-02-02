#pragma once

class InfoFileLoader
{
public:
	virtual int __stdcall SetMemory(char* name, int a2);
	virtual int* __stdcall LoadFile(char* name, int a2);
};