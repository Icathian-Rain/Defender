// 头文件
#include<iostream>
#include<cstdio>
#include<windows.h>
#include<thread>
// detours库
#include<detours.h>

// 链接库
#pragma comment(lib, "detours.lib")

using namespace std;
int main(int argc, char* argv[]) {
    // 受限于go的os.startProcess函数，参数个数为1，即为要注入的测试程序路径
	if (argc != 1)
	{
		return -1;
	}
	// 基本配置
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);

    // 获取当前路径
	WCHAR DirPath[MAX_PATH + 1];
	GetCurrentDirectoryW(MAX_PATH, DirPath);

    // 获取DLL路径，DLL应在当前路径下
	char DLLPath[MAX_PATH + 1];
    GetCurrentDirectoryA(MAX_PATH, DLLPath);
    strcat_s(DLLPath, MAX_PATH, "\\DllMain.dll");

    // 获取测试程序路径
	WCHAR EXE[MAX_PATH + 1] = { 0 };
	swprintf_s(EXE, MAX_PATH, L"%hs", argv[0]);

    // 启动测试程序
	if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath,
		&si, &pi, DLLPath, NULL)) {
		// 恢复线程
		ResumeThread(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	else {
		char error[100];
		DWORD err = GetLastError();
		sprintf_s(error, sizeof(err)+1, "%d", err);
	}
	return 0;
}