/*
Normal Operation: 
1. MessageBox
2. heap Create Alloc Free Destroy
3. File New Open Close Read Write
4. Registry Create Open Close Read Write

Dangerous Operation:
1. Heap
2. File 
3. Registry

Optional Operation:
1. socket 
2. memory cpoy
*/
// 头文件
#include <Windows.h>
#include <iostream>
#include <stdlib.h>

#define PAGE_SIZE 4096
#pragma comment(lib, "ws2_32.lib")

void showMenu()
{
    printf("----------------------------------------------------\n");
    printf("选择功能:\n");
    printf("1.MessageBoxA\n");
    printf("2.MessageBoxW\n");
    printf("3.堆操作\n");
    printf("4.写文件操作\n");
    printf("5.读文件操作\n");
    printf("6.注册表写操作\n");
    printf("7.注册表读操作\n");
    printf("8.注册表删除操作\n");

}

// 堆操作
void heapCreatAndDestory()
{
    getchar();
    printf("按任意键开始创建堆\n");
    getchar();
    HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE*10, PAGE_SIZE*100);
    if (hHeap == NULL)
    {
        printf("创建堆失败\n");
        return;
    }
    printf("创建堆成功\n");
    printf("按任意键开始释放堆\n");
    getchar();
    int *pArr = (int *)HeapAlloc(hHeap, 0, sizeof(int) * 30);
    if (pArr == NULL)
    {
        printf("分配内存失败\n");
        return;
    }
    printf("分配内存成功\n");
    for (int i = 0; i < 30; i++)
    {
        pArr[i] = i;
    }
    printf("按任意键开始释放内存\n");
    getchar();
    HeapFree(hHeap, 0, pArr);
    printf("释放内存成功\n");
    printf("按任意键开始释放堆\n");
    getchar();
    HeapDestroy(hHeap);
    printf("释放堆成功\n");
    return ;
}


// 文件操作
void writeFile()
{
    char* buffer;
    HANDLE hFile = (HANDLE)CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("创建文件失败\n");
        return;
    }
    printf("创建文件成功\n");
    buffer = (char *)malloc(sizeof(char) * 100);
    strcpy_s(buffer, strlen("hello world") + 1, "hello world");
    if(!WriteFile(hFile, buffer, strlen(buffer) + 1, NULL, NULL))
    {
        printf("写文件失败\n");
        return;
    }
    printf("写文件成功\n");
    free(buffer);
    FlushFileBuffers(hFile);
    CloseHandle(hFile);
}

void readFile()
{
    char* buffer;
    HANDLE hFile = (HANDLE)CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败\n");
        return;
    }
    printf("打开文件成功\n");
    buffer = (char *)malloc(sizeof(char) * 100);
    if (!ReadFile(hFile, buffer, 100, NULL, NULL))
    {
        printf("读文件失败\n");
        return;
    }
    printf("读文件成功\n");
    printf("文件内容为:");
    printf("%s\n", buffer);
    free(buffer);
    CloseHandle(hFile);
}

// 注册表操作
void regCreateAndSet() 
{
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    {
        printf("创建键失败\n");
        return;
    }
    printf("创建键成功\n");
    if (RegSetValueEx(hKey, L"test", 0, REG_SZ, (BYTE *)"hello world", strlen("hello world") + 1) != ERROR_SUCCESS)
    {
        printf("设置值失败\n");
        return;
    }
    printf("设置值成功\n");
    RegCloseKey(hKey);
}

void regOpenAndGet()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        printf("打开键失败\n");
        return;
    }
    printf("打开键成功\n");
    char* buffer;
    DWORD dwSize = 0;
    if (RegQueryValueEx(hKey, L"test", 0, NULL, NULL, &dwSize) != ERROR_SUCCESS)
    {
        printf("读取值失败\n");
        return;
    }
    printf("读取值成功\n");
    buffer = (char *)malloc(sizeof(char) * dwSize);
    if (RegQueryValueEx(hKey, L"test", 0, NULL, (BYTE *)buffer, &dwSize) != ERROR_SUCCESS)
    {
        printf("读取值失败\n");
        return;
    }
    printf("读取值成功\n");
    printf("值为:");
    printf("%s\n", buffer);
    free(buffer);
    RegCloseKey(hKey);
}

void regOpenAndDelete()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        printf("打开键失败\n");
        return;
    }
    printf("打开键成功\n");
    if (RegDeleteValue(hKey, L"test") != ERROR_SUCCESS)
    {
        printf("删除值失败\n");
        return;
    }
    printf("删除值成功\n");
    RegCloseKey(hKey);
}

// dangerous Operation
void modifyEXE() 
{
    HANDLE hOpenFile = (HANDLE)CreateFile(L"test.exe", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hOpenFile == INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败\n");
        return;
    }
    printf("打开文件成功\n");
    CloseHandle(hOpenFile);
}


void selfCopy()
{
    HANDLE hFile = (HANDLE)CreateFile(L"testCode.exe", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("创建文件失败\n");
        return;
    }
    printf("创建文件成功\n");
    CloseHandle(hFile);
}

void modifyStartup()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        printf("打开键失败\n");
        return;
    }
    printf("打开键成功\n");
    if (RegSetValueEx(hKey, L"test", 0, REG_SZ, (BYTE *)"testCode.exe", strlen("testCode.exe") + 1) != ERROR_SUCCESS)
    {
        printf("设置值失败\n");
        return;
    }
    printf("设置值成功\n");
    RegCloseKey(hKey);
}

void deleteStartup()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        printf("打开键失败\n");
        return;
    }
    printf("打开键成功\n");
    if (RegDeleteValue(hKey, L"test") != ERROR_SUCCESS)
    {
        printf("删除值失败\n");
        return;
    }
    printf("删除值成功\n");
    RegCloseKey(hKey);
}

void openAnotherFolder() {
    HANDLE hOpenFIle = (HANDLE)CreateFile(L"..\\test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hOpenFIle == INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败\n");
        return;
    }
    printf("打开文件成功\n");
    CloseHandle(hOpenFIle);
}

void recvData()
{
    SCOKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(2000);
    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) != 0)
    {
        printf("连接失败\n");
        return;
    }
    printf("连接成功\n");
    char buffer[1024];
    int len = recv(sock, buffer, sizeof(buffer), 0);
    if (len == SOCKET_ERROR)
    {
        printf("接收数据失败\n");
        return;
    }
    printf("接收数据成功\n");
    printf("接收到的数据为:");
    printf("%s\n", buffer);
    closesocket(sock);
}

void sendData()
{
    SCOKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(2000);
    
}


int main()
{
    // std::cout << "Hello World!\n";
    int flag = 0;
    while (1)
    {
        showMenu();
        std::cin>>flag;
        switch (flag)
        {
        case 1:
            MessageBoxA(NULL, "Hello World!", "MessageBoxA", MB_OK);
            break;
        case 2:
            MessageBoxW(NULL, L"Hello World!", L"MessageBoxW", MB_OK);
            break;
        case 3:
            heapCreatAndDestory();
            break;
        case 4:
            writeFile();
            break;
        case 5:
            readFile();
            break;
        case 6:
            regCreateAndSet();
            break;
        case 7:
            regOpenAndGet();
            break;
        case 8:
            regOpenAndDelete();
            break;
        default:
            break;
        }
        if (flag == 0)
        {
            break;
        }
    }
}