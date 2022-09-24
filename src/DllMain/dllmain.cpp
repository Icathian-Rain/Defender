// Headers
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include <atlstr.h>

#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

// Hook List
#include "hookMessagebox.h"
#include "hookFile.h"
#include "hookHeap.h"
#include "hookReg.h"
#include "hookNet.h"
#include "hookMemory.h"

// lib
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")


udp::UdpClient client("127.0.0.1", 4000);

/// @brief 发送当前进程的ID，时间
void sendProcessInfo()
{
    std::string msg = "{\n \"processID\": " + std::to_string(GetCurrentProcessId()) + ",\n";
    msg += " \"time\": \"" + getCurrentTime() + "\"\n}";
    client.send(msg.c_str());
}

BOOL WINAPI DllMain(HMODULE hModule,
                    DWORD ul_reason_for_call,
                    LPVOID lpReserved)
{
    initLock();
    setLock();
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        sendProcessInfo();
        DetourAttach(&(PVOID &)OldMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID &)OldMessageBoxW, NewMessageBoxW);

        DetourAttach(&(PVOID &)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID &)OldWriteFile, NewWriteFile);
        DetourAttach(&(PVOID &)OldReadFile, NewReadFile);

        DetourAttach(&(PVOID &)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID &)OldHeapDestroy, NewHeapDestory);
        DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

        DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourAttach(&(PVOID&)OldRegQueryValueEx, NewRegQueryValueEx);
        DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourAttach(&(PVOID&)OldRegDeleteKey, NewRegDeleteKey);
        DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);

        DetourAttach(&(PVOID &)Oldsocket, Newsocket);
        DetourAttach(&(PVOID &)Oldbind, Newbind);
        DetourAttach(&(PVOID &)Oldlisten, Newlisten);
        DetourAttach(&(PVOID &)Oldaccept, Newaccept);
        DetourAttach(&(PVOID &)Oldconnect, Newconnect);
        DetourAttach(&(PVOID &)Oldsend, Newsend);
        DetourAttach(&(PVOID &)Oldrecv, Newrecv);
        DetourAttach(&(PVOID&)Oldsendto, Newsendto);
        DetourAttach(&(PVOID&)Oldrecvfrom, Newrecvfrom);
        DetourAttach(&(PVOID&)Oldmemcpy, Newmemcpy);
        DetourTransactionCommit();
        releaseLock();
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        // DetourDetach(&(PVOID &)OldMessageBoxA, NewMessageBoxA);
        // DetourDetach(&(PVOID &)OldMessageBoxW, NewMessageBoxW);

        // DetourDetach(&(PVOID &)OldCreateFile, NewCreateFile);
        // DetourDetach(&(PVOID &)OldWriteFile, NewWriteFile);
        // DetourDetach(&(PVOID &)OldReadFile, NewReadFile);

        // DetourDetach(&(PVOID &)OldHeapCreate, NewHeapCreate);
        // DetourDetach(&(PVOID &)OldHeapDestory, NewHeapDestory);
        // DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        // DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);

        // DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        // DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        // DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        // DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        // DetourDetach(&(PVOID&)OldRegDeleteKey, NewRegDeleteKey);
        // DetourDetach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);

        // DetourDetach(&(PVOID &)Oldsocket, Newsocket);
        // DetourDetach(&(PVOID &)Oldbind, Newbind);
        // DetourDetach(&(PVOID &)Oldlisten, Newlisten);
        // DetourDetach(&(PVOID &)Oldaccept, Newaccept);
        // DetourDetach(&(PVOID &)Oldconnect, Newconnect);
        // DetourDetach(&(PVOID &)Oldsend, Newsend);
        // DetourDetach(&(PVOID &)Oldrecv, Newrecv);
        // DetourDetach(&(PVOID&)Oldsendto, Newsendto);
        // DetourDetach(&(PVOID&)Oldrecvfrom, Newrecvfrom);
        // DetourDetach(&(PVOID&)Oldmemcpy, Newmemcpy);
        DetourTransactionCommit();
        break;
    }
    }
    return true;
}