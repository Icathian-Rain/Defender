import { getDecode, getEncode } from "./base64.js";

const regStartupList = [
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

function getFileDir(filePath) {
    let pos = filePath.lastIndexOf("\\\\");
    if (pos == -1) {
        pos = filePath.lastIndexOf("/");
        if (pos == -1) {
            pos = filePath.lastIndexOf("\\");
            if (pos == -1) {
                return "";
            }
        }
    }
    return filePath.substring(0, pos)
}

function getFileType(filePath) {
    let pos = filePath.lastIndexOf(".");
    if (pos == -1) {
        return "";
    }
    return filePath.substring(pos + 1)
}

function getFileName(filePath) {
    let pos = filePath.lastIndexOf("\\\\");
    if (pos == -1) {
        pos = filePath.lastIndexOf("/");
        if (pos == -1) {
            pos = filePath.lastIndexOf("\\");
            if (pos == -1) {
                return filePath;
            }
        }

    }
    return filePath.substring(pos + 1)
}


function analysis(msg, testInstancePath, heapList, dirList) {

    // 判断消息的类型
    msg.type = "success";
    msg.title = "低风险操作";
    msg.err = "正常";

    switch (msg.funcName) {
        // 堆操作异常分析
        case "HeapAlloc":
            let ret = getDecode(msg.info.ret);
            heapList.push(ret)
            break;
        case "HeapFree":
            let lpMem = getDecode(msg.info.lpMem);
            let index0 = heapList.indexOf(lpMem)
            if (index0 == -1) {
                msg.type = "error";
                msg.title = "高风险操作";
                msg.err = "堆重复释放";
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
            let testInstanceName = getFileName(testInstancePath)
            // let index1 = dirList.indexOf(fileDir)
            // if (index1 == -1) {
            //     dirList.push(fileDir)
            // }
            // else {

            // }
            if (fileType === "exe" || fileType === "dll" || fileType === "ocx") {
                msg.type = "warning";
                msg.title = "中风险操作";
                msg.err = "操作可执行文件";
            }
            if (fileName === testInstanceName) {
                msg.type = "error";
                msg.title = "高风险操作";
                msg.err = "自我复制";
            }
            break;
        // 注册表异常分析
        case "RegOpenKeyEx":
            let lpSubKey = getDecode(msg.info.lpSubKey)
            let index2 = regStartupList.indexOf(lpSubKey)
            if (index2 != -1) {
                msg.type = "warning";
                msg.title = "中风险操作";
                msg.err = "注册表开机自启动";
            }
            else {
                msg.type = "warning";
                msg.title = "中风险操作";
                msg.err = "注册表操作";
            }
            break;
        case "RegCreateKeyEx":
        case "RegSetValueEx":
        case "RegQueryValueEx":
        case "RegCloseKey":
        case "RegDeleteKey":
        case "RegDeleteValue":
            msg.type = "warning";
            msg.title = "中风险操作";
            msg.err = "注册表操作";
            break;
    }
}

export default analysis;