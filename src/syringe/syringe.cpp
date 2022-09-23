// 头文件
#include<iostream>
#include<cstdio>
#include<windows.h>
#include<thread>
// detours库
#include<detours.h>

// 链接库
#pragma comment(lib, "detours.lib")

std::string getDir(std::string path) {
    int pos = path.find_last_of("\\");
    return path.substr(0, pos);
}

void string2wchar(std::string str, wchar_t* wchar) {
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wchar, len);
}


using namespace std;
int main(int argc, char* argv[]) {
    // 受限于go的os.startProcess函数，参数个数为2，0为dll路径，1为目标程序路径
	if (argc != 2)
	{
		return -1;
	}
	// 基本配置
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);


    // 获取DLL路径，DLL路径为argv[0]
	char DLLPath[MAX_PATH + 1];
    sprintf_s(DLLPath, "%s", argv[0]);

    // 获取测试程序路径,测试程序路径为argv[1]
	WCHAR EXE[MAX_PATH + 1] = { 0 };
	swprintf_s(EXE, MAX_PATH, L"%hs", argv[1]);

    // 获取测试程序所在目录
    std::string dir = getDir(argv[1]);
	WCHAR DirPath[MAX_PATH + 1];
	swprintf_s(DirPath, MAX_PATH, L"%hs", dir.c_str());

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