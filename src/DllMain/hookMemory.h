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
// static void *(__cdecl *Oldmemcpy)(
//     void *_Dst,       // 目的地址
//     const void *_Src, // 源地址
//     size_t _Size      // 大小
//     ) = memcpy;
// extern "C" __declspec(dllexport) void *__cdecl Newmemcpy(void *_Dst, const void *_Src, size_t _Size)
// {
//     Msg msg("memcpy");
//     msg.setItem("Dst", std::to_string((int)_Dst));
//     msg.setItem("Src", std::to_string((int)_Src));
//     msg.setItem("Size", std::to_string(_Size));
//     client.send(msg.getMsg().c_str());
//     return Oldmemcpy(_Dst, _Src, _Size);
// }