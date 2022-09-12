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


