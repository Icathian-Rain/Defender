// Headers
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include <atlstr.h>
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

// lib
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")

// udp client
// 参数：ip地址，端口号
udp::UdpClient client("127.0.0.1", 4000);

// API List
// // MessageBox操作
// #define MessageBoxA 1
// #define MessageBoxW 2

// // 堆操作
// #define HeapCreate 3
// #define HeapDestroy 4
// #define HeapAlloc 5
// #define HeapFree 6

// // 文件操作
// #define CreateFile 7
// #define WriteFile 8
// #define ReadFile 9

// // 注册表操作
// #define RegCreateKeyEx 10
// #define RegSetValueEx 11
// #define RegOpenKeyEx  12
// #define RegQueryValueEx 13
// #define RegCloseKey 13
// #define RegDeleteKey 14
// #define RegDeleteValue 15

// // 网络通信操作
// #define socket 16
// // tcp
// #define bind 17
// #define listen 18
// #define accept 19
// #define connect 20
// #define send 21
// #define recv 22
// // udp
// #define sendto 23
// #define recvfrom 24


// MessageBox
// MessageBoxA
static int(WINAPI *OldMessageBoxA)(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType) = MessageBoxA;

extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(
    _In_opt_ HWND hWnd,        // 窗口句柄，代表这个消息窗口的所有者
    _In_opt_ LPCSTR lpText,    // 填入窗口的字符串 char *
    _In_opt_ LPCSTR lpCaption, // 填入窗口的标题	char *
    _In_ UINT uType            // button
)
{
    // heap操作上锁
    setLock();

    int ret = OldMessageBoxA(hWnd, lpText, lpCaption, uType);
    // char *参数转string
    std::string lpText_tmp = std::string(lpText);
    std::string lpCaption_tmp = std::string(lpCaption);

    // 构建消息
    Msg msg("MessageBoxA");
    msg.setItem("hWnd", std::to_string((int)hWnd));
    msg.setItem("lpText", lpText_tmp);
    msg.setItem("lpCaption", lpCaption_tmp);
    msg.setItem("uType", std::to_string(uType));
    msg.setItem("ret", std::to_string(ret));

    // 发送消息
    client.send(msg.getMsg().c_str());

    // heap操作解锁
    releaseLock();
    return ret;
}


// MessageBoxW
static int(WINAPI *OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(
    _In_opt_ HWND hWnd,         // OldMessageBoxA
    _In_opt_ LPCWSTR lpText,    // 填入窗口的字符串 wchar *
    _In_opt_ LPCWSTR lpCaption, // 填入窗口的标题	wchar *
    _In_ UINT uType             // 类型
)
{
    // heap操作上锁
    setLock();
    int ret = OldMessageBoxW(hWnd, lpText, lpCaption, uType);
    // wchar *参数转string
    std::string lpText_tmp = wchar2string(lpText);
    std::string lpCaption_tmp = wchar2string(lpCaption);

    // 构建消息
    Msg msg("MessageBoxW");
    msg.setItem("hWnd", std::to_string((int)hWnd));
    msg.setItem("lpText", lpText_tmp);
    msg.setItem("lpCaption", lpCaption_tmp);
    msg.setItem("uType", std::to_string(uType));
    msg.setItem("ret", std::to_string(ret));

    // 发送消息
    client.send(msg.getMsg().c_str());

    // heap操作解锁
    releaseLock();

    return ret;
}

// CreateFile
static HANDLE(WINAPI *OldCreateFile)(
    LPCWSTR lpFileName,                         // 打开文件名，宽字符
    DWORD dwDesiredAccess,                      // 权限管理，读or 写
    DWORD dwShareMode,                          // 文件是否共享访问
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // 文件的安全特性
    DWORD dwCreationDisposition,                // 创建or 打开
    DWORD dwFlagsAndAttributes,                 // 文档属性
    HANDLE hTemplateFile                        // 如果不为0则指定一个文件句柄，复制扩展属性
    ) = CreateFile;
extern "C" __declspec(dllexport) HANDLE WINAPI NewCreateFile(
    LPCWSTR lpFileName,                         // 打开文件名，宽字符
    DWORD dwDesiredAccess,                      // 权限管理，读or 写
    DWORD dwShareMode,                          // 文件是否共享访问
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // 文件的安全特性
    DWORD dwCreationDisposition,                // 创建or 打开
    DWORD dwFlagsAndAttributes,                 // 文档属性
    HANDLE hTemplateFile                        // 如果不为0则指定一个文件句柄，复制扩展属性
)
{
    // heap操作上锁
    setLock();
    HANDLE ret = OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    // 构建消息
    Msg msg("CreateFile");
    msg.setItem("lpFileName", wchar2string(lpFileName));
    msg.setItem("dwDesiredAccess", dword2hex(dwDesiredAccess));
    msg.setItem("dwShareMode", dword2hex(dwShareMode));
    msg.setItem("lpSecurityAttributes", dword2hex((DWORD)lpSecurityAttributes));
    msg.setItem("dwCreationDisposition", dword2hex(dwCreationDisposition));
    msg.setItem("dwFlagsAndAttributes", dword2hex(dwFlagsAndAttributes));
    msg.setItem("hTemplateFile", dword2hex((DWORD)hTemplateFile));
    msg.setItem("ret", dword2hex((DWORD)ret));
    client.send(msg.getMsg().c_str());

    // heap操作解锁
    releaseLock();

    return ret;
}

// WriteFile
static BOOL(WINAPI *OldWriteFile)(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped) = WriteFile;
extern "C" __declspec(dllexport) BOOL WINAPI NewWriteFile(
    HANDLE hFile,                   // 处理要写入的文件
    LPCVOID lpBuffer,               // 指向要写入文件的数据的指针
    DWORD nNumberOfBytesToWrite,    // 要写入的字节数
    LPDWORD lpNumberOfBytesWritten, // 指针写入的字节数
    LPOVERLAPPED lpOverlapped       // 指向重叠I/O结构的指针
)
{
    // heap操作上锁
    setLock();
    BOOL ret = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    // 构建消息
    Msg msg("WriteFile");

    // 文件内容为字符串
    std::string lpBuffer_tmp((char *)lpBuffer, nNumberOfBytesToWrite);
    lpBuffer_tmp = GbkToUtf8(lpBuffer_tmp.c_str());

    // 文件内容为二进制
    // std::string lpBuffer_tmp = bytes2string((const BYTE*)lpBuffer, nNumberOfBytesToWrite);
    
    msg.setItem("hFile", dword2hex((DWORD)hFile));
    msg.setItem("lpBuffer", lpBuffer_tmp);
    msg.setItem("nNumberOfBytesToWrite", std::to_string(nNumberOfBytesToWrite));
    if (lpNumberOfBytesWritten != NULL)
    {
        msg.setItem("lpNumberOfBytesWritten", std::to_string(*lpNumberOfBytesWritten));
    }
    else
    {
        msg.setItem("lpNumberOfBytesWritten", "NULL");
    }
    msg.setItem("lpOverlapped", dword2hex((DWORD)lpOverlapped));
    msg.setItem("ret", std::to_string(ret));

    // 发送消息
    client.send(msg.getMsg().c_str());
    // heap操作解锁
    releaseLock();

    return ret;
}
// ReadFile
static BOOL(WINAPI *OldReadFile)(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped) = ReadFile;
extern "C" __declspec(dllexport) BOOL WINAPI NewReadFile(
    HANDLE hFile,                // 处理要读取的文件
    LPVOID lpBuffer,             // 保存数据的指针
    DWORD nNumberOfBytesToRead,  // 要读取的字节数
    LPDWORD lpNumberOfBytesRead, // 读取的字节数
    LPOVERLAPPED lpOverlapped    // 指向重叠I/O结构的指针
)
{
    // heap操作上锁
    setLock();
    BOOL ret = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);    
    Msg msg("ReadFile");
    // 文件内容为字符串
    std::string lpBuffer_tmp((char *)lpBuffer, nNumberOfBytesToRead);
    lpBuffer_tmp = GbkToUtf8(lpBuffer_tmp.c_str());

    // 文件内容为二进制
    // std::string lpBuffer_tmp = bytes2string((const BYTE*)lpBuffer, nNumberOfBytesToRead);

    msg.setItem("hFile", dword2hex((DWORD)hFile));
    msg.setItem("lpBuffer", lpBuffer_tmp);
    msg.setItem("nNumberOfBytesToRead", std::to_string(nNumberOfBytesToRead));
    if (lpNumberOfBytesRead != NULL)
    {
        msg.setItem("lpNumberOfBytesRead", std::to_string(*lpNumberOfBytesRead));
    }
    else
    {
        msg.setItem("lpNumberOfBytesRead", "NULL");
    }
    if (lpOverlapped != NULL)
    {
        msg.setItem("lpOverlapped", dword2hex((DWORD)lpOverlapped));
    }
    else
    {
        msg.setItem("lpOverlapped", "NULL");
    }
    msg.setItem("ret", std::to_string(ret));
    // 发送消息
    client.send(msg.getMsg().c_str());

    // heap操作解锁
    releaseLock();
    return ret;
}

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
static BOOL(WINAPI *OldHeapDestory)(HANDLE) = HeapDestroy;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestory(
    HANDLE hHeap // 要销毁堆的句柄
)
{
    // heap操作上锁
    setLock();
    BOOL ret = OldHeapDestory(hHeap);
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

// RegCreateKeyEx
static LSTATUS(WINAPI *OldRegCreateKeyEx)(
    HKEY hKey,                                        // 一个打开项的句柄
    LPCWSTR lpSubKey,                                 // 创建子项的名字
    DWORD Reserved,                                   // 0
    LPWSTR lpClass,                                   // 项的类名
    DWORD dwOptions,                                  // 选项
    REGSAM samDesired,                                // 定义访问权限
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes, // 指定要创建的秘钥的访问权限的掩码
    PHKEY phkResult,                                  // 指向接收到已打开或创建的秘钥句柄的变量的指针
    LPDWORD lpdwDisposition                           // 出参
    ) = RegCreateKeyEx;

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
    setLock();
    DWORD res;
    if (lpdwDisposition == NULL)
    {
        lpdwDisposition = &res;
    }
    LSTATUS ret = OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, &res);   
    Msg msg("RegCreateKeyEx");
    std::string lpSubKey_tmp = wchar2string(lpSubKey);
    std::string lpClass_tmp = "";
    if (lpClass != NULL)
        lpClass_tmp = wchar2string(lpClass);
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("lpSubKey", lpSubKey_tmp);
    msg.setItem("Reserved", std::to_string(Reserved));
    msg.setItem("lpClass", lpClass_tmp);
    msg.setItem("dwOptions", dword2hex(dwOptions));
    msg.setItem("samDesired", std::to_string(samDesired));
    msg.setItem("lpSecurityAttributes", std::to_string((int)lpSecurityAttributes));
    msg.setItem("phkResult", std::to_string((int)phkResult));
    msg.setItem("lpdwDisposition", dword2hex(*lpdwDisposition));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegSetValueEx
static LSTATUS(WINAPI *OldRegSetValueEx)(
    HKEY hKey,           // 已经打开的项的句柄
    LPCWSTR lpValueName, // 设置值的名称 wchar
    DWORD Reserved,      // 强制为0
    DWORD dwType,        // 指定要保存数据的类型
    const BYTE *lpData,  // 保存指定值名称的数据
    DWORD cbData         // 数据的大小，单位为字节
    ) = RegSetValueEx;
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegSetValueEx(
    HKEY hKey,
    LPCWSTR lpValueName,
    DWORD Reserved,
    DWORD dwType,
    const BYTE *lpData,
    DWORD cbData)
{
    setLock();
    LSTATUS ret = OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    Msg msg("RegSetValueEx");
    std::string lpValueName_tmp = wchar2string(lpValueName);
    // std::string lpData_tmp = std::string((char *)lpData, cbData);
    std::string lpData_tmp = wchar2string((wchar_t *)lpData);
    lpData_tmp = GbkToUtf8(lpData_tmp.c_str());
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("lpValueName", lpValueName_tmp);
    msg.setItem("Reserved 0", dword2hex(Reserved));
    msg.setItem("dwType", dword2hex(dwType));
    msg.setItem("lpData", lpData_tmp);
    msg.setItem("cbData", std::to_string(cbData));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegOpenKeyEx
static LSTATUS(WINAPI *OldRegOpenKeyEx)(
    HKEY hKey,         // 需要打开的主键
    LPCWSTR lpSubKey,  // 需要打开的子键的名称
    DWORD ulOptions,   // 保留， 0
    REGSAM samDesired, // 安全访问标记，权限
    PHKEY phkResult    // 得到的将要打开键的句柄
    ) = RegOpenKeyEx;
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY hKey,
    LPCWSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkResult)
{
    setLock();
    LSTATUS ret = OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    Msg msg("RegOpenKeyEx");
    std::string lpSubKey_tmp = wchar2string(lpSubKey);
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("lpSubKey", lpSubKey_tmp);
    msg.setItem("ulOptions", dword2hex(ulOptions));
    msg.setItem("samDesired", std::to_string(samDesired));
    msg.setItem("phkResult", dword2hex((DWORD)*phkResult));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}
// RegQueryValueEx
static LSTATUS(WINAPI *OldRegQueryValueEx)(
    HKEY hKey,         // 已经打开的项的句柄
    LPCWSTR lpValueName, // 要查询的值的名称
    LPDWORD lpReserved,  // 保留， 0
    LPDWORD lpType,      // 指向一个变量，用于接收值的类型
    LPBYTE lpData,       // 指向一个缓冲区，用于接收值的数据
    LPDWORD lpcbData     // 指向一个变量，用于接收数据的大小
    ) = RegQueryValueEx;
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
    // 信息构造
    Msg msg("RegQueryValueEx");
    std::string lpValueName_tmp = wchar2string(lpValueName);
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("lpValueName", lpValueName_tmp);
    msg.setItem("lpReserved", std::to_string((int)lpReserved));
    if(lpType != NULL)
    {
        msg.setItem("lpType", std::to_string((int)*lpType));
    }
    else
    {
        msg.setItem("lpType", "NULL");
    }

    std::string lpData_tmp;
    if(lpData != NULL)
    {
        lpData_tmp = wchar2string((wchar_t *)lpData);
    }
    else
    {
        lpData_tmp = "NULL";
    }
    msg.setItem("lpData", lpData_tmp);
    if(lpcbData != NULL)
    {
        msg.setItem("lpcbData", std::to_string(*lpcbData));
    }
    else
    {
        msg.setItem("lpcbData", "NULL");
    }
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}


// RegCloseKey
static LSTATUS(WINAPI *OldRegCloseKey)(
    HKEY hKey // 关闭键的句柄
    ) = RegCloseKey;
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCloseKey(
    HKEY hKey)
{
    setLock();
    LSTATUS ret = OldRegCloseKey(hKey);
    Msg msg("RegCloseKey");
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return OldRegCloseKey(hKey);
}

// RegDeleteKey
static LSTATUS(WINAPI *OldRegDeleteKey)(
    HKEY hKey,       // 关闭键的句柄
    LPCWSTR lpSubKey // 子键的名称
    ) = RegDeleteKey;
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegDeleteKey(
    HKEY hKey,
    LPCWSTR lpSubKey)
{
    setLock();
    LSTATUS ret = OldRegDeleteKey(hKey, lpSubKey);
    Msg msg("RegDeleteKey");
    std::string lpSubKey_tmp = wchar2string(lpSubKey);
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("lpSubKey", lpSubKey_tmp);
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// RegDeleteValue
static LSTATUS(WINAPI *OldRegDeleteValue)(
    HKEY hKey,          // 打开项的句柄
    LPCWSTR lpValueName // 要删除的值名
    ) = RegDeleteValue;
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegDeleteValue(
    HKEY hKey,
    LPCWSTR lpValueName)
{
    setLock();
    LSTATUS ret = OldRegDeleteValue(hKey, lpValueName);
    Msg msg("RegDeleteValue");
    std::string lpValueName_tmp = wchar2string(lpValueName);
    msg.setItem("hKey", dword2hex((DWORD)hKey));
    msg.setItem("lpValueName", lpValueName_tmp);
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// socket
static SOCKET(WINAPI *Oldsocket)(
    int af,      // 地址系列
    int type,    // 类型, SOCK_STREAM基于TCP， SOCK_DGRAM基于UDP
    int protocol // 协议号
    ) = socket;
extern "C" __declspec(dllexport) SOCKET WINAPI Newsocket(int af, int type, int protocol)
{
    setLock();
    SOCKET ret = Oldsocket(af, type, protocol);
    Msg msg("socket");
    msg.setItem("af", std::to_string(af));
    msg.setItem("type", std::to_string(type));
    msg.setItem("protocol", std::to_string(protocol));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());
    releaseLock();
    return ret;
}

// bind
static int(WINAPI *Oldbind)(
    SOCKET s,             // 绑定目标的socket
    const sockaddr *name, // 本地地址
    int namelen           // 地址长度
    ) = bind;
extern "C" __declspec(dllexport) int WINAPI Newbind(SOCKET s, const sockaddr *name, int namelen)
{
    setLock();
    int ret = Oldbind(s, name, namelen);
    Msg msg("bind");
    msg.setItem("s", std::to_string(s));
    // msg.setItem("name", std::to_string((int)name));
    sockaddr_in *addr_in = (sockaddr_in *)name;
    std::string sin_family = std::to_string(addr_in->sin_family);
    // std::string ip = inet_ntoa(addr->sin_addr);
    char ip[20];
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, 20);
    
    std::string port = std::to_string(ntohs(addr_in->sin_port));
    msg.setItem("sin_family", sin_family);
    msg.setItem("ip", ip);
    msg.setItem("port", port);
    msg.setItem("namelen", std::to_string(namelen));
    msg.setItem("ret", std::to_string(ret));
    client.send(msg.getMsg().c_str());

    releaseLock();
    return ret;
}

// listen
static int(WINAPI *Oldlisten)(
    SOCKET s,   // 监听的socket
    int backlog // 最大连接数
    ) = listen;
extern "C" __declspec(dllexport) int WINAPI Newlisten(SOCKET s, int backlog)
{
    setLock();

    int ret = Oldlisten(s, backlog);
    Msg msg("listen");
    msg.setItem("s", std::to_string(s));
    msg.setItem("backlog", std::to_string(backlog));
    msg.setItem("ret", std::to_string(ret));
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
    setLock();

    SOCKET ret = Oldaccept(s, addr, addrlen);
    Msg msg("accept");
    msg.setItem("s", std::to_string(s));
    // msg.setItem("addr", std::to_string((int)addr));
    sockaddr_in *addr_in = (sockaddr_in *)addr;
    std::string sin_family = std::to_string(addr_in->sin_family);

    // std::string ip = inet_ntoa(addr_in->sin_addr);
    char ip[20];
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, 20);

    std::string port = std::to_string(ntohs(addr_in->sin_port));

    msg.setItem("sin_family", sin_family);
    msg.setItem("ip", ip);
    msg.setItem("port", port);
    msg.setItem("addrlen", std::to_string((int)addrlen));
    msg.setItem("ret", std::to_string(ret));
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
    setLock();
    int ret = Oldconnect(s, name, namelen);
    Msg msg("connect");
    msg.setItem("s", std::to_string(s));
    // msg.setItem("name", std::to_string((int)name));
    sockaddr_in *addr_in = (sockaddr_in *)name;
    std::string sin_family = std::to_string(addr_in->sin_family);
    // std::string ip = inet_ntoa(addr->sin_addr);
    char ip[20];
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, 20);
    std::string port = std::to_string(ntohs(addr_in->sin_port));
    msg.setItem("sin_family", sin_family);
    msg.setItem("ip", ip);
    msg.setItem("port", port);
    msg.setItem("namelen", std::to_string(namelen));
    msg.setItem("ret", std::to_string(ret));
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
    setLock();
    int ret = Oldsend(s, buf, len, flags);
    Msg msg("send");
    std::string buf_tmp(buf, len);
    msg.setItem("s", std::to_string(s));
    msg.setItem("buf", buf_tmp);
    msg.setItem("len", std::to_string(len));
    msg.setItem("flags", std::to_string(flags));
    msg.setItem("ret", std::to_string(ret));
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
    setLock();
    int ret = Oldrecv(s, buf, len, flags);
    Msg msg("recv");
    std::string buf_tmp(buf, ret);
    msg.setItem("s", std::to_string(s));
    msg.setItem("buf", buf_tmp);
    msg.setItem("len", std::to_string(len));
    msg.setItem("flags", std::to_string(flags));
    msg.setItem("ret", std::to_string(ret));
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

// DLLMain
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
        std::string msg = "{\n \"processID\": " + std::to_string(GetCurrentProcessId()) + ",\n";
        msg += " \"time\": \"" + getCurrentTime() + "\"\n}";
        client.send(msg.c_str());
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID &)OldMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID &)OldMessageBoxW, NewMessageBoxW);

        DetourAttach(&(PVOID &)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID &)OldWriteFile, NewWriteFile);
        DetourAttach(&(PVOID &)OldReadFile, NewReadFile);

        DetourAttach(&(PVOID &)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID &)OldHeapDestory, NewHeapDestory);
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
        // DetourAttach(&(PVOID&)Oldmemcpy, Newmemcpy);
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