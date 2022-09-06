// Headers
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "winudp.h"
#include <sstream>

// lib
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")

// udp client
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
    std::ostringstream oss;
    oss << "funcName\nMessageBoxA\n"
        << "hWnd\n"
        << hWnd << "\n"
        << "lpText\n"
        << lpText << "\n"
        << "lpCaption\n"
        << lpCaption << "\n"
        << "uType\n"
        << uType << "\n";
    client.send(oss.str().c_str());
    return OldMessageBoxA(hWnd, lpText, lpCaption, uType);
}

// MessageBoxW
static int(WINAPI *OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(
    _In_opt_ HWND hWnd,         // OldMessageBoxA
    _In_opt_ LPCWSTR lpText,    // 填入窗口的字符串 wchar *
    _In_opt_ LPCWSTR lpCaption, // 填入窗口的标题	wchar *
    _In_ UINT uType             // button
)
{
    std::ostringstream oss;
    oss << "funcName\nMessageBoxW\n"
        << "hWnd\n"
        << hWnd << "\n"
        << "lpText\n"
        << lpText << "\n"
        << "lpCaption\n"
        << lpCaption << "\n"
        << "uType\n"
        << uType << "\n";
    client.send(oss.str().c_str());
    return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
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
    std::ostringstream oss;
    oss << "funcName\nCreateFile\n"
        << "lpFileName\n"
        << lpFileName << "\n"
        << "dwDesiredAccess\n"
        << dwDesiredAccess << "\n"
        << "dwShareMode\n"
        << dwShareMode << "\n"
        << "lpSecurityAttributes\n"
        << lpSecurityAttributes << "\n"
        << "dwCreationDisposition\n"
        << dwCreationDisposition << "\n"
        << "dwFlagsAndAttributes\n"
        << dwFlagsAndAttributes << "\n"
        << "hTemplateFile\n"
        << hTemplateFile << "\n";
    return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
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
    std::ostringstream oss;
    oss << "funcName\nWriteFile\n"
        << "hFile\n"
        << hFile << "\n"
        << "lpBuffer\n"
        << lpBuffer << "\n"
        << "nNumberOfBytesToWrite\n"
        << nNumberOfBytesToWrite << "\n"
        << "lpNumberOfBytesWritten\n"
        << lpNumberOfBytesWritten << "\n"
        << "lpOverlapped\n"
        << lpOverlapped << "\n";
    return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
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
    LPVOID lpBuffer,             // 指向要读处的数据的指针
    DWORD nNumberOfBytesToRead,  // 要读取的字节数
    LPDWORD lpNumberOfBytesRead, // 读取的字节数
    LPOVERLAPPED lpOverlapped    // 指向重叠I/O结构的指针
)
{
    std::ostringstream oss;
    oss << "funcName\nReadFile\n"
        << "hFile\n"
        << hFile << "\n"
        << "lpBuffer\n"
        << lpBuffer << "\n"
        << "nNumberOfBytesToRead\n"
        << nNumberOfBytesToRead << "\n"
        << "lpNumberOfBytesRead\n"
        << lpNumberOfBytesRead << "\n"
        << "lpOverlapped\n"
        << lpOverlapped << "\n";
    return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
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
    std::ostringstream oss;
    oss << "funcName\nHeapCreate\n"
        << "fIOoptions\n"
        << fIOoptions << "\n"
        << "dwInitialSize\n"
        << dwInitialSize << "\n"
        << "dwMaximumSize\n"
        << dwMaximumSize << "\n";
    return OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
}

// HeapDestroy
static BOOL(WINAPI *OldHeapDestory)(HANDLE) = HeapDestroy;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestory(
    HANDLE hHeap // 要销毁堆的句柄
)
{
    std::ostringstream oss;
    oss << "funcName\nHeapDestroy\n"
        << "hHeap\n"
        << hHeap << "\n";
    return OldHeapDestory(hHeap);
}

bool isAlloc = false;
// HeapAlloc
static LPVOID(WINAPI *OldHeapAlloc)(
    HANDLE hHeap,
    DWORD dwFlags,
    SIZE_T dwBytes) = HeapAlloc;
extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(
    HANDLE hHeap,  // 分配堆的句柄
    DWORD dwFlags, // 分配选项
    SIZE_T dwBytes //	分配大小
)
{
    if (isAlloc == false)
    {
        isAlloc = true;
        std::ostringstream oss;
        oss << "funcName\nHeapAlloc\n"
            << "hHeap\n"
            << hHeap << "\n"
            << "dwFlags\n"
            << dwFlags << "\n"
            << "dwBytes\n"
            << dwBytes << "\n";
        client.send(oss.str().c_str());
    }
    else
        isAlloc = false;
    return OldHeapAlloc(hHeap, dwFlags, dwBytes);
}

bool isFree = false;
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
    if (isFree == false)
    {
        isFree = true;
        std::ostringstream oss;
        oss << "funcName\nHeapFree\n"
            << "hHeap\n"
            << hHeap << "\n"
            << "dwFlags\n"
            << dwFlags << "\n"
            << "lpMem\n"
            << lpMem << "\n";
        client.send(oss.str().c_str());
    }
    else
        isFree = false;
    return OldHeapFree(hHeap, dwFlags, lpMem);
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
    std::ostringstream oss;
    oss << "funcName\nRegCreateKeyEx\n"
        << "hKey\n"
        << hKey << "\n"
        << "lpSubKey\n"
        << lpSubKey << "\n"
        << "Reserved\n"
        << Reserved << "\n"
        << "lpClass\n"
        << lpClass << "\n"
        << "dwOptions\n"
        << dwOptions << "\n"
        << "samDesired\n"
        << samDesired << "\n"
        << "lpSecurityAttributes\n"
        << lpSecurityAttributes << "\n"
        << "phkResult\n"
        << phkResult << "\n"
        << "lpdwDisposition\n"
        << lpdwDisposition << "\n";
    client.send(oss.str().c_str());
    return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
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
    std::ostringstream oss;
    oss << "funcName\nRegSetValueEx\n"
        << "hKey\n"
        << hKey << "\n"
        << "lpValueName\n"
        << lpValueName << "\n"
        << "Reserved\n"
        << Reserved << "\n"
        << "dwType\n"
        << dwType << "\n"
        << "lpData\n"
        << lpData << "\n"
        << "cbData\n"
        << cbData << "\n";
    client.send(oss.str().c_str());
    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

// RegOpenKeyEx
static LSTATUS(WINAPI *OldRegOpenKeyEx)(
    HKEY hKey,         // 需要打开的主键的名称
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
    std::ostringstream oss;
    oss << "funcName\nRegOpenKeyEx\n"
        << "hKey\n"
        << hKey << "\n"
        << "lpSubKey\n"
        << lpSubKey << "\n"
        << "ulOptions\n"
        << ulOptions << "\n"
        << "samDesired\n"
        << samDesired << "\n"
        << "phkResult\n"
        << phkResult << "\n";
    client.send(oss.str().c_str());
    return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

// RegCloseKey
static LSTATUS(WINAPI *OldRegCloseKey)(
    HKEY hKey // 关闭键的句柄
    ) = RegCloseKey;
extern "C" __declspec(dllexport) LSTATUS WINAPI NewRegCloseKey(
    HKEY hKey)
{
    std::ostringstream oss;
    oss << "funcName\nRegCloseKey\n"
        << "hKey\n"
        << hKey << "\n";
    client.send(oss.str().c_str());
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
    std::ostringstream oss;
    oss << "funcName\nRegDeleteKey\n"
        << "hKey\n"
        << hKey << "\n"
        << "lpSubKey\n"
        << lpSubKey << "\n";
    client.send(oss.str().c_str());
    return OldRegDeleteKey(hKey, lpSubKey);
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
    std::ostringstream oss;
    oss << "funcName\nRegDeleteValue\n"
        << "hKey\n"
        << hKey << "\n"
        << "lpValueName\n"
        << lpValueName << "\n";
    client.send(oss.str().c_str());
    return OldRegDeleteValue(hKey, lpValueName);
}

// socket
static SOCKET(WINAPI *Oldsocket)(
    int af,      // 地址系列
    int type,    // 类型, SOCK_STREAM基于TCP， SOCK_DGRAM基于UDP
    int protocol // 协议号
    ) = socket;
extern "C" __declspec(dllexport) SOCKET WINAPI Newsocket(int af, int type, int protocol)
{
    std::ostringstream oss;
    oss << "funcName\nsocket\n"
        << "af\n"
        << af << "\n"
        << "type\n"
        << type << "\n"
        << "protocol\n"
        << protocol << "\n";
    client.send(oss.str().c_str());
    return Oldsocket(af, type, protocol);
}

// bind
static int(WINAPI *Oldbind)(
    SOCKET s,             // 绑定目标的socket
    const sockaddr *name, // 本地地址
    int namelen           // 地址长度
    ) = bind;
extern "C" __declspec(dllexport) int WINAPI Newbind(SOCKET s, const sockaddr *name, int namelen)
{
    std::ostringstream oss;
    oss << "funcName\nbind\n"
        << "s\n"
        << s << "\n"
        << "name\n"
        << name << "\n"
        << "namelen\n"
        << namelen << "\n";
    client.send(oss.str().c_str());
    return Oldbind(s, name, namelen);
}

// listen
static int(WINAPI *Oldlisten)(
    SOCKET s,   // 监听的socket
    int backlog // 最大连接数
    ) = listen;
extern "C" __declspec(dllexport) int WINAPI Newlisten(SOCKET s, int backlog)
{
    std::ostringstream oss;
    oss << "funcName\nlisten\n"
        << "s\n"
        << s << "\n"
        << "backlog\n"
        << backlog << "\n";
    client.send(oss.str().c_str());
    return Oldlisten(s, backlog);
}

// accpet
static SOCKET(WINAPI *Oldaccept)(
    SOCKET s,       // 监听的socket
    sockaddr *addr, // 客户端地址
    int *addrlen    // 地址长度
    ) = accept;
extern "C" __declspec(dllexport) SOCKET WINAPI Newaccept(SOCKET s, sockaddr *addr, int *addrlen)
{
    std::ostringstream oss;
    oss << "funcName\naccept\n"
        << "s\n"
        << s << "\n"
        << "addr\n"
        << addr << "\n"
        << "addrlen\n"
        << addrlen << "\n";
    client.send(oss.str().c_str());
    return Oldaccept(s, addr, addrlen);
}

// connect
static int(WINAPI *Oldconnect)(
    SOCKET s,             // 源socket
    const sockaddr *name, // 目的地址
    int namelen           // 目的地址长度
    ) = connect;
extern "C" __declspec(dllexport) int WINAPI Newconnect(SOCKET s, const sockaddr *name, int namelen)
{
    std::ostringstream oss;
    oss << "funcName\nconnect\n"
        << "s\n"
        << s << "\n"
        << "name\n"
        << name << "\n"
        << "namelen\n"
        << namelen << "\n";
    client.send(oss.str().c_str());
    return Oldconnect(s, name, namelen);
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
    std::ostringstream oss;
    oss << "funcName\nsend\n"
        << "s\n"
        << s << "\n"
        << "buf\n"
        << buf << "\n"
        << "len\n"
        << len << "\n"
        << "flags\n"
        << flags << "\n";
    client.send(oss.str().c_str());
    return Oldsend(s, buf, len, flags);
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
    std::ostringstream oss;
    oss << "funcName\nrecv\n"
        << "s\n"
        << s << "\n"
        << "buf\n"
        << buf << "\n"
        << "len\n"
        << len << "\n"
        << "flags\n"
        << flags << "\n";
    client.send(oss.str().c_str());
    return Oldrecv(s, buf, len, flags);
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
    std::ostringstream oss;
    oss << "funcName\nsendto\n"
        << "s\n"
        << s << "\n"
        << "buf\n"
        << buf << "\n"
        << "len\n"
        << len << "\n"
        << "flags\n"
        << flags << "\n"
        << "to\n"
        << to << "\n"
        << "tolen\n"
        << tolen << "\n";
    client.send(oss.str().c_str());
    return Oldsendto(s, buf, len, flags, to, tolen);
}

// recvfrom
static int(WINAPI *Oldrecvfrom)(
    SOCKET s,           // 源地址socket
    char *buf,          // 存储接收数据的地址
    int len,            // 数据长度
    int flags,          // 传送数据的类型
    sockaddr *from,     // 源地址
    int *fromlen        // 源地址长度
    ) = recvfrom;
extern "C" __declspec(dllexport) int WINAPI Newrecvfrom(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen)
{
    std::ostringstream oss;
    oss << "funcName\nrecvfrom\n"
        << "s\n"
        << s << "\n"
        << "buf\n"
        << buf << "\n"
        << "len\n"
        << len << "\n"
        << "flags\n"
        << flags << "\n"
        << "from\n"
        << from << "\n"
        << "fromlen\n"
        << fromlen << "\n";
    client.send(oss.str().c_str());
    return Oldrecvfrom(s, buf, len, flags, from, fromlen);
}

// memcpy
// static void *(__cdecl *Oldmemcpy)(
//     void *_Dst,       // 目的地址
//     const void *_Src, // 源地址
//     size_t _Size      // 大小
//     ) = memcpy;
// extern "C" __declspec(dllexport) void *__cdecl Newmemcpy(void *_Dst, const void *_Src, size_t _Size)
// {
//     std::ostringstream oss;
//     oss << "funcName\nmemcpy\n"
//         << "_Dst\n"
//         << _Dst << "\n"
//         << "_Src\n"
//         << _Src << "\n"
//         << "_Size\n"
//         << _Size << "\n";
//     client.send(oss.str().c_str());
//     return Oldmemcpy(_Dst, _Src, _Size);
// }

// DLLMain
BOOL WINAPI DllMain(HMODULE hModule,
                    DWORD ul_reason_for_call,
                    LPVOID lpReserved)
{
    // wchar_t FileName[256];
    // GetModuleFileName(0, (LPWSTR)FileName, 256);
    // printf("文件名：%c%c%c%c：文件名", FileName[0], FileName[1], FileName[2], FileName[3]);
    // if (wcsstr(FileName, L"syringe.exe")) {
    //	return true;
    // }
    // wprintf(L"%ls\n", FileName);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
		
		DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
		DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
		DetourAttach(&(PVOID&)OldReadFile, NewReadFile);

		DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
		DetourAttach(&(PVOID&)OldHeapDestory, NewHeapDestory);
		// DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
		// DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

		// DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
		// DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        // DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
		// DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        // DetourAttach(&(PVOID&)OldRegDeleteKey, NewRegDeleteKey);
		// DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
		

		DetourAttach(&(PVOID&)Oldsocket, Newsocket);
        DetourAttach(&(PVOID&)Oldbind, Newbind);
        DetourAttach(&(PVOID&)Oldlisten, Newlisten);
        DetourAttach(&(PVOID&)Oldaccept, Newaccept);
		DetourAttach(&(PVOID&)Oldconnect, Newconnect);
		DetourAttach(&(PVOID&)Oldsend, Newsend);
		DetourAttach(&(PVOID&)Oldrecv, Newrecv);
        // DetourAttach(&(PVOID&)Oldsendto, Newsendto);
        // DetourAttach(&(PVOID&)Oldrecvfrom, Newrecvfrom);
        // DetourAttach(&(PVOID&)Oldmemcpy, Newmemcpy);
        DetourTransactionCommit();
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
		
		DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
		DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
		DetourAttach(&(PVOID&)OldReadFile, NewReadFile);

		DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
		DetourAttach(&(PVOID&)OldHeapDestory, NewHeapDestory);
		// DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
		// DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

		// DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
		// DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        // DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
		// DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        // DetourAttach(&(PVOID&)OldRegDeleteKey, NewRegDeleteKey);
		// DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
		

		DetourAttach(&(PVOID&)Oldsocket, Newsocket);
        DetourAttach(&(PVOID&)Oldbind, Newbind);
        DetourAttach(&(PVOID&)Oldlisten, Newlisten);
        DetourAttach(&(PVOID&)Oldaccept, Newaccept);
		DetourAttach(&(PVOID&)Oldconnect, Newconnect);
		DetourAttach(&(PVOID&)Oldsend, Newsend);
		DetourAttach(&(PVOID&)Oldrecv, Newrecv);
        // DetourAttach(&(PVOID&)Oldsendto, Newsendto);
        // DetourAttach(&(PVOID&)Oldrecvfrom, Newrecvfrom);
		// DetourDetach(&(PVOID&)Oldmemcpy, Newmemcpy);
        DetourTransactionCommit();
        break;
    }
    }
    return true;
}