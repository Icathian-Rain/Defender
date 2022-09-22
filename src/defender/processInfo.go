package main

import (
	"path/filepath"
	"syscall"
	"unsafe"
)

type ListMode uintptr

const (
	LIST_MODULES_DEFAULT ListMode = 0x0  //查看默认的模块
	LIST_MODULES_32BIT   ListMode = 0x01 //查看32-bit的模块
	LIST_MODULES_64BIT   ListMode = 0x02 //查看64-bit的模块
	LIST_MODULES_ALL     ListMode = 0x03 //查看所有的模块
)

const (
	ABOVE_NORMAL_PRIORITY_CLASS = 0x00008000
	BELOW_NORMAL_PRIORITY_CLASS = 0x00004000
	HIGH_PRIORITY_CLASS         = 0x00000080
	IDLE_PRIORITY_CLASS         = 0x00000040
	NORMAL_PRIORITY_CLASS       = 0x00000020
	REALTIME_PRIORITY_CLASS     = 0x00000100
)

var psapi = syscall.NewLazyDLL("Psapi.dll")
var processthreadsapi = syscall.NewLazyDLL("kernel32.dll")
var enumProcessModulesEx = psapi.NewProc("EnumProcessModulesEx")
var getModuleFileNameExW = psapi.NewProc("GetModuleFileNameExW")
var GetPriorityClass = processthreadsapi.NewProc("GetPriorityClass")
var GetProcessImageFileNameA = psapi.NewProc("GetProcessImageFileNameA")

//Windows上实现
func getstring(vcode uintptr) string {
	var vbyte []byte
	for i := 0; i < 200; i++ {
		sbyte := *((*byte)(unsafe.Pointer(vcode)))
		if sbyte == 0 {
			break
		}
		vbyte = append(vbyte, sbyte)
		vcode += 1
	}
	return string(vbyte)
}

func getProcessPriority(HANDLE syscall.Handle) string {
	var priority string
	var ret, _, _ = GetPriorityClass.Call(uintptr(HANDLE))
	switch ret {
	case ABOVE_NORMAL_PRIORITY_CLASS:
		priority = "ABOVE_NORMAL_PRIORITY_CLASS"
	case BELOW_NORMAL_PRIORITY_CLASS:
		priority = "BELOW_NORMAL_PRIORITY_CLASS"
	case HIGH_PRIORITY_CLASS:
		priority = "HIGH_PRIORITY_CLASS"
	case IDLE_PRIORITY_CLASS:
		priority = "IDLE_PRIORITY_CLASS"
	case NORMAL_PRIORITY_CLASS:
		priority = "NORMAL_PRIORITY_CLASS"
	case REALTIME_PRIORITY_CLASS:
		priority = "REALTIME_PRIORITY_CLASS"
	}
	return priority
}

func getProcessInfo(pid uint32, mode ListMode, processInfo *ProcessInfo) {
	// 获取进程ID
	processInfo.ProcessID = pid
	//获取进程句柄
	var hProcess, err = syscall.OpenProcess(0x0400|0x0010, false, pid)
	if err != nil {
		return
	}

	//注意关闭句柄
	defer syscall.CloseHandle(hProcess)

	// 获取进程名称
	var buf = make([]uint16, 260)
	var n = uint32(len(buf))
	var ret, _, _ = GetProcessImageFileNameA.Call(uintptr(hProcess), uintptr(unsafe.Pointer(&buf[0])), uintptr(n))
	if ret == 0 {
		return
	}
	processInfo.ProcessName = getstring(uintptr(unsafe.Pointer(&buf[0])))
	processInfo.ProcessName = filepath.Base(processInfo.ProcessName)

	// 获取进程权限
	processInfo.ProcessPriority = getProcessPriority(hProcess)

	//获取进程打开的模块数
	var (
		cbNeeded uint32
		hMods    [1024]uintptr
	)

	r, _, err := enumProcessModulesEx.Call(uintptr(hProcess), uintptr(unsafe.Pointer(&hMods[0])), unsafe.Sizeof(hMods), uintptr(unsafe.Pointer(&cbNeeded)), uintptr(mode))
	if r == 0 {
		return
	}

	//计算模块数量
	var modelength = int(cbNeeded) / int(unsafe.Sizeof(hMods[0]))
	var szModName [260]uint16

	for i := 0; i < modelength; i++ {
		r, _, err = getModuleFileNameExW.Call(uintptr(hProcess), hMods[i], uintptr(unsafe.Pointer(&szModName)), unsafe.Sizeof(szModName))
		if r == 0 {
			return
		}
		processInfo.ProcessDll = append(processInfo.ProcessDll, syscall.UTF16ToString(szModName[:]))
	}
	return
}
