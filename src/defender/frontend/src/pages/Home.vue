<template>
    <div class="flex-col">
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

const testInstance = ref({
    filePath: "",
    isRunning: false,
});

const percentage = ref(0);
const msg = ref({
    0: "未启动...",
    50: "正在检测...",
    100: "检测完成",
});

let refresh = () => {
    GetFilePath().then((filePath) => {
        testInstance.value.filePath = filePath;
    });
    GetIsRunning().then((isRunning) => {
        testInstance.value.isRunning = isRunning;
        percentage.value = isRunning ? 100 : 0;
    });
};

let kill = () => {
    KillTest().then((flag) => {
        refresh();
    });
};

let openFile = () => {
    OpenEXEDialog()
        .then((flag) => {
            refresh();
        })
        .catch((err) => {
            console.log("err");
        });
};

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

refresh();
</script>