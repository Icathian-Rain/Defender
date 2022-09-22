package main

import (
	"encoding/base64"
	"fmt"
	"path/filepath"
)

var regStartupList = [...]string{
	"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run",
	"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunOnce",
	"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunOnceEx",
	"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunServices",
	"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunServicesOnce",
	"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run",
	"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunOnce",
	"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunOnceEx",
	"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunServices",
	"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\RunServicesOnce",
}

type Msg struct {
	FuncName string            `json:"funcName"`
	Time     string            `json:"time"`
	Info     map[string]string `json:"info"`
	Result   Result            `json:"result"`
}

type Result struct {
	Type  string `json:"type"`
	Title string `json:"title"`
	Err   string `json:"err"`
}

func (a *App) analyzeMsg(msg *Msg) {
	msg.Result.Type = "success"
	msg.Result.Title = "低风险操作"
	msg.Result.Err = "正常"
	a.testInstance.statistics.TotalNum++
	a.testInstance.statistics.SuccessNum++
	// 判断
	switch msg.FuncName {
	// 弹窗操作
	case "MessageBoxA", "MessageBoxW":
		a.testInstance.statistics.MessageboxNum++
	// 文件操作
	case "CreateFile":
		lpFileName, _ := base64.StdEncoding.DecodeString(msg.Info["lpFileName"])
		filePath := string(lpFileName)
		// fileDir := filepath.Dir(filePath)
		fileName := filepath.Base(filePath)
		fileType := filepath.Ext(filePath)
		testInstanceName := filepath.Base(a.testInstance.filePath)
		if fileType == ".exe" || fileType == ".dll" || fileType == ".ocx" {
			msg.Result.Type = "warning"
			msg.Result.Title = "中风险操作"
			msg.Result.Err = "操作可执行文件"
			a.testInstance.statistics.WarningNum++
			a.testInstance.statistics.SuccessNum--
		}
		if fileName == testInstanceName {
			msg.Result.Type = "error"
			msg.Result.Title = "高风险操作"
			msg.Result.Err = "自我复制"
			a.testInstance.statistics.ErrorNum++
			a.testInstance.statistics.SuccessNum--
		}
		a.testInstance.statistics.FileNum++
	case "ReadFile", "WriteFile":
		a.testInstance.statistics.FileNum++
	// 堆操作
	case "HeapAlloc":
		ret, err := base64.StdEncoding.DecodeString(msg.Info["ret"])
		if err != nil {
			fmt.Println(err)
			return
		}
		a.testInstance.heapList = append(a.testInstance.heapList, string(ret))
		a.testInstance.statistics.HeapNum++
	case "HeapFree":
		lpMem, _ := base64.StdEncoding.DecodeString(msg.Info["lpMem"])
		var index int = -1
		for i, v := range a.testInstance.heapList {
			if v == string(lpMem) {
				index = i
				break
			}
		}
		if index == -1 {
			msg.Result.Type = "error"
			msg.Result.Title = "高风险操作"
			msg.Result.Err = "堆重复释放"
			a.testInstance.statistics.ErrorNum++
			a.testInstance.statistics.SuccessNum--
		} else {
			a.testInstance.heapList = append(a.testInstance.heapList[:index], a.testInstance.heapList[index+1:]...)
		}
		a.testInstance.statistics.HeapNum++
	case "HeapCreate", "HeapDestroy":
		a.testInstance.statistics.HeapNum++
	case "RegOpenKeyEx":
		lpSubKey, _ := base64.StdEncoding.DecodeString(msg.Info["lpSubKey"])
		index := -1
		for i, v := range regStartupList {
			if v == string(lpSubKey) {
				index = i
				break
			}
		}
		if index != -1 {
			msg.Result.Type = "error"
			msg.Result.Title = "高风险操作"
			msg.Result.Err = "注册表开机自启动"
			a.testInstance.statistics.ErrorNum++
			a.testInstance.statistics.SuccessNum--
		} else {
			msg.Result.Type = "warning"
			msg.Result.Title = "中风险操作"
			msg.Result.Err = "注册表操作"
			a.testInstance.statistics.WarningNum++
			a.testInstance.statistics.SuccessNum--
		}
		a.testInstance.statistics.RegNum++
	case "RegCreateKeyEx", "RegSetValueEx","RegQueryValueEx","RegDeleteKeyEx", "RegDeleteValue", "RegCloseKey":
		msg.Result.Type = "warning"
		msg.Result.Title = "中风险操作"
		msg.Result.Err = "注册表操作"
		a.testInstance.statistics.WarningNum++
		a.testInstance.statistics.SuccessNum--
		a.testInstance.statistics.RegNum++
	case "socket", "bind", "listen", "accept", "connect", "send", "recv", "sendto", "recvfrom":
		a.testInstance.statistics.NetNum++
	}
}
