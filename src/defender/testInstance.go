package main

import (
	"os"
	"syscall"
)

// 进程信息
type ProcessInfo struct {
	ProcessName     string   `json:"processName"`
	ProcessID       uint32   `json:"processID"`
	ProcessDll      []string `json:"processDll"`
	ProcessPriority string   `json:"processPriority"`
}

// 接受的进程信息
type Process struct {
	ProcessID uint32
	Time      string
}

// 获取文件路径
func (a *App) GetFilePath() string {
	return a.testInstance.filePath
}

// 获取进程是否在运行
func (a *App) GetIsRunning() bool {
	return a.testInstance.isRunning
}

// 获取消息
func (a *App) GetMsgs() []Msg {
	return a.testInstance.msgs
}

// 获取统计信息
func (a *App) GetStatistics() Statistics {
	return a.testInstance.statistics
}

// 清空消息
func (a *App) ClearMsgs() {
	a.testInstance.msgs = make([]Msg, 0)
	a.testInstance.statistics = Statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
}

// RunTest
func (a *App) RunTest(filePath string) bool {
	a.testInstance.filePath = filePath
	var length = len(filePath)
	if length < 5 || filePath[length-4:] != ".exe" {
		a.testInstance.isRunning = false
		a.testInstance.syringe_pid = nil
		return false
	}
	syringe_path := "C:\\Users\\22057\\Desktop\\softwareSecurity\\test\\syringe.exe"
	// dir, _ := os.Getwd()
	env := os.Environ()
	procAttr := &os.ProcAttr{
		Env: env,
		Files: []*os.File{
			os.Stdin,
			os.Stdout,
			os.Stderr,
		},
		Sys: &syscall.SysProcAttr{HideWindow: true},
		// Dir: dir,
		Dir: "C:\\Users\\22057\\Desktop\\softwareSecurity\\test",
	}
	pid, err := os.StartProcess(syringe_path, []string{a.testInstance.filePath}, procAttr)
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

// Kill Test
func (a *App) KillTest() bool {
	a.testInstance.filePath = ""
	a.testInstance.startTime = ""
	a.testInstance.heapList = make([]string, 0)
	a.testInstance.isRunning = false
	a.testInstance.msgs = make([]Msg, 0)
	a.testInstance.statistics = Statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
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

// 获取注射器进程信息
// 进程名称、进程ID、权限、加载DLL
func (a *App) GetSyringeProcessInfo() ProcessInfo {
	var syringeProcessInfo ProcessInfo
	syringeProcessInfo.ProcessName = ""
	syringeProcessInfo.ProcessID = 0
	syringeProcessInfo.ProcessPriority = ""
	syringeProcessInfo.ProcessDll = make([]string, 0)
	if a.testInstance.syringe_pid == nil {
		return syringeProcessInfo
	}
	getProcessInfo(uint32(a.testInstance.syringe_pid.Pid), LIST_MODULES_32BIT, &syringeProcessInfo)
	return syringeProcessInfo
}

// 获取测试进程信息
// 进程名称、进程ID、权限、加载DLL
func (a *App) GetTestProcessInfo() ProcessInfo {
	var testProcessInfo ProcessInfo
	testProcessInfo.ProcessName = ""
	testProcessInfo.ProcessID = 0
	testProcessInfo.ProcessPriority = ""
	testProcessInfo.ProcessDll = make([]string, 0)
	if a.testInstance.syringe_pid == nil {
		return testProcessInfo
	}
	getProcessInfo(uint32(a.testInstance.test_pid.Pid), LIST_MODULES_32BIT, &testProcessInfo)
	return testProcessInfo
}
