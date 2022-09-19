

import { getDecode, getEncode} from "./base64.js";
function getFileDir(filePath) {
    let pos = filePath.lastIndexOf("\\\\");
    if(pos == -1) {
        pos = filePath.lastIndexOf("/");
        if(pos == -1) {
            return "";
        }
    }
    return filePath.substring(0, pos)
}

function getFileType(filePath) {
    let pos = filePath.lastIndexOf(".");
    if(pos == -1) {
        return "";
    }
    return filePath.substring(pos + 1)
}

function getFileName(filePath) {
    let pos = filePath.lastIndexOf("\\\\");
    if(pos == -1) {
        pos = filePath.lastIndexOf("/");
        if(pos == -1) {
            return filePath;
        }
    }
    return filePath.substring(pos + 1)
}


function analysis(msg, testInstanceName, heapList, dirList) {
    let regStartupList = [
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
    ];
    let result = {
        type: "success",
        title: "低风险操作",
        err: "正常",
    };
    // 判断消息的类型
    switch (msg.funcName) {
        // 堆操作异常分析
        case "HeapAlloc":
            let ret = msg.info.ret;
            heapList.push(ret)
        break;
        case "HeapFree":
            let lpMem = msg.info.lpMem;
            let index0 = heapList.indexOf(lpMem)
            if (index0 > -1) {
                result.type = "error";
                result.title = "高风险操作";
                result.err = "堆重复释放";
            }
            else {
                heapList.splice(index0, 1)
            }
        break;
        // 文件操作异常分析
        case "CreateFile":
            let filePath = getDecode(msg.info.lpFileName)
            let fileDir = getFileDir(filePath)
            let fileType = getFileType(filePath)
            let fileName = getFileName(filePath)
            // let index1 = dirList.indexOf(fileDir)
            // if (index1 == -1) {
            //     dirList.push(fileDir)
            // }
            // else {
            //     msg.type = 1
            //     msg.err = "操作多个文件夹"
            // }
            if (fileType == "exe" || fileType == "dll" || fileType == "ocx") {
                result.type = "error";
                result.title = "高风险操作";
                result.err = "操作可执行文件";
            }
            if (fileName == testInstanceName) {
                result.type = "error";
                result.title = "高风险操作";
                result.err = "存在自我复制";
            }
        break;
        // 注册表异常分析
        case "RegOpenKeyEx":
            let lpSubKey = getDecode(msg.info.lpSubKey)
            let index2 = regStartupList.indexOf(lpSubKey)
            if (index2 != -1) {
                result.type = "error";
                result.title = "高风险操作";
                result.err = "注册表开机启动";
            }
        break;
        case "RegSetValueEx":
        case "RegCreateKeyEx":
        case "RegDeleteKey":
        case "RegDeleteValue":
            result.type = "warning";
            result.title = "中风险操作";
            result.err = "注册表操作";
        break;
        // 网络异常分析
    }
    return result;
}

export default analysis;