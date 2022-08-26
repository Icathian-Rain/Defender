<template>
    <div class="flex-col">
        <!-- 输入可执行文件 -->
        <div>
            <el-input
                type="text"
                placeholder="请输入可执行文件路径"
                v-model="testInstance.filePath"
                class="w-3/4"
            ></el-input>
            <el-button
                type="primary"
                v-on:click="openFile()"
                class="inline absolute right-3"
                >选择可执行文件</el-button
            >
        </div>
        <!-- RunTest 或者 Kill Test -->
        <div class="h-20 mt-10 text-center">
            <el-button
                type="success"
                @click="test()"
                circle
                class="h-20 w-20"
                v-if="testInstance.isRunning === false"
            >
                启动检测
            </el-button>

            <el-button
                type="danger"
                @click="kill()"
                circle
                class="h-20 w-20"
                v-if="testInstance.isRunning === true"
            >
                终止程序
            </el-button>
        </div>

        <!-- 进度条 -->
        <div class="h-20 mt-10">
            <el-progress
                class=""
                :percentage="percentage"
                status="success"
                :stroke-width="30"
            >
                <span>{{ msg[percentage] }}</span>
            </el-progress>
        </div>
    </div>
</template>

<script setup>
import { ElMessage } from "element-plus";
import { ref } from "vue";
import {
    OpenEXEDialog,
    RunTest,
    GetFilePath,
    GetIsRunning,
    KillTest,
} from "../../wailsjs/go/main/App";

// 初始化测试实例
const testInstance = ref({
    filePath: "",
    isRunning: false,
});

// 初始化进度条
const percentage = ref(0);
const msg = ref({
    0: "未启动...",
    50: "正在检测...",
    100: "检测完成",
});

// 刷新实例状态
let refresh = () => {
    GetFilePath().then((filePath) => {
        testInstance.value.filePath = filePath;
    });
    GetIsRunning().then((isRunning) => {
        testInstance.value.isRunning = isRunning;
        percentage.value = isRunning ? 100 : 0;
    });
};

// 打开文件
let openFile = () => {
    OpenEXEDialog()
        .then((flag) => {
            refresh();
        })
        .catch((err) => {
            console.log("err");
        });
};

// 启动测试
let test = () => {
    percentage.value = 50;
    RunTest()
        .then((flag) => {
            refresh();
        })
        .catch((err) => {
            console.log(err);
        });
};

// 停止测试
let kill = () => {
    KillTest().then((flag) => {
        refresh();
    });
};

// 刷新实例状态
refresh();
</script>