
#pragma once

/*
    HOOK File
    HOOK API List: CreateFile, ReadFile, WriteFile, CloseHandle
*/
#include <atlstr.h>
#include "lock.h"
#include "lock.h"
#include "utils.h"
#include "winudp.h"
#include "msg.h"

extern udp::UdpClient client;


// HooK CreateFile
static HANDLE(WINAPI *OldCreateFile)(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,                 
    HANDLE hTemplateFile                        
    ) = CreateFile;


/// @brief newCreateFile
/// @param lpFileName 打开文件名，宽字符
/// @param dwDesiredAccess 权限管理，读or 写
/// @param dwShareMode 文件是否共享访问
/// @param lpSecurityAttributes 文件的安全特性
/// @param dwCreationDisposition 创建or 打开
/// @param dwFlagsAndAttributes 文档属性
/// @param hTemplateFile 如果不为0则指定一个文件句柄，复制扩展属性
/// @return ret
extern "C" __declspec(dllexport) HANDLE WINAPI NewCreateFile(
    LPCWSTR lpFileName,                         
    DWORD dwDesiredAccess,                       
    DWORD dwShareMode,                          
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
    DWORD dwCreationDisposition,                 
    DWORD dwFlagsAndAttributes,                  
    HANDLE hTemplateFile                        
)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    HANDLE ret = OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    // 类型转换
    std::string lpFileName_tmp = wchar2string(lpFileName);
    std::string dwDesiredAccess_tmp = dword2hex(dwDesiredAccess);
    std::string dwShareMode_tmp = dword2hex(dwShareMode);
    std::string lpSecurityAttributes_tmp = dword2hex((DWORD)lpSecurityAttributes);
    std::string dwCreationDisposition_tmp = dword2hex(dwCreationDisposition);
    std::string dwFlagsAndAttributes_tmp = dword2hex(dwFlagsAndAttributes);
    std::string hTemplateFile_tmp = dword2hex((DWORD)hTemplateFile);
    std::string ret_tmp = dword2hex((DWORD)ret);
    // 构造消息
    Msg msg("CreateFile");
    msg.setItem("lpFileName", lpFileName_tmp);
    msg.setItem("dwDesiredAccess", dwDesiredAccess_tmp);
    msg.setItem("dwShareMode", dwShareMode_tmp);
    msg.setItem("lpSecurityAttributes", lpSecurityAttributes_tmp);
    msg.setItem("dwCreationDisposition", dwCreationDisposition_tmp);
    msg.setItem("dwFlagsAndAttributes", dwFlagsAndAttributes_tmp);
    msg.setItem("hTemplateFile", hTemplateFile_tmp);
    msg.setItem("ret", ret_tmp);
    // 发送消息
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
/// @brief newWriteFile
/// @param hFile 处理要写入的文件
/// @param lpBuffer 指向要写入文件的数据的指针
/// @param nNumberOfBytesToWrite 要写入的字节数
/// @param lpNumberOfBytesWritten 指针写入的字节数
/// @param lpOverlapped 指向重叠I/O结构的指针
/// @return ret
extern "C" __declspec(dllexport) BOOL WINAPI NewWriteFile(
    HANDLE hFile,
    LPCVOID lpBuffer, 
    DWORD nNumberOfBytesToWrite,     
    LPDWORD lpNumberOfBytesWritten,  
    LPOVERLAPPED lpOverlapped
)
{
    // heap操作上锁
    setLock();
    // 进行函数操作
    BOOL ret = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    // 类型转换
    std::string hFile_tmp = dword2hex((DWORD)hFile);
    // 文件内容为字符串
    std::string lpBuffer_tmp((char *)lpBuffer, nNumberOfBytesToWrite);
    lpBuffer_tmp = GbkToUtf8(lpBuffer_tmp.c_str());
    // 文件内容为二进制
    // std::string lpBuffer_tmp = bytes2string((const BYTE*)lpBuffer, nNumberOfBytesToWrite);
    std::string nNumberOfBytesToWrite_tmp = dword2hex(nNumberOfBytesToWrite);
    std::string lpNumberOfBytesWritten_tmp;
    if (lpNumberOfBytesWritten != NULL)
    {
        lpNumberOfBytesWritten_tmp = dword2hex(*lpNumberOfBytesWritten);
    }
    else
    {
        lpNumberOfBytesWritten_tmp = "NULL";
    }
    std::string lpOverlapped_tmp = dword2hex((DWORD)lpOverlapped);
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("WriteFile");
    msg.setItem("hFile", hFile_tmp);
    msg.setItem("lpBuffer", lpBuffer_tmp);
    msg.setItem("nNumberOfBytesToWrite", nNumberOfBytesToWrite_tmp);
    msg.setItem("lpNumberOfBytesWritten", lpNumberOfBytesWritten_tmp);
    msg.setItem("lpOverlapped", lpOverlapped_tmp);
    msg.setItem("ret", ret_tmp);
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
/// @brief newReadFile
/// @param hFile 处理要读取的文件
/// @param lpBuffer 保存数据的指针
/// @param nNumberOfBytesToRead 要读取的字节数
/// @param lpNumberOfBytesRead 读取的字节数
/// @param lpOverlapped 指向重叠I/O结构的指针
/// @return 
extern "C" __declspec(dllexport) BOOL WINAPI NewReadFile(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped 
)
{
    // heap操作上锁
    setLock();
    BOOL ret = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);    
    // 类型转换
    std::string hFile_tmp = dword2hex((DWORD)hFile);
    std::string lpBuffer_tmp;
    std::string lpNumberOfBytesRead_tmp;
    if(lpNumberOfBytesRead != NULL)
    {
        // 文件内容为字符串
        lpBuffer_tmp = std::string((char *)lpBuffer, *lpNumberOfBytesRead);
        lpBuffer_tmp = GbkToUtf8(lpBuffer_tmp.c_str());
        // 文件内容为二进制
        // std::string lpBuffer_tmp = bytes2string((const BYTE*)lpBuffer, nNumberOfBytesToRead);
        lpNumberOfBytesRead_tmp = dword2hex(*lpNumberOfBytesRead);
    }
    else
    {
        // 文件内容为字符串
        lpBuffer_tmp = std::string((char *)lpBuffer);
        lpBuffer_tmp = GbkToUtf8(lpBuffer_tmp.c_str());
        // 文件内容为二进制
        // std::string lpBuffer_tmp = bytes2string((const BYTE*)lpBuffer);
        lpNumberOfBytesRead_tmp = "NULL";
    }
    std::string nNumberOfBytesToRead_tmp = std::to_string(nNumberOfBytesToRead);
    std::string lpOverlapped_tmp;
    if(lpOverlapped != NULL)
    {
        lpOverlapped_tmp = dword2hex((DWORD)lpOverlapped);
    }
    else
    {
        lpOverlapped_tmp = "NULL";
    }
    std::string ret_tmp = std::to_string(ret);
    // 构造消息
    Msg msg("ReadFile");
    msg.setItem("hFile", hFile_tmp);
    msg.setItem("lpBuffer", lpBuffer_tmp);
    msg.setItem("nNumberOfBytesToRead", nNumberOfBytesToRead_tmp);
    msg.setItem("lpNumberOfBytesRead", lpNumberOfBytesRead_tmp);
    msg.setItem("lpOverlapped", lpOverlapped_tmp);
    msg.setItem("ret", ret_tmp);
    // 发送消息
    client.send(msg.getMsg().c_str());
    // heap操作解锁
    releaseLock();
    return ret;
}