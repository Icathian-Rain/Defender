
#pragma once
#include <atlstr.h>
#include "lock.h"
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

extern udp::UdpClient client;

// HeapCreate
static HANDLE(WINAPI *OldHeapCreate)(
    DWORD fIOoptions,     // 堆操作类型
    SIZE_T dwInitialSize, // 开始时分给堆的字节数
    SIZE_T dwMaximumSize  // 堆最大能增长到的字节数
    ) = HeapCreate;

extern "C" __declspec(dllexport) HANDLE WINAPI NewHeapCreate(
    DWORD fIOoptions,     // 堆操作类型
    SIZE_T dwInitialSize, // 开始时分给堆的字节数
    SIZE_T dwMaximumSize  // 堆最大OldHeapFree能增长到的字节数
)
{
    // heap操作上锁
    setLock();
    HANDLE ret = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    handle_set.insert(ret);
    Msg msg("HeapCreate");
    msg.setItem("fIOoptions", dword2hex(fIOoptions));
    msg.setItem("dwInitialSize", std::to_string(dwInitialSize));
    msg.setItem("dwMaximumSize", std::to_string(dwMaximumSize));
    msg.setItem("ret", dword2hex((DWORD)ret));
    client.send(msg.getMsg().c_str());
    // heap操作解锁
    releaseLock();
    return ret;
}

// HeapDestroy
static BOOL(WINAPI *OldHeapDestroy)(HANDLE) = HeapDestroy;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestory(
    HANDLE hHeap // 要销毁堆的句柄
)
{
    // heap操作上锁
    setLock();
    BOOL ret = OldHeapDestroy(hHeap);
    Msg msg("HeapDestroy");
    msg.setItem("hHeap", std::to_string((int)hHeap));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    // heap操作解锁
    releaseLock();
    return ret;
}

// HeapAlloc
static LPVOID(WINAPI *
OldHeapAlloc)(
    HANDLE hHeap,
    DWORD dwFlags,
    SIZE_T dwBytes) = HeapAlloc;
extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(
    HANDLE hHeap,  // 分配堆的句柄
    DWORD dwFlags, // 分配选项
    SIZE_T dwBytes //	分配大小
)
{
    LPVOID ret = OldHeapAlloc(hHeap, dwFlags, dwBytes);
    if (isLocked() == false && handle_set.find(hHeap) != handle_set.end())
    {
        setLock();
        Msg msg("HeapAlloc");
        msg.setItem("hHeap", dword2hex((DWORD)hHeap));
        msg.setItem("dwFlags", dword2hex(dwFlags));
        msg.setItem("dwBytes", std::to_string(dwBytes));
        msg.setItem("ret", dword2hex((DWORD)ret));
        client.send(msg.getMsg().c_str());
        // heap操作解锁
        releaseLock();
    }   
    return ret;
}

// HeapFree
static BOOL(WINAPI *OldHeapFree)(
    HANDLE hHeap,
    DWORD dwFlags,
    _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(
    HANDLE hHeap,                // 要释放堆的句柄
    DWORD dwFlags,               // 堆释放选项
    _Frees_ptr_opt_ LPVOID lpMem // 指向要释放的内存块的指针
)
{
    BOOL ret = OldHeapFree(hHeap, dwFlags, lpMem);
    if (isLocked() == false && handle_set.find(hHeap) != handle_set.end())
    {
        setLock();
        Msg msg("HeapFree");
        msg.setItem("hHeap", dword2hex((DWORD)hHeap));
        msg.setItem("dwFlags", std::to_string(dwFlags));
        msg.setItem("lpMem", dword2hex((DWORD)lpMem));
        msg.setItem("ret", std::to_string(ret));
        client.send(msg.getMsg().c_str());
        releaseLock();
    }
    return ret;
}
