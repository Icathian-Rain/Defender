
#pragma once
/*
    HOOK Reg
    HOOK API List: RegCreateKeyEx, RegSetValueEx,RegOpenKeyEx, RegQueryValueEx, RegDeleteKey, RegDeleteValue, RegCloseKey
*/
#include <atlstr.h>
#include "lock.h"
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

extern udp::UdpClient client;


// RegCreateKeyEx
static LSTATUS(WINAPI *OldRegCreateKeyEx)(
    HKEY hKey,
    LPCWSTR lpSubKey,
    DWORD Reserved,
    LPWSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired, 
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition
    ) = RegCreateKeyEx;

/// @brief newRegCreateKeyEx
/// @param hKey 一个打开项的句柄
/// @param lpSubKey 创建子项的名字
/// @param Reserved 0
/// @param lpClass 项的类名
/// @param dwOptions 选项
/// @param samDesired 定义访问权限
/// @param lpSecurityAttributes 指定要创建的秘钥的访问权限的掩码
/// @param phkResult 指向接收到已打开或创建的秘钥句柄的变量的指针
/// @param lpdwDisposition 出参
/// @return ret
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCreateKeyEx(
    HKEY hKey,
    LPCWSTR lpSubKey,
    DWORD Reserved,
    LPWSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    LSTATUS ret = OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);   
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string lpSubKey_tmp = wchar2string(lpSubKey);
    std::string Reserved_tmp = dword2hex(Reserved);
    std::string lpClass_tmp = "NULL";
    if (lpClass != NULL)
    {
        lpClass_tmp = wchar2string(lpClass);
    }
    std::string dwOptions_tmp = dword2hex(dwOptions);
    std::string samDesired_tmp = std::to_string(samDesired);
    std::string lpSecurityAttributes_tmp = std::to_string((DWORD)lpSecurityAttributes);
    std::string phkResult_tmp = std::to_string((DWORD)phkResult);
    std::string lpdwDisposition_tmp = "NULL";
    if (lpdwDisposition != NULL)
    {
        lpdwDisposition_tmp = dword2hex(*lpdwDisposition);
    }
    std::string ret_tmp = dword2hex(ret);
    // 构造消息
    Msg msg("RegCreateKeyEx");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("lpSubKey", lpSubKey_tmp);
    msg.setItem("Reserved", Reserved_tmp);
    msg.setItem("lpClass", lpClass_tmp);
    msg.setItem("dwOptions", dwOptions_tmp);
    msg.setItem("samDesired", samDesired_tmp);
    msg.setItem("lpSecurityAttributes", lpSecurityAttributes_tmp);
    msg.setItem("phkResult", phkResult_tmp);
    msg.setItem("lpdwDisposition", lpdwDisposition_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegSetValueEx
static LSTATUS(WINAPI *OldRegSetValueEx)(
    HKEY hKey,
    LPCWSTR lpValueName,
    DWORD Reserved,
    DWORD dwType,
    const BYTE *lpData,
    DWORD cbData
    ) = RegSetValueEx;

/// @brief 
/// @param hKey 已经打开的项的句柄
/// @param lpValueName 设置值的名称 wchar
/// @param Reserved 强制为0
/// @param dwType 指定要保存数据的类型
/// @param lpData 保存指定值名称的数据
/// @param cbData 数据的大小，单位为字节
/// @return ret
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegSetValueEx(
    HKEY hKey,
    LPCWSTR lpValueName,
    DWORD Reserved,
    DWORD dwType,
    const BYTE *lpData,
    DWORD cbData)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    LSTATUS ret = OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string lpValueName_tmp = wchar2string(lpValueName);
    std::string Reserved_tmp = dword2hex(Reserved);
    std::string dwType_tmp = dword2hex(dwType);
    std::string lpData_tmp = wchar2string((wchar_t *)lpData);
    lpData_tmp = GbkToUtf8(lpData_tmp.c_str());
    // std::string lpData_tmp = std::string((char *)lpData, cbData);
    std::string cbData_tmp = std::to_string(cbData);
    std::string res_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("RegSetValueEx");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("lpValueName", lpValueName_tmp);
    msg.setItem("Reserved 0", Reserved_tmp);
    msg.setItem("dwType", dwType_tmp);
    msg.setItem("lpData", lpData_tmp);
    msg.setItem("cbData", cbData_tmp);
    msg.setItem("ret", res_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegOpenKeyEx
static LSTATUS(WINAPI *OldRegOpenKeyEx)(
    HKEY hKey,         // 
    LPCWSTR lpSubKey,  //
    DWORD ulOptions,   // 
    REGSAM samDesired, // 
    PHKEY phkResult    // 
    ) = RegOpenKeyEx;
/// @brief newRegOpenKeyEx
/// @param hKey  需要打开的主键
/// @param lpSubKey 需要打开的子键的名称
/// @param ulOptions 保留， 0
/// @param samDesired 安全访问标记，权限
/// @param phkResult 得到的将要打开键的句柄
/// @return ret
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY hKey,
    LPCWSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkResult)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    LSTATUS ret = OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string lpSubKey_tmp = wchar2string(lpSubKey);
    std::string ulOptions_tmp = dword2hex(ulOptions);
    std::string samDesired_tmp = std::to_string(samDesired);
    std::string phkResult_tmp = dword2hex((DWORD)*phkResult);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("RegOpenKeyEx");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("lpSubKey", lpSubKey_tmp);
    msg.setItem("ulOptions", ulOptions_tmp);
    msg.setItem("samDesired", samDesired_tmp);
    msg.setItem("phkResult", phkResult_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}
// RegQueryValueEx
static LSTATUS(WINAPI *OldRegQueryValueEx)(
    HKEY hKey, 
    LPCWSTR lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData
    ) = RegQueryValueEx;
/// @brief newRegQueryValueEx
/// @param hKey 已经打开的项的句柄
/// @param lpValueName 要查询的值的名称
/// @param lpReserved 保留， 0
/// @param lpType 指向一个变量，用于接收值的类型
/// @param lpData 指向一个缓冲区，用于接收值的数据
/// @param lpcbData 指向一个变量，用于接收数据的大小
/// @return ret
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegQueryValueEx(
    HKEY hKey,
    LPCWSTR lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData)
{
    setLock();
    LSTATUS ret = OldRegQueryValueEx(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string lpValueName_tmp = wchar2string(lpValueName);
    std::string lpReserved_tmp = std::to_string((int)lpReserved);
    std::string lpType_tmp = "NULL";
    if(lpType != NULL)
        lpType_tmp = std::to_string(*lpType);
    std::string lpData_tmp = "NULL";
    if(lpData != NULL)
        lpData_tmp = wchar2string((wchar_t *)lpData);
    std::string lpcbData_tmp = "NULL";
    if(lpcbData != NULL)
        lpcbData_tmp = std::to_string(*lpcbData);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("RegQueryValueEx");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("lpValueName", lpValueName_tmp);
    msg.setItem("lpReserved", lpReserved_tmp);
    msg.setItem("lpType", lpType_tmp);
    msg.setItem("lpData", lpData_tmp);
    msg.setItem("lpcbData", lpcbData_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}


// RegCloseKey
static LSTATUS(WINAPI *OldRegCloseKey)(
    HKEY hKey 
    ) = RegCloseKey;
/// @brief newRegCloseKey
/// @param hKey 关闭键的句柄
/// @return ret
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCloseKey(
    HKEY hKey)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    LSTATUS ret = OldRegCloseKey(hKey);
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("RegCloseKey");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegDeleteKey
static LSTATUS(WINAPI *OldRegDeleteKey)(
    HKEY hKey, 
    LPCWSTR lpSubKey  
    ) = RegDeleteKey;
/// @brief newRegDeleteKey
/// @param hKey 关闭键的句柄
/// @param lpSubKey 子键的名称
/// @return 
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegDeleteKey(
    HKEY hKey,
    LPCWSTR lpSubKey)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    LSTATUS ret = OldRegDeleteKey(hKey, lpSubKey);
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string lpSubKey_tmp = wchar2string(lpSubKey);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("RegDeleteKey");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("lpSubKey", lpSubKey_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegDeleteValue
static LSTATUS(WINAPI *OldRegDeleteValue)(
    HKEY hKey,
    LPCWSTR lpValueName
    ) = RegDeleteValue;

/// @brief newRegDeleteValue
/// @param hKey 打开项的句柄
/// @param lpValueName 要删除的值名
/// @return ret
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegDeleteValue(
    HKEY hKey,
    LPCWSTR lpValueName)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    LSTATUS ret = OldRegDeleteValue(hKey, lpValueName);
    // 类型转换
    std::string hKey_tmp = dword2hex((DWORD)hKey);
    std::string lpValueName_tmp = wchar2string(lpValueName);
    std::string ret_tmp = std::to_string(ret);
    Msg msg("RegDeleteValue");
    msg.setItem("hKey", hKey_tmp);
    msg.setItem("lpValueName", lpValueName_tmp);
    msg.setItem("ret", ret_tmp);
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}