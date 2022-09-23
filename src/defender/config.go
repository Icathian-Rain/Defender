package main
import (
	"os"
	"encoding/json"
	"github.com/wailsapp/wails/v2/pkg/runtime"
)

// 获取配置
func (a *App) GetConfig() Config{
	return a.Config
}

// 导出默认配置
func (a *App) ExportDefaultConfig(){
	configFile, err := os.Create("config.json")
	if err != nil {
		return
	}
	defaultConfig := Config{".\\syringe.exe", ".\\dllMain.dll"}
	content,_ := json.MarshalIndent(defaultConfig, "", "	")
	configFile.Write(content)
	defer configFile.Close()
}

// 导出当前配置
func (a *App) ExportCurrentConfig(){
	configFile, err := os.Create("config.json")
	if err != nil {
		return
	}
	content,_ := json.MarshalIndent(a.Config, "", "	")
	configFile.Write(content)
	defer configFile.Close()
}

// 导入配置
func (a *App) ImportConfig(){
	configFile, err := os.Open("config.json")
	if err != nil {
		runtime.EventsEmit(a.ctx, "updateConfig", a.Config)
		return
	}
	var config Config
	json.NewDecoder(configFile).Decode(&config)
	a.Config = config
	runtime.EventsEmit(a.ctx, "updateConfig", a.Config)
	defer configFile.Close()
}
