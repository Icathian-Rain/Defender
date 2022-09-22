
#pragma once
/*setSelectedType
    HOOK MESSAGEBOX
    HOOK API List: MessageBoxA, MessageBoxW
*/
#include <atlstr.h>
#include "lock.h"
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

// client
extern udp::UdpClient client;

// MessageBoxA HOOK
static int(WINAPI *OldMessageBoxA)(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType) = MessageBoxA;

/// @brief newMessageBoxA
/// @param hWnd 窗口句柄，代表这个消息窗口的所有者
/// @param lpText 填入窗口的字符串 char *
/// @param lpCaption 填入窗口的标题	char *
/// @param uType 指定窗口的类型
/// @return ret 返回值
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType)
{
    // 上锁
    setLock();
    // 进行函数操作
    int ret = OldMessageBoxA(hWnd, lpText, lpCaption, uType);
    // 类型转换
    std::string hWnd_tmp = dword2hex((DWORD)hWnd);
    std::string lpText_tmp = std::string(lpText);
    std::string lpCaption_tmp = std::string(lpCaption);
    std::string uType_tmp = dword2hex((DWORD)uType);
    std::string ret_tmp = std::to_string(ret);
    // 构建消息
    Msg msg("MessageBoxA");
    msg.setItem("hWnd", hWnd_tmp);
    msg.setItem("lpText", lpText_tmp);
    msg.setItem("lpCaption", lpCaption_tmp);
    msg.setItem("uType", uType_tmp);
    msg.setItem("ret", ret_tmp);
    // 发送消息
    client.send(msg.getMsg().c_str());
    // 解锁
    releaseLock();
    return ret;
}

// MessageBoxW HOOK
static int(WINAPI *OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;

/// @brief newMessageBoxW
/// @param hWnd 窗口句柄，代表这个消息窗口的所有者
/// @param lpText 填入窗口的字符串 wchar_t *
/// @param lpCaption 窗口标题 wchar_t *
/// @param uType 类型
/// @return ret 返回值
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    int ret = OldMessageBoxW(hWnd, lpText, lpCaption, uType);
    // 类型转换
    std::string hWnd_tmp = dword2hex((DWORD)hWnd);
    std::string lpText_tmp = wchar2string(lpText);
    std::string lpCaption_tmp = wchar2string(lpCaption);
    std::string uType_tmp = dword2hex((DWORD)uType);
    std::string ret_tmp = std::to_string(ret);
    // 构建消息
    Msg msg("MessageBoxW");
    msg.setItem("hWnd", hWnd_tmp);
    msg.setItem("lpText", lpText_tmp);
    msg.setItem("lpCaption", lpCaption_tmp);
    msg.setItem("uType", uType_tmp);
    msg.setItem("ret", ret_tmp);
    // 发送消息
    client.send(msg.getMsg().c_str());
    // heap操作解锁
    releaseLock();
    return ret;
}
