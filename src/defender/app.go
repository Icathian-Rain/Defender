package main

import (
	"context"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"strings"

	"github.com/wailsapp/wails/v2/pkg/runtime"
)

// App struct
type App struct {
	ctx          context.Context // 上下文
	testInstance Test            // Test instance
}

//

type Statistics struct {
	TotalNum   int `json:"totalNum"`
	SuccessNum int `json:"successNum"`
	WarningNum int `json:"warningNum"`
	ErrorNum   int `json:"errorNum"`
	MessageboxNum int `json:"messageboxNum"`
	FileNum    int `json:"fileNum"`
	HeapNum    int `json:"heapNum"`
	RegNum     int `json:"regNum"`
	NetNum    int `json:"netNum"`
	MemoryNum  int `json:"memoryNum"`
}

// Test struct
type Test struct {
	startTime   string
	filePath    string
	isRunning   bool
	syringe_pid *os.Process
	test_pid    *os.Process
	msgs        []Msg
	heapList    []string
	statistics  Statistics
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
	a.testInstance.msgs = make([]Msg, 0)
	a.testInstance.heapList = make([]string, 0)
	a.testInstance.statistics = Statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	a.StartUDP()
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
			go func() {
				rawMsg := string(buf[:n])
				if strings.Contains(rawMsg, "processID") {
					var process Process
					json.Unmarshal([]byte(rawMsg), &process)
					a.testInstance.test_pid, _ = os.FindProcess(int(process.ProcessID))
					a.testInstance.startTime = process.Time
				} else {
					var msg Msg
					json.Unmarshal([]byte(rawMsg), &msg)
					a.analyzeMsg(&msg)
					a.testInstance.msgs = append(a.testInstance.msgs, msg)
					runtime.EventsEmit(a.ctx, "UDPMessage")
				}
			}()
		}
	}()
}
