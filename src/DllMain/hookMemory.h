#pragma once
/*
    HOOK memory
    HOOK API List: memcpy
*/
#include <atlstr.h>
#include "lock.h"
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

extern udp::UdpClient client;

// memcpy
static void *(__cdecl *Oldmemcpy)(
    void *_Dst,       // 目的地址
    const void *_Src, // 源地址
    size_t _Size      // 大小
    ) = memcpy;
/// @brief newMemcpy
/// @param _Dst 目的地址
/// @param _Src 源地址
/// @param _Size 大小
/// @return 
extern "C" __declspec(dllexport) void *__cdecl Newmemcpy(void *_Dst, const void *_Src, size_t _Size)
{
    // 函数操作
    void *ret = Oldmemcpy(_Dst, _Src, _Size);
    if (isLocked() == false)
    {
         // heap操作上锁
        setLock();
         // 类型转换
        std::string _Dst_tmp = dword2hex((DWORD)_Dst);
        std::string _Src_tmp = dword2hex((DWORD)_Src);
        std::string _Size_tmp = std::to_string(_Size);
        std::string ret_tmp = dword2hex((DWORD)ret);
        Msg msg("memcpy");
        msg.setItem("Dst", _Dst_tmp);
        msg.setItem("Src", _Src_tmp);
        msg.setItem("Size", _Size_tmp);
        msg.setItem("ret", ret_tmp);
        client.send(msg.getMsg().c_str());
        // heap操作解锁
        releaseLock();
    }
    return ret;
}