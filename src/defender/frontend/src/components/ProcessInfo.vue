<template>
    <el-card shadow="hover" class="inline-block h-auto">
        <div class="flex flex-col">
            <div class="mb-5">
                <span class="text-lg font-bold">进程信息</span>
                <span class="text-sm text-gray-500 ml-2"
                    >Process Information</span
                >
            </div>
            <div>
                <span class="text-sm text-gray-500">进程名</span>
                <span class="text-sm font-bold ml-9">{{
                    props.processInfo.processName
                }}</span>
            </div>
            <div>
                <span class="text-sm text-gray-500">进程ID</span>
                <span class="text-sm font-bold ml-9">{{
                    props.processInfo.processID
                }}</span>
            </div>
            <div>
                <span class="text-sm text-gray-500">进程权限</span>
                <span class="text-sm font-bold ml-6">{{
                    props.processInfo.processPriority
                }}</span>
            </div>
            <span class="text-sm text-gray-500">进程加载DLL</span>
            <el-scrollbar height="140px">
                
                <span
                    class="block"
                    v-for="dll in props.processInfo.processDll"
                    :key="dll"
                    >{{ dll }}</span
                >
            </el-scrollbar>
        </div>
    </el-card>
</template>


<script setup>
import { ref } from "vue";

const props = defineProps({
    processInfo: {
        type: Object,
        default: {
            processName: "",
            processID: 0,
            processDll: [],
            processPriority: "",
        },
    },
});
const activeName = ref("1");
</script>