
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
    // 函数操作
    HANDLE ret = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    // 类型转换
    std::string fIOoptions_tmp = dword2hex(fIOoptions);
    std::string dwInitialSize_tmp = std::to_string(dwInitialSize);
    std::string dwMaximumSize_tmp = std::to_string(dwMaximumSize);
    std::string ret_tmp = dword2hex((DWORD)ret);
    // 插入句柄
    handle_set.insert(ret);
    // 构造消息
    Msg msg("HeapCreate");
    msg.setItem("fIOoptions", fIOoptions_tmp);
    msg.setItem("dwInitialSize", dwInitialSize_tmp);
    msg.setItem("dwMaximumSize", dwMaximumSize_tmp);
    msg.setItem("ret", ret_tmp);
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
    // 函数操作
    BOOL ret = OldHeapDestroy(hHeap);
    // 类型转换
    std::string hHeap_tmp = dword2hex((DWORD)hHeap);
    std::string ret_tmp = std::to_string(ret);
    Msg msg("HeapDestroy");
    msg.setItem("hHeap", hHeap_tmp);
    msg.setItem("ret", ret_tmp);
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
    // heap操作
    LPVOID ret = OldHeapAlloc(hHeap, dwFlags, dwBytes);
    if (isLocked() == false && handle_set.find(hHeap) != handle_set.end())
    {
        // heap操作上锁
        setLock();
        // 类型转换
        std::string hHeap_tmp = dword2hex((DWORD)hHeap);
        std::string dwFlags_tmp = dword2hex(dwFlags);
        std::string dwBytes_tmp = std::to_string(dwBytes);
        std::string ret_tmp = dword2hex((DWORD)ret);
        Msg msg("HeapAlloc");
        msg.setItem("hHeap", hHeap_tmp);
        msg.setItem("dwFlags", dwFlags_tmp);
        msg.setItem("dwBytes", dwBytes_tmp);
        msg.setItem("ret", ret_tmp);
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
    // heap操作
    BOOL ret = OldHeapFree(hHeap, dwFlags, lpMem);
    if (isLocked() == false && handle_set.find(hHeap) != handle_set.end())
    {
        // heap操作上锁
        setLock();
        Msg msg("HeapFree");
        // 类型转换
        std::string hHeap_tmp = dword2hex((DWORD)hHeap);
        std::string dwFlags_tmp = dword2hex(dwFlags);
        std::string lpMem_tmp = dword2hex((DWORD)lpMem);
        std::string ret_tmp = std::to_string(ret);
        msg.setItem("hHeap", hHeap_tmp);
        msg.setItem("dwFlags", dwFlags_tmp);
        msg.setItem("lpMem", lpMem_tmp);
        msg.setItem("ret", ret_tmp);
        client.send(msg.getMsg().c_str());
        releaseLock();
    }
    return ret;
}
