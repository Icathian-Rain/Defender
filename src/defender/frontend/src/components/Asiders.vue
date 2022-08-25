<template>
    <div class="flex flex-col items-center bg-gray-200 h-full">
        <div v-for="option in options" class="option">
            <router-link
                :to="option.path"
                class="visited:bg-blue-400"
                active-class="active"
            >
                <div class="text-center text-xl">
                    {{ option.label }}
                </div>
            </router-link>
        </div>
    </div>
</template>

<script setup>
import { ref } from "vue";

let options = [
    {
        label: "主页",
        icon: "el-icon-menu",
        path: "/",
    },
    {
        label: "行为监控",
        icon: "el-icon-menu",
        path: "/monitor",
    },
    {
        label: "系统设置",
        icon: "el-icon-menu",
        path: "/settings",
    },
];
</script>

<style>
.option {
    padding-bottom: 10px;
    padding-top: 10px;
    width: 100%;
    height: 50px;
}

.option:hover {
    background-color: #ecf5ff;
}

.active {
    color: #60aeff;
}
</style>