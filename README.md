# Defender
华中科技大学网安学院2020级软件安全课设

## 目录
    -softwaresecurity\                  根目录
    |-question\                         任务书
    |    |-软件安全 课程设计 实验指导书2022（整合）v1.9_plain.docx
    |    |-软件安全 课程设计 实验指导书2022（整合）v1.9_plain.pdf
    |-README.md
    |-src\                              源代码
    |    |-defender\                    图形化界面
    |    |    |-app.go                  功能代码
    |    |    |-build\                  
    |    |    |-frontend\               前端
    |    |    |-go.mod
    |    |    |-go.sum
    |    |    |-main.go
    |    |    |-processInfo\            进程相关类
    |    |    |-README.md
    |    |    |-wails.json
    |    |-DllMain\                     DllMain
    |    |    |-.vs\
    |    |    |-base64.cpp              base64 C++实现
    |    |    |-base64.h                
    |    |    |-dllmain.cpp             dllmain
    |    |    |-msg.h                   通信信息封装类
    |    |    |-winudp.h                udp server client封装类
    |    |-syringe\                     注射器
    |    |    |-syringe.cpp             注射器实现
    |    |-testEXE\
    |    |    |-testEXE.cpp             测试样例源代码
    |-test\
    |    |-ez.exe
    |    |-MFCApplication2.exe          32位测试样例
    |    |-MFCApplication2_64.exe       64位测试样例

## 技术栈
DllMain: C++ C

defender: go Vue 基于wails的桌面应用 element-plus组件

syringe: C++ C

## 原理
DllMain 使用detours开源库 hook 对应的winAPI，将参数信息截取、加密后转为字符串通过udp发送至4000端口。

syringe 将dll注入目标程序

defender 图形化界面共有三个线程，一个线程负责前端数据展示，一个线程负责调用后端函数，另一个线程打开4000端口接收数据并存入数组中。


## MFCtest
测试样例说明

* Messagexbox
  * MessageboxA 弹窗Hello World!
  * MessageboxW 弹窗Hello World!

* 文件操作
  * 打开文件并写入 打开test.txt，写入Hello World!
  * 打开文件并读取 打开test.txt, 读取Hello World!
  * 操作多个文件夹 分别打开test1/test.txt, test2/test.txt, test3/test.txt, 写入Hello World!
  * 自我复制 打开自己，读取内容，写入test/MFCtest.exe
  * 读取文件并发送 打开test.txt，udp发送至127.0.0.1:7777

* 堆操作
  * 正常堆操作 heapcreate heapalloc heapfree heapdestroy
  * 异常堆操作 heapcreate heapalloc heapfree heapfree heapdestroy

* 注册表操作
  * 注册表打开并设置 打开HKEY_CURRENT_USER\\SOFTWARE\\test,设置键test值为hello world，关闭键
  * 注册表打开并读取 打开HKEY_CURRENT_USER\\SOFTWARE\\test,读取键test值，关闭键
  * 注册表打开并删除 打开HKEY_CURRENT_USER\\SOFTWARE\\test，删除键test值 删除键test值
  * 注册表自启动 获取程序路径与名称，打开HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run,设置键程序名称值为程序路径
  * 删除自启动 获取程序路径与名称，打开HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run,删除键程序名称， 删除值程序路径

* 网络操作
  * http请求 创建套接字，connect cse.hust.edu.cn，将请求内容send到cse.hust.edu.cn的80端口，循环多次接受响应内容
  * udp请求 创建套接字，将Hello World sendto 127.0.0.1:7777


 


