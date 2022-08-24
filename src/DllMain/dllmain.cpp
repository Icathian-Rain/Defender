#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "winudp.h"

// lib
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")

// udp client
udp::UdpClient client("127.0.0.1", 3000);



static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;


// MessageBoxA
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(
	_In_opt_ HWND hWnd,			// 窗口句柄，代表这个消息窗口的所有者	
	_In_opt_ LPCSTR lpText,		// 填入窗口的字符串 char *
	_In_opt_ LPCSTR lpCaption,	// 填入窗口的标题	char *
	_In_ UINT uType				// button
)		
{
	
	// 返回原始接口
	client.send("MessageBoxA");
	return OldMessageBoxA(hWnd, lpText, lpCaption, uType);
}

// MessageBoxW
static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(
	_In_opt_ HWND hWnd,			// OldMessageBoxA
	_In_opt_ LPCWSTR lpText,	// 填入窗口的字符串 wchar *
	_In_opt_ LPCWSTR lpCaption,	// 填入窗口的标题	wchar *
	_In_ UINT uType				// button
)
{
	client.send("MessageBoxW");
	return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
}


// CreateFile
static HANDLE(WINAPI* OldCreateFile)(
	LPCWSTR               lpFileName,				// 打开文件名，宽字符
	DWORD                 dwDesiredAccess,			// 权限管理，读or 写
	DWORD                 dwShareMode,				// 文件是否共享访问
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,		// 文件的安全特性
	DWORD                 dwCreationDisposition,	// 创建or 打开
	DWORD                 dwFlagsAndAttributes,		// 文档属性
	HANDLE                hTemplateFile				// 如果不为0则指定一个文件句柄，复制扩展属性
	) = CreateFile;
extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
	LPCWSTR               lpFileName,				// 打开文件名，宽字符
	DWORD                 dwDesiredAccess,			// 权限管理，读or 写
	DWORD                 dwShareMode,				// 文件是否共享访问
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,		// 文件的安全特性
	DWORD                 dwCreationDisposition,	// 创建or 打开
	DWORD                 dwFlagsAndAttributes,		// 文档属性
	HANDLE                hTemplateFile				// 如果不为0则指定一个文件句柄，复制扩展属性
)
{
	client.send("CreateFile");
	return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

// WriteFile
static BOOL(WINAPI* OldWriteFile)(
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
	) = WriteFile;
extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
	HANDLE       hFile,							// 处理要写入的文件
	LPCVOID      lpBuffer,						// 指向要写入文件的数据的指针
	DWORD        nNumberOfBytesToWrite,			// 要写入的字节数
	LPDWORD      lpNumberOfBytesWritten,		// 指针写入的字节数
	LPOVERLAPPED lpOverlapped					// 指向重叠I/O结构的指针
)
{
	client.send("WriteFile");
	return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}



// ReadFile
static BOOL(WINAPI* OldReadFile)(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
	) = ReadFile;
extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(
	HANDLE       hFile,						// 处理要读取的文件
	LPVOID       lpBuffer,					// 指向要读处的数据的指针
	DWORD        nNumberOfBytesToRead,		// 要读取的字节数
	LPDWORD      lpNumberOfBytesRead,		// 读取的字节数
	LPOVERLAPPED lpOverlapped				// 指向重叠I/O结构的指针
)
{
	client.send("ReadFile");
	return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

// HeapCreate
static HANDLE(WINAPI* OldHeapCreate)(
	DWORD fIOoptions,		// 堆操作类型
	SIZE_T dwInitialSize,	// 开始时分给堆的字节数
	SIZE_T dwMaximumSize	// 堆最大能增长到的字节数
	) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(
	DWORD fIOoptions,		// 堆操作类型
	SIZE_T dwInitialSize,	// 开始时分给堆的字节数
	SIZE_T dwMaximumSize	// 堆最大OldHeapFree能增长到的字节数
)
{
	client.send("HeapCreate");
	return OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
}

// HeapDestroy
static BOOL(WINAPI* OldHeapDestory)(HANDLE) = HeapDestroy;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestory(
	HANDLE hHeap			// 要销毁堆的句柄
)
{
	client.send("HeapDestroy");
	return OldHeapDestory(hHeap);
}

bool isFree = false;
// HeapFree
static BOOL(WINAPI* OldHeapFree)(
	HANDLE hHeap,
	DWORD dwFlags,
	_Frees_ptr_opt_ LPVOID lpMem
	) = HeapFree;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(
	HANDLE hHeap,					// 要释放堆的句柄
	DWORD dwFlags,					// 堆释放选项
	_Frees_ptr_opt_ LPVOID lpMem	// 指向要释放的内存块的指针
) 
{
	if (isFree == false)
	{
		isFree = true;
		client.send("HeapFree");
	}
	return OldHeapFree(hHeap, dwFlags, lpMem);
}

bool isAlloc = false;
// HeapAlloc
static LPVOID(WINAPI* OldHeapAlloc)(
	HANDLE hHeap,		
	DWORD dwFlags,
	SIZE_T dwBytes
	) = HeapAlloc;
extern "C" __declspec(dllexport) LPVOID  WINAPI NewHeapAlloc(
	HANDLE hHeap,				// 分配堆的句柄
	DWORD dwFlags,				// 分配选项
	SIZE_T dwBytes				//	分配大小
)
{
	if (isAlloc == false)
	{
		isAlloc = true;
		client.send("HeapAlloc");
	}
	return OldHeapAlloc(hHeap, dwFlags, dwBytes);
}

// RegCreateKeyEx
static LSTATUS(WINAPI* OldRegCreateKeyEx)(
	HKEY                        hKey,					// 一个打开项的句柄
	LPCWSTR                     lpSubKey,				// 创建子项的名字
	DWORD                       Reserved,				// 0
	LPWSTR                      lpClass,				// 项的类名
	DWORD                       dwOptions,				// 选项 
	REGSAM                      samDesired,				// 定义访问权限
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,	// 指定要创建的秘钥的访问权限的掩码
	PHKEY                       phkResult,				// 指向接收到已打开或创建的秘钥句柄的变量的指针
	LPDWORD                     lpdwDisposition			// 出参
	) = RegCreateKeyEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyEx(
	HKEY                        hKey,
	LPCWSTR                     lpSubKey,
	DWORD                       Reserved,
	LPWSTR                      lpClass,
	DWORD                       dwOptions,
	REGSAM                      samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY                       phkResult,
	LPDWORD                     lpdwDisposition
) 
{	
	client.send("RegCreateKeyEx");
	return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

// RegSetValueEx
static LSTATUS(WINAPI* OldRegSetValueEx)(
	HKEY       hKey,				// 已经打开的项的句柄
	LPCWSTR    lpValueName,			// 设置值的名称 wchar
	DWORD      Reserved,			// 强制为0
	DWORD      dwType,				// 指定要保存数据的类型	
	const BYTE* lpData,				// 保存指定值名称的数据
	DWORD      cbData				// 数据的大小，单位为字节
	) = RegSetValueEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueEx(
	HKEY       hKey,
	LPCWSTR    lpValueName,
	DWORD      Reserved,
	DWORD      dwType,
	const BYTE * lpData,
	DWORD      cbData)
{
	client.send("RegSetValueEx");
	return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

// RegCloseKey
static LSTATUS(WINAPI* OldRegCloseKey)(
	HKEY hKey			// 关闭键的句柄
	) = RegCloseKey;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(
	HKEY hKey
)
{
	client.send("RegCloseKey");
	return OldRegCloseKey(hKey);
}

// RegOpenKeyEx
static LSTATUS(WINAPI* OldRegOpenKeyEx)(
	HKEY    hKey,				// 需要打开的主键的名称
	LPCWSTR lpSubKey,			// 需要打开的子键的名称
	DWORD   ulOptions,			// 保留， 0
	REGSAM  samDesired,			// 安全访问标记，权限
	PHKEY   phkResult			// 得到的将要打开键的句柄
	) = RegOpenKeyEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
	HKEY    hKey,
	LPCWSTR lpSubKey,
	DWORD   ulOptions,
	REGSAM  samDesired,
	PHKEY   phkResult)
{
	client.send("RegOpenKeyEx");
	return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}



// RegDeleteValue
static LSTATUS(WINAPI* OldRegDeleteValue)(
	HKEY    hKey,				// 打开项的句柄
	LPCWSTR lpValueName			// 要删除的值名
	) = RegDeleteValue;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValue(
	HKEY    hKey,
	LPCWSTR lpValueName)
{
	client.send("RegDeleteValue");
	return OldRegDeleteValue(hKey, lpValueName);
}

// socket
static SOCKET(WINAPI* Oldsocket)(
	int af,				// 地址系列
	int type,			// 类型, SOCK_STREAM基于TCP， SOCK_DGRAM基于UDP
	int protocol		// 协议号
	) = socket;
extern "C" __declspec(dllexport)SOCKET WINAPI Newsocket(int af, int type, int protocol) {
	client.send("socket");
	return Oldsocket(af, type, protocol);
}


// bind
static int (WINAPI* Oldbind)(
	SOCKET         s,			// 绑定目标的socket
	const sockaddr* name,		// 本地地址
	int            namelen		// 地址长度
	) = bind;
extern "C" __declspec(dllexport)int WINAPI Newbind(SOCKET s, const sockaddr * name, int namelen) {
	client.send("bind");
	return Oldbind(s, name, namelen);
}

// send
static int (WINAPI* Oldsend)(
	SOCKET     s,			// 源套接字
	const char* buf,		// 数据地址
	int        len,			// 数据长度
	int        flags		// 0
	) = send;
extern "C" __declspec(dllexport)int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags) {
	client.send("send");
	return Oldsend(s, buf, len, flags);
}

// recv
static int (WINAPI* Oldrecv)(
	SOCKET     s,			// 源地址socket
	char* buf,				// 存储接收数据的地址
	int        len,			// 数据长度
	int        flags		// 传送数据的类型
	) = recv;
extern "C" __declspec(dllexport)int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags) {
	client.send("recv");
	return Oldrecv(s, buf, len, flags);
}

// connect
static int (WINAPI* Oldconnect)(
	SOCKET         s,			// 源socket
	const sockaddr* name,		// 目的地址
	int            namelen		// 目的地址长度
	) = connect;
extern "C" __declspec(dllexport)int WINAPI Newconnect(SOCKET s, const sockaddr * name, int namelen) {
	client.send("connect");
	return Oldconnect(s, name, namelen);
}


// memcpy
static void* (__cdecl* Oldmemcpy)(
	void* _Dst,				// 目的地址
	const void* _Src,		// 源地址
	size_t _Size			// 大小
) = memcpy;
extern "C" __declspec(dllexport)void* __cdecl Newmemcpy(void* _Dst, const void* _Src, size_t _Size) {
	client.send("memcpy");
	return Oldmemcpy(_Dst, _Src, _Size);
}



// DLLMain
BOOL WINAPI DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
)
{
	//wchar_t FileName[256];
	//GetModuleFileName(0, (LPWSTR)FileName, 256);
	//printf("文件名：%c%c%c%c：文件名", FileName[0], FileName[1], FileName[2], FileName[3]);
	//if (wcsstr(FileName, L"syringe.exe")) {
	//	return true;
	//}
	//wprintf(L"%ls\n", FileName);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
		DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
		DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
		DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
		DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
		DetourAttach(&(PVOID&)OldHeapDestory, NewHeapDestory);
		DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
		DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
		DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
		DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
		DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
		DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
		DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
		DetourAttach(&(PVOID&)Oldsocket, Newsocket);
		DetourAttach(&(PVOID&)Oldbind, Newbind);
		DetourAttach(&(PVOID&)Oldsend, Newsend);
		DetourAttach(&(PVOID&)Oldconnect, Newconnect);
		DetourAttach(&(PVOID&)Oldrecv, Newrecv);
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
		DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
		DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
		DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
		DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
		DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
		DetourDetach(&(PVOID&)OldHeapDestory, NewHeapDestory);
		DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
		DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
		DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
		DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
		DetourDetach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
		DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
		DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
		DetourDetach(&(PVOID&)Oldsocket, Newsocket);
		DetourDetach(&(PVOID&)Oldbind, Newbind);
		DetourDetach(&(PVOID&)Oldsend, Newsend);
		DetourDetach(&(PVOID&)Oldconnect, Newconnect);
		DetourDetach(&(PVOID&)Oldrecv, Newrecv);
		//DetourDetach(&(PVOID&)Oldmemcpy, Newmemcpy);
		DetourTransactionCommit();
		break;
	}
	}
	return true;
}