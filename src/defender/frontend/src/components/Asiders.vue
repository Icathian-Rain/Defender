<template>
    <div style="background-color: #212529" class="h-full">
        <div class="h-45 pt-8 text-center pb-5">
            <img
                src="../assets/icon/security.svg"
                alt=""
                class="h-24 w-24 m-auto mb-2"
            />
            <span class="text-3xl text-stroke-sm text-light-50 cursor-default">Defender</span>
        </div>
        <div class="border-gray-500 border-b w-4/5 m-auto mt-4 mb-10"></div>
        <div class="flex flex-col items-center">
            <div v-for="option in options" class="option">
                <router-link
                    :to="option.path"
                    active-class="active"
                >
                    <div class="text-center text-xl text-light-50 hover:text-blue-300">
                        {{ option.label }}
                    </div>
                </router-link>
            </div>
        </div>
    </div>
</template>

<script setup>
import { ref } from "vue";

let options = [
    {
        label: "主页",
        path: "/",
    },
    {
        label: "行为监控",
        path: "/monitor",
    },
    {
        label: "统计分析",
        path: "/statistics",
    },
    {
        label: "设置",
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


.active {
}
.active::after {
    content: "";
    display: inline-block;
    position: relative;
    top: -22px;
    left: 180px;
    width: 0;
    height: 0;
    border-right: 20px solid #f2f2f2;
    border-top: 10px solid transparent;
    border-bottom: 10px solid transparent;
}
</style>