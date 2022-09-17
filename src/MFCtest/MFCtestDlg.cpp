
// MFCtestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCtest.h"
#include "MFCtestDlg.h"
#include "windup.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_ABOUTBOX
    };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

void wchar2char(const wchar_t *wchar, char *ch)
{
    int len = WideCharToMultiByte(CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, wchar, -1, ch, len, NULL, NULL);
}

void char2wchar(const char *ch, wchar_t *wchar)
{
    int len = MultiByteToWideChar(CP_ACP, 0, ch, -1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, ch, -1, wchar, len);
}

void path2fileName(char *path, char *fileName)
{
    std::string str(path);
    int pos = str.find_last_of("\\");
    if (pos != std::string::npos)
    {
        str = str.substr(pos + 1);
    }
    strcpy_s(fileName, str.length() + 1, str.c_str());
}

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMFCtestDlg 对话框

CMFCtestDlg::CMFCtestDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCTEST_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCtestDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON4, messagebox);
}

BEGIN_MESSAGE_MAP(CMFCtestDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON4, &CMFCtestDlg::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON5, &CMFCtestDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON10, &CMFCtestDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON6, &CMFCtestDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON7, &CMFCtestDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON8, &CMFCtestDlg::OnBnClickedButton8)
ON_BN_CLICKED(IDC_BUTTON9, &CMFCtestDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON11, &CMFCtestDlg::OnBnClickedButton11)
ON_BN_CLICKED(IDC_BUTTON12, &CMFCtestDlg::OnBnClickedButton12)
ON_BN_CLICKED(IDC_BUTTON1, &CMFCtestDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CMFCtestDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON3, &CMFCtestDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON13, &CMFCtestDlg::OnBnClickedButton13)
ON_BN_CLICKED(IDC_BUTTON14, &CMFCtestDlg::OnBnClickedButton14)
ON_BN_CLICKED(IDC_BUTTON15, &CMFCtestDlg::OnBnClickedButton15)
ON_BN_CLICKED(IDC_BUTTON16, &CMFCtestDlg::OnBnClickedButton16)
END_MESSAGE_MAP()

// CMFCtestDlg 消息处理程序

BOOL CMFCtestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);  // 设置大图标
    SetIcon(m_hIcon, FALSE); // 设置小图标

    // TODO: 在此添加额外的初始化代码
    return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCtestDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCtestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMFCtestDlg::OnEnChangetips1()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}

void CMFCtestDlg::OnBnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码
    MessageBoxA(NULL, "Hello World!", "MessageBoxA", MB_OK);
}

void CMFCtestDlg::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码
    MessageBoxW(L"Hello World!", L"MessageBoxW", MB_OK);
}

void CMFCtestDlg::OnBnClickedButton6()
{
    // TODO: 在此添加控件通知处理程序代码
    char buffer[] = "Hello World!\n";
    HANDLE hFile = (HANDLE)CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if (!WriteFile(hFile, buffer, strlen(buffer) + 1, NULL, NULL))
    {
        return;
    }
    FlushFileBuffers(hFile);
    CloseHandle(hFile);
}

void CMFCtestDlg::OnBnClickedButton7()
{
    // TODO: 在此添加控件通知处理程序代码
    HANDLE hFile = (HANDLE)CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    DWORD dwRead = GetFileSize(hFile, NULL);
    char* buffer = new char[dwRead + 1];
    if (!ReadFile(hFile, buffer, dwRead, NULL, NULL))
    {
        return;
    }
    CloseHandle(hFile);
}

void CMFCtestDlg::OnBnClickedButton8()
{
    // TODO: 在此添加控件通知处理程序代码
    HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, sizeof(int) * 1000, sizeof(int) * 10000);
    if (hHeap == NULL)
    {
        return;
    }
    int *pArr = (int *)HeapAlloc(hHeap, 0, sizeof(int) * 30);
    if (pArr == NULL)
    {
        return;
    }
    HeapFree(hHeap, 0, pArr);
    HeapDestroy(hHeap);
    return;
}

void CMFCtestDlg::OnBnClickedButton9()
{
    // TODO: 在此添加控件通知处理程序代码
    HANDLE hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, sizeof(int) * 1000, sizeof(int) * 10000);
    if (hHeap == NULL)
    {
        return;
    }
    int *pArr = (int *)HeapAlloc(hHeap, 0, sizeof(int) * 30);
    if (pArr == NULL)
    {
        return;
    }
    HeapFree(hHeap, 0, pArr);
    try
    {
        HeapFree(hHeap, 0, 0);
        HeapDestroy(hHeap);
    }
    catch (...)
    {
    }
    return;
}

void CMFCtestDlg::OnBnClickedButton10()
{
    // TODO: 在此添加控件通知处理程序代码
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    {
        return;
    }
    if (RegSetValueEx(hKey, L"test", 0, REG_SZ, (BYTE *)L"hello world", wcslen(L"hello world") * 2) != ERROR_SUCCESS)
    {
        return;
    }
    RegCloseKey(hKey);
}

void CMFCtestDlg::OnBnClickedButton11()
{
    // TODO: 在此添加控件通知处理程序代码
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        return;
    }
    char buffer[100];
    DWORD dwSize = 0;
    if (RegQueryValueEx(hKey, L"test", 0, NULL, NULL, &dwSize) != ERROR_SUCCESS)
    {
        return;
    }
    if (RegQueryValueEx(hKey, L"test", 0, NULL, (BYTE *)buffer, &dwSize) != ERROR_SUCCESS)
    {
        return;
    }
    RegCloseKey(hKey);
}

void CMFCtestDlg::OnBnClickedButton12()
{
    // TODO: 在此添加控件通知处理程序代码
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        return;
    }
    if (RegDeleteValue(hKey, L"test") != ERROR_SUCCESS)
    {
        return;
    }
    if (RegDeleteKey(hKey, L"test") != ERROR_SUCCESS)
    {
        return;
    }
    RegCloseKey(hKey);
}

void CMFCtestDlg::OnBnClickedButton1()
{
    // 初始化
    WORD winsock_version = MAKEWORD(1, 1);
    WSADATA wsa_data;
    if (WSAStartup(winsock_version, &wsa_data) != 0)
    {
        return;
    }
    char url[] = "cse.hust.edu.cn";
    std::string http_msg = "";
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(url, "80", &hints, &result) != 0)
    {
        return;
    }
    // 创建套接字
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;
    addr.sin_port = htons(80);
    if (connect(sock, (SOCKADDR *)&addr, sizeof(addr)) != 0)
    {
        return;
    }
    char buffer[10000];
    sprintf_s(buffer, "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", url);
    send(sock, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    while (recv(sock, buffer, sizeof(buffer), 0) > 0)
    {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
        {
            http_msg += buffer[i];
            i++;
        }
    }
    freeaddrinfo(result);
    closesocket(sock);
    WSACleanup();
}

void CMFCtestDlg::OnBnClickedButton2()
{
    udp::UdpClient client("127.0.0.1", 7777);
    client.send("Hello World!\n");
}

/// @brief 操作多个文件夹
void CMFCtestDlg::OnBnClickedButton3()
{

    char folderPath[] = "test1";
    wchar_t wFolderPath[100];
    char2wchar(folderPath, wFolderPath);
    if ((GetFileAttributesA(folderPath) & FILE_ATTRIBUTE_DIRECTORY))
    {
        CreateDirectory(wFolderPath, NULL);
    }
    folderPath[4] = '2';
    char2wchar(folderPath, wFolderPath);
    if ((GetFileAttributesA(folderPath) & FILE_ATTRIBUTE_DIRECTORY))
    {
        CreateDirectory(wFolderPath, NULL);
    }
    folderPath[4] = '3';
    char2wchar(folderPath, wFolderPath);
    if ((GetFileAttributesA(folderPath) & FILE_ATTRIBUTE_DIRECTORY))
    {
        CreateDirectory(wFolderPath, NULL);
    }
    char buffer[] = "hello world";
    HANDLE hfile = CreateFile(L"test1\\test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if (WriteFile(hfile, buffer, strlen(buffer), NULL, NULL) == FALSE)
    {
        return;
    }
    CloseHandle(hfile);
    hfile = CreateFile(L"test2\\test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if (WriteFile(hfile, buffer, strlen(buffer), NULL, NULL) == FALSE)
    {
        return;
    }
    CloseHandle(hfile);
    hfile = CreateFile(L"test3\\test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if (WriteFile(hfile, buffer, strlen(buffer), NULL, NULL) == FALSE)
    {
        return;
    }
    CloseHandle(hfile);
}

/// @brief 自我复制
void CMFCtestDlg::OnBnClickedButton13()
{
    // TODO: 在此添加控件通知处理程序代码
    char name[1000];

    // 获取当前程序路径
    GetModuleFileNameA(NULL, name, sizeof(name));
    path2fileName(name, name);
    // 复制文件
    HANDLE hfile = CreateFileA(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE)
    {
        
        MessageBoxA(NULL, "ReadFile Error", "Error", MB_OK);
        return;
    }
    DWORD dwSize = GetFileSize(hfile, NULL);
    char *buffer = new char[dwSize];
    if (ReadFile(hfile, buffer, dwSize, NULL, NULL) == FALSE)
    {
        return;
    }
    CloseHandle(hfile);

    // 写入文件
    // 创建文件夹
    char folderPath[] = "test";
    if ((GetFileAttributesA(folderPath) & FILE_ATTRIBUTE_DIRECTORY))
    {
        CreateDirectoryA(folderPath, NULL);
    }
    

    char new_name[1000];
    sprintf_s(new_name, "test\\%s", name);

    hfile = CreateFileA(new_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if (WriteFile(hfile, buffer, dwSize, NULL, NULL) == FALSE)
    {
        return;
    }
    CloseHandle(hfile);
}

/// @brief 读取文件并通过UDP发送
void CMFCtestDlg::OnBnClickedButton14()
{
    // TODO: 在此添加控件通知处理程序代码
    char buffer[10000];
    HANDLE hfile = CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    DWORD dwSize = GetFileSize(hfile, NULL);
    if (ReadFile(hfile, buffer, dwSize, NULL, NULL) == FALSE)
    {
        return;
    }
    CloseHandle(hfile);
    udp::UdpClient client("127.0.0.1", 7777);
    client.send(buffer);
}

/// @brief 注册表自启动
void CMFCtestDlg::OnBnClickedButton15()
{
    // TODO: 在此添加控件通知处理程序代码
    HKEY hKey;
    char name[1000];
    wchar_t wname[1000];
    char path[1000];
    wchar_t wpath[1000];
    GetModuleFileName(NULL, wpath, sizeof(wpath));
    wchar2char(wpath, path);
    path2fileName(path, name);
    char2wchar(name, wname);
    char2wchar(path, wpath);
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        return;
    }
    if (RegSetValueEx(hKey, wname, 0, REG_SZ, (BYTE *)wpath, wcslen(wpath) * 2) != ERROR_SUCCESS)
    {
        return;
    }
}

/// @brief 删除注册表自启动
void CMFCtestDlg::OnBnClickedButton16()
{
    // TODO: 在此添加控件通知处理程序代码
    HKEY hKey;
    char name[1000];
    wchar_t wname[1000];
    char path[1000];
    wchar_t wpath[1000];
    GetModuleFileName(NULL, wpath, sizeof(wpath));
    wchar2char(wpath, path);
    path2fileName(path, name);
    char2wchar(name, wname);
    char2wchar(path, wpath);
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        return;
    }
    if (RegDeleteValue(hKey, wname) != ERROR_SUCCESS)
    {
        return;
    }
}
