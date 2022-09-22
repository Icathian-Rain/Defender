
#pragma once
/*
    HOOK Sock
    HOOK API List: socket, bind, listen, accept, connect, send, recv, sendto, recvfrom, closesocket
*/
#include <atlstr.h>
#include "lock.h"
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

extern udp::UdpClient client;


// socket
static SOCKET(WINAPI *Oldsocket)(
    int af,
    int type,
    int protocol 
    ) = socket;
/// @brief newsocket
/// @param af 地址系列
/// @param type 类型, SOCK_STREAM基于TCP， SOCK_DGRAM基于UDP
/// @param protocol 协议号
/// @return 
extern "C" __declspec(dllexport) SOCKET WINAPI Newsocket(int af, int type, int protocol)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    SOCKET ret = Oldsocket(af, type, protocol);
    // 类型转换
    std::string af_tmp = std::to_string(af);
    std::string type_tmp = std::to_string(type);
    std::string protocol_tmp = std::to_string(protocol);
    std::string ret_tmp = std::to_string(ret);
    Msg msg("socket");
    msg.setItem("af", af_tmp);
    msg.setItem("type", type_tmp);
    msg.setItem("protocol", protocol_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// bind
static int(WINAPI *Oldbind)(
    SOCKET s, 
    const sockaddr *name, 
    int namelen
    ) = bind;
/// @brief newbind
/// @param s 绑定目标的socket
/// @param name 本地地址
/// @param namelen 地址长度
/// @return 
extern "C" __declspec(dllexport) int WINAPI Newbind(SOCKET s, const sockaddr *name, int namelen)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    int ret = Oldbind(s, name, namelen);
    // 类型转换
    std::string s_tmp = std::to_string(s);
    sockaddr_in *addr_in = (sockaddr_in *)name;
    char ip[20];
    std::string sin_family = std::to_string(addr_in->sin_family);
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, 20);
    std::string port = std::to_string(ntohs(addr_in->sin_port));
    std::string namelen_tmp = std::to_string(namelen);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("bind");
    msg.setItem("s", s_tmp);
    msg.setItem("sin_family", sin_family);
    msg.setItem("ip", ip);
    msg.setItem("port", port);
    msg.setItem("namelen", namelen_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());

    releaseLock();
    return ret;
}

// listen
static int(WINAPI *Oldlisten)(
    SOCKET s, 
    int backlog 
    ) = listen;
/// @brief newlisten
/// @param s 监听的socket
/// @param backlog 最大连接数
/// @return ret
extern "C" __declspec(dllexport) int WINAPI Newlisten(SOCKET s, int backlog)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    int ret = Oldlisten(s, backlog);
    // 类型转换
    std::string s_tmp = std::to_string(s);
    std::string backlog_tmp = std::to_string(backlog);
    std::string ret_tmp = std::to_string(ret);
    Msg msg("listen");
    msg.setItem("s", s_tmp);
    msg.setItem("backlog", backlog_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// accpet
static SOCKET(WINAPI *Oldaccept)(
    SOCKET s,       // 监听的socket
    sockaddr *addr, // 客户端地址
    int *addrlen    // 地址长度
    ) = accept;
extern "C" __declspec(dllexport) SOCKET WINAPI Newaccept(SOCKET s, sockaddr *addr, int *addrlen)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    SOCKET ret = Oldaccept(s, addr, addrlen);
    // 类型转换
    std::string s_tmp = std::to_string(s);
    sockaddr_in *addr_in = (sockaddr_in *)addr;
    char ip[20];
    std::string sin_family = std::to_string(addr_in->sin_family);
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, 20);
    std::string port = std::to_string(ntohs(addr_in->sin_port));
    std::string addrlen_tmp = std::to_string(*addrlen);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("accept");
    msg.setItem("s", s_tmp);
    msg.setItem("sin_family", sin_family);
    msg.setItem("ip", ip);
    msg.setItem("port", port);
    msg.setItem("addrlen", addrlen_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// connect
static int(WINAPI *Oldconnect)(
    SOCKET s,             // 源socket
    const sockaddr *name, // 目的地址
    int namelen           // 目的地址长度
    ) = connect;
extern "C" __declspec(dllexport) int WINAPI Newconnect(SOCKET s, const sockaddr *name, int namelen)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    int ret = Oldconnect(s, name, namelen);
    // 类型转换
    std::string s_tmp = std::to_string(s);
    sockaddr_in *addr_in = (sockaddr_in *)name;
    char ip[20];
    std::string sin_family = std::to_string(addr_in->sin_family);
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, 20);
    std::string port = std::to_string(ntohs(addr_in->sin_port));
    std::string namelen_tmp = std::to_string(namelen);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("connect"); 
    msg.setItem("s", s_tmp);
    msg.setItem("sin_family", sin_family);
    msg.setItem("ip", ip);
    msg.setItem("port", port);
    msg.setItem("namelen", namelen_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// send
static int(WINAPI *Oldsend)(
    SOCKET s,        // 源套接字
    const char *buf, // 数据地址
    int len,         // 数据长度
    int flags        // 0
    ) = send;
extern "C" __declspec(dllexport) int WINAPI Newsend(SOCKET s, const char *buf, int len, int flags)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    int ret = Oldsend(s, buf, len, flags);
    // 类型转换
    std::string s_tmp = std::to_string(s);
    std::string buf_tmp(buf, len);
    std::string len_tmp = std::to_string(len);
    std::string flags_tmp = std::to_string(flags);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("send");
    msg.setItem("s", s_tmp);
    msg.setItem("buf", buf_tmp);
    msg.setItem("len", len_tmp);
    msg.setItem("flags", flags_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// recv
static int(WINAPI *Oldrecv)(
    SOCKET s,  // 源地址socket
    char *buf, // 存储接收数据的地址
    int len,   // 数据长度
    int flags  // 传送数据的类型
    ) = recv;
extern "C" __declspec(dllexport) int WINAPI Newrecv(SOCKET s, char *buf, int len, int flags)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    int ret = Oldrecv(s, buf, len, flags);
    // 类型转换
    std::string s_tmp = std::to_string(s);
    std::string buf_tmp(buf, ret);
    std::string len_tmp = std::to_string(len);
    std::string flags_tmp = std::to_string(flags);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("recv");
    msg.setItem("s", s_tmp);
    msg.setItem("buf", buf_tmp);
    msg.setItem("len", len_tmp);
    msg.setItem("flags", flags_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// sendto
static int(WINAPI *Oldsendto)(
    SOCKET s,           // 源套接字
    const char *buf,    // 数据地址
    int len,            // 数据长度
    int flags,          // 传送数据的类型
    const sockaddr *to, // 目的地址
    int tolen           // 目的地址长度
    ) = sendto;
extern "C" __declspec(dllexport) int WINAPI Newsendto(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen)
{
    int ret;
    if(isLocked() == false)
    {
        setLock();
        ret = Oldsendto(s, buf, len, flags, to, tolen);
        Msg msg("sendto");
        std::string buf_tmp(buf, len);
        msg.setItem("s", std::to_string(s));
        msg.setItem("buf", buf_tmp);
        msg.setItem("len", std::to_string(len));
        msg.setItem("flags", std::to_string(flags));
        msg.setItem("to", std::to_string((int)to));
        msg.setItem("tolen", std::to_string(tolen));
        msg.setItem("ret", std::to_string(ret));
        client.send(msg.getMsg().c_str());
        releaseLock();
    }
    else
    {
        ret = Oldsendto(s, buf, len, flags, to, tolen);
    }
    return ret;
}

// recvfrom
static int(WINAPI *Oldrecvfrom)(
    SOCKET s,       // 源地址socket
    char *buf,      // 存储接收数据的地址
    int len,        // 数据长度
    int flags,      // 传送数据的类型
    sockaddr *from, // 源地址
    int *fromlen    // 源地址长度
    ) = recvfrom;
extern "C" __declspec(dllexport) int WINAPI Newrecvfrom(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen)
{
    int ret;
    if( isLocked() == false)
    {
        setLock();
        ret = Oldrecvfrom(s, buf, len, flags, from, fromlen);
        Msg msg("recvfrom");
        std::string buf_tmp(buf, len);
        msg.setItem("s", std::to_string(s));
        msg.setItem("buf", buf_tmp);
        msg.setItem("len", std::to_string(len));
        msg.setItem("flags", std::to_string(flags));
        msg.setItem("from", std::to_string((int)from));
        msg.setItem("fromlen", std::to_string((int)fromlen));
        msg.setItem("ret", std::to_string(ret));
        client.send(msg.getMsg().c_str());
        releaseLock();
    }
    else
    {
        ret = Oldrecvfrom(s, buf, len, flags, from, fromlen);
    }
    return ret;
}