package main

import (
	"encoding/json"
	"fmt"
	"net"
	"os"
	"strings"

	"github.com/wailsapp/wails/v2/pkg/runtime"
)

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
