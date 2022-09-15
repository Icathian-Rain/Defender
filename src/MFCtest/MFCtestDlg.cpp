
// MFCtestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCtest.h"
#include "MFCtestDlg.h"
#include "afxdialogex.h"

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
    char buffer[100];
    HANDLE hFile = (HANDLE)CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if (!ReadFile(hFile, buffer, 100, NULL, NULL))
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
    HeapFree(hHeap, 0, pArr);
    HeapDestroy(hHeap);
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
    if (RegSetValueEx(hKey, L"test", 0, REG_SZ, (BYTE *)"hello world", strlen("hello world") + 1) != ERROR_SUCCESS)
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
    WORD winsock_version = MAKEWORD(2, 2);
    WSADATA wsa_data;
    if (WSAStartup(winsock_version, &wsa_data) != 0)
    {
        return ;
    }
    // TODO: 在此添加控件通知处理程序代码
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(5678);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return;
    }
    if (listen(sock, 5) == SOCKET_ERROR)
    {
        return;
    }
    while (1)
    {
        SOCKET client = accept(sock, NULL, NULL);
        if (client == INVALID_SOCKET)
        {
            return;
        }
        char buffer[1024];
        int len = recv(client, buffer, sizeof(buffer), 0);
        if (len == SOCKET_ERROR)
        {
            return;
        }
    }
    closesocket(sock);
}

void CMFCtestDlg::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1")
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(5678);
    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return;
    }
    char buffer[] = "Hello World";
    if (send(sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
        return;
    }
}
