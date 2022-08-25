#include<iostream>
#include<cstdio>
#include<windows.h>
#include<detours.h>
#include<thread>
#pragma comment(lib, "detours.lib")
using namespace std;
int main(int argc, char* argv[]) {
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

	// 路径配置
	WCHAR DirPath[MAX_PATH + 1];
	/* DllMain x64
	wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\22057\\Desktop\\softwareSecurity\\src\\DLLMain\\x64\\Debug");	// dll文件夹
	char DLLPath[MAX_PATH + 1] = "C:\\Users\\22057\\Desktop\\softwareSecurity\\src\\DLLMain\\x64\\Debug\\DllMain.dll"; // dll的地址
	
	*/
	wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\22057\\Desktop\\softwareSecurity\\src\\DLLMain\\Debug");	// dll文件夹
	char DLLPath[MAX_PATH + 1] = "C:\\Users\\22057\\Desktop\\softwareSecurity\\src\\DLLMain\\Debug\\DllMain.dll"; // dll的地址
	

	WCHAR EXE[MAX_PATH + 1] = { 0 };
	swprintf_s(EXE, MAX_PATH, L"%hs", argv[0]);
	// wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\22057\\Desktop\\softwareSecurity\\src\\testEXE\\X64\\Debug\\testEXE.exe");
	// wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\22057\\Desktop\\softwareSecurity\\MFCApplication2.exe");
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