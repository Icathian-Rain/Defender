package main

import (
	"context"
	"os"
)

// App struct
type App struct {
	ctx          context.Context // 上下文
	testInstance Test            // Test instance
	Config 	 Config          // 配置
}

type Config struct {
	SyringePath string `json:"syringePath"`
	DllPath string `json:"dllPath"`
}


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
	a.Config = Config{".\\syringe.exe", ".\\dllMain.dll"}
	a.testInstance.filePath = ""
	a.testInstance.startTime = ""
	a.testInstance.isRunning = false
	a.testInstance.syringe_pid = nil
	a.testInstance.test_pid = nil
	a.testInstance.msgs = make([]Msg, 0)
	a.testInstance.heapList = make([]string, 0)
	a.testInstance.statistics = Statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	a.StartUDP()
	a.ImportConfig()
}




