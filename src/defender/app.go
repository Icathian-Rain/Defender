package main

import (
	"context"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	"time"

	"defender/processInfo"

	"github.com/wailsapp/wails/v2/pkg/runtime"
)

// App struct
type App struct {
	ctx          context.Context
	testInstance Test // Test instance
}

type ProcessInfo struct {
	ProcessName     string   `json:"processName"`
	ProcessID       uint32   `json:"processID"`
	ProcessDll      []string `json:"processDll"`
	ProcessPriority string   `json:"processPriority"`
}

// Test struct
type Test struct {
	startTime   string
	filePath    string
	isRunning   bool
	syringe_pid *os.Process
	test_pid    *os.Process
	msgs        []string
}

// NewApp creates a new App application struct
func NewApp() *App {
	return &App{}
}

// startup is called when the app starts. The context is saved
// so we can call the runtime methods
// 初始化
func (a *App) startup(ctx context.Context) {
	a.ctx = ctx
	a.testInstance.filePath = ""
	a.testInstance.startTime = ""
	a.testInstance.isRunning = false
	a.testInstance.syringe_pid = nil
	a.testInstance.test_pid = nil
	a.testInstance.msgs = make([]string, 0)
	a.StartUDP()
}

// 获取文件路径
func (a *App) GetFilePath() string {
	return a.testInstance.filePath
}

// 获取进程是否在运行
func (a *App) GetIsRunning() bool {
	return a.testInstance.isRunning
}

// Kill Test
func (a *App) KillTest() bool {
	a.testInstance.filePath = ""
	a.testInstance.startTime = ""
	a.testInstance.isRunning = false
	a.testInstance.msgs = make([]string, 0)
	if a.testInstance.syringe_pid != nil {
		a.testInstance.syringe_pid.Kill()
		a.testInstance.syringe_pid = nil
	}
	if a.testInstance.test_pid != nil {
		a.testInstance.test_pid.Kill()
		a.testInstance.test_pid = nil
	}
	return true
}

// 打开文件
func (a *App) OpenEXEDialog() bool {
	filePath, err := runtime.OpenFileDialog(a.ctx, runtime.OpenDialogOptions{
		Title: "Open EXE",
		Filters: []runtime.FileFilter{
			{
				DisplayName: "*.exe",
				Pattern:     "*.exe",
			},
		},
	})
	if err != nil {
		a.testInstance.filePath = ""
		return false
	}
	a.testInstance.filePath = filePath
	return true

}

// RunTest
func (a *App) RunTest() bool {
	if a.testInstance.filePath == "" {
		a.testInstance.isRunning = false
		a.testInstance.syringe_pid = nil
		return false
	}
	env := os.Environ()
	procAttr := &os.ProcAttr{
		Env: env,
		Files: []*os.File{
			os.Stdin,
			os.Stdout,
			os.Stderr,
		},
	}
	pid, err := os.StartProcess("C:\\Users\\22057\\Desktop\\softwareSecurity\\src\\syringe\\Debug\\syringe.exe", []string{a.testInstance.filePath}, procAttr)
	if err != nil {
		a.testInstance.filePath = ""
		a.testInstance.isRunning = false
		a.testInstance.syringe_pid = nil
		return false
	}
	a.testInstance.isRunning = true
	a.testInstance.syringe_pid = pid
	return true
}

// 开启UDP通信
func (a *App) StartUDP() {
	go func() {
		udp_addr, err := net.ResolveUDPAddr("udp", "127.0.0.1:4000")
		if err != nil {
			fmt.Println(err)
			return
		}
		conn, err := net.ListenUDP("udp", udp_addr)
		if err != nil {
			fmt.Println(err)
			return
		}
		defer conn.Close()
		buf := make([]byte, 0xffff)
		for {
			n, _, err := conn.ReadFromUDP(buf)
			if err != nil {
				fmt.Println(err)
				return
			}
			timeStr := time.Now().Format("2006-01-02 15:04:05")
			msg := string(buf[:n]) + "time\n" + timeStr
			if strings.Contains(msg, "processID") {
				pid := strings.Split(msg, "\n")[1]
				pid_int, _ := strconv.Atoi(pid)
				a.testInstance.test_pid, _ = os.FindProcess(pid_int)
				startTime := strings.Split(msg, "\n")[3]
				a.testInstance.startTime = startTime
			} else {
				a.testInstance.msgs = append(a.testInstance.msgs, msg)
				runtime.EventsEmit(a.ctx, "UDPMessage")
			}
		}
	}()
}

// 获取消息
func (a *App) GetMsgs() []string {
	return a.testInstance.msgs
}

// 清空消息
func (a *App) ClearMsgs() {
	a.testInstance.msgs = make([]string, 0)
}

// 获取注射器进程信息
// 进程名称、进程ID、权限、加载DLL
func (a *App) GetSyringeProcessInfo() ProcessInfo {
	var processinfo ProcessInfo
	processinfo.ProcessName = ""
	processinfo.ProcessID = 0
	processinfo.ProcessPriority = ""
	processinfo.ProcessDll = make([]string, 0)
	if a.testInstance.syringe_pid == nil {
		return processinfo
	}
	var err error
	processinfo.ProcessName, processinfo.ProcessID, processinfo.ProcessPriority, processinfo.ProcessDll, err = processInfo.GetProcessInfo(uint32(a.testInstance.syringe_pid.Pid), processInfo.LIST_MODULES_32BIT)
	if err != nil {
		return processinfo
	}
	return processinfo
}

// 获取测试进程信息
// 进程名称、进程ID、权限、加载DLL
func (a *App) GetTestProcessInfo() ProcessInfo {
	var processinfo ProcessInfo
	processinfo.ProcessName = ""
	processinfo.ProcessID = 0
	processinfo.ProcessPriority = ""
	processinfo.ProcessDll = make([]string, 0)
	if a.testInstance.syringe_pid == nil {
		return processinfo
	}
	var err error
	processinfo.ProcessName, processinfo.ProcessID, processinfo.ProcessPriority, processinfo.ProcessDll, err = processInfo.GetProcessInfo(uint32(a.testInstance.test_pid.Pid), processInfo.LIST_MODULES_32BIT)
	if err != nil {
		return processinfo
	}
	return processinfo
}
