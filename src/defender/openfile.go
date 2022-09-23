package main

import (

	"github.com/wailsapp/wails/v2/pkg/runtime"
)

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

// 打开注射器
func (a *App) OpenSyringeFile(){
	filePath, err := runtime.OpenFileDialog(a.ctx, runtime.OpenDialogOptions{
		Title: "Open File",
		Filters: []runtime.FileFilter{
			{
				DisplayName: "exe",
				Pattern:     "*.exe",
			},
		},
	})
	if err != nil {
		runtime.EventsEmit(a.ctx, "updateConfig", a.Config)
		return 
	}
	if filePath != "" {
	a.Config.SyringePath = filePath
	}
	runtime.EventsEmit(a.ctx, "updateConfig", a.Config)
}

// 打开Dll文件
func (a *App) OpenDllFile(){
	filePath, err := runtime.OpenFileDialog(a.ctx, runtime.OpenDialogOptions{
		Title: "Open File",
		Filters: []runtime.FileFilter{
			{
				DisplayName: "dll",
				Pattern:     "*.dll",
			},
		},
	})
	if err != nil {
		runtime.EventsEmit(a.ctx, "updateConfig", a.Config)
		return 
	}
	if filePath != "" {
	a.Config.DllPath = filePath
	}
	runtime.EventsEmit(a.ctx, "updateConfig", a.Config)
}