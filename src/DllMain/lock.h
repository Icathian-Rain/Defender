#pragma once

#include<Windows.h>
#include<unordered_set>

std::unordered_set<HANDLE> handle_set;

static long Lock = 0;

void initLock()
{
    Lock = TlsAlloc();
    TlsSetValue((long)Lock, (LPVOID)0);
}

void setLock()
{
    TlsSetValue(Lock, (LPVOID)1);
}

void releaseLock()
{
    TlsSetValue(Lock, (LPVOID)0);
}

bool isLocked()
{
    return (long)TlsGetValue(Lock) == 0 ? false : true;
}
