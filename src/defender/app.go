package main

import (
	"context"
	"fmt"
	"net"
	"os"

	"github.com/wailsapp/wails/v2/pkg/runtime"
)

// App struct
type Test struct {
	filePath  string
	isRunning bool
	pid       *os.Process
}

type App struct {
	ctx          context.Context
	testInstance Test
}

// NewApp creates a new App application struct
func NewApp() *App {
	return &App{}
}

// startup is called when the app starts. The context is saved
// so we can call the runtime methods
func (a *App) startup(ctx context.Context) {
	a.ctx = ctx
	a.testInstance.filePath = ""
	a.testInstance.isRunning = false
	a.testInstance.pid = nil
	a.StartUDP()
}

func (a *App) GetFilePath() string {
	return a.testInstance.filePath
}

func (a *App) GetIsRunning() bool {
	return a.testInstance.isRunning
}

func (a *App) KillTest() bool {
	if a.testInstance.pid == nil {
		a.testInstance.filePath = ""
		a.testInstance.isRunning = false
		return false
	}
	err := a.testInstance.pid.Kill()
	if err != nil {
		a.testInstance.filePath = ""
		a.testInstance.isRunning = false
		return false
	}
	a.testInstance.filePath = ""
	a.testInstance.isRunning = false
	a.testInstance.pid = nil
	return true
}

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
func (a *App) RunTest() bool {
	if a.testInstance.filePath == "" {
		a.testInstance.isRunning = false
		a.testInstance.pid = nil
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
		a.testInstance.pid = nil
		return false
	}
	a.testInstance.isRunning = true
	a.testInstance.pid = pid
	fmt.Printf("Process %d started...\n", pid)
	return true
}

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
		buf := make([]byte, 1024)
		for {
			n, _, err := conn.ReadFromUDP(buf)
			if err != nil {
				fmt.Println(err)
				return
			}
			runtime.EventsEmit(a.ctx, "UDPMessage", string(buf[0:n]))
		}
	}()
}
