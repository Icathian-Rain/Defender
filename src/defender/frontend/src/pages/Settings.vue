<template>
    <div>
        <span class="text-xl">
            提示:配置文件为当前目录下的config.json文件,导出配置时会覆盖当前配置文件
            <br/>
        </span>
        <span class="text-xl">
            请使用官方注射器，以免参数错误导致无法注射
        </span>
    </div>
    <div class="flex justify-between mt-5">
        <div>
            <el-button type="success" @click="ExportDefaultConfig()">生成默认配置</el-button>
            <el-button type="primary" @click="ExportCurrentConfig()">导出当前配置</el-button>
        </div>
        <el-button type="info" @click="ImportConfig()">导入配置</el-button>
    </div>

    <div class="flex-row mt-5">
        <div class="w-full mb-5">
            <el-input
                type="text"
                placeholder="请输入注射器路径"
                v-model = "syringePath"
                class="w-3/4"
            ></el-input>
            <el-button
                type="primary"
                v-on:click="OpenSyringeFile()"
                class="left-15 relative"
                >选择注射器路径</el-button
            >
        </div>
        <div class="w-full">
            <el-input
                type="text"
                placeholder="请输入注射DLL路径"
                v-model = "dllPath"
                class="w-3/4"
            ></el-input>
            <el-button
                type="primary"
                v-on:click="OpenDllFile()"
                class="left-15 relative"
                >选择dll路径</el-button
            >
        </div>
    </div>
</template>


<script setup>
import {ref} from 'vue'
import { OpenSyringeFile, OpenDllFile, GetConfig, ImportConfig, ExportDefaultConfig, ExportCurrentConfig} from "../../wailsjs/go/main/App"
import { EventsOn } from "../../wailsjs/runtime/runtime";

const syringePath = ref('')
const dllPath = ref('')

EventsOn("updateConfig", (config) => {
    syringePath.value = config.syringePath
    dllPath.value = config.dllPath
})

GetConfig().then((config) => {
    syringePath.value = config.syringePath
    dllPath.value = config.dllPath
})



</script>