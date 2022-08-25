<script setup>
import Headers from "./components/Headers.vue";
import Asiders from "./components/Asiders.vue";
</script>

<template>
<!-- 主界面 -->
    <div class="common-layout bg-light-50 h-full">
        <el-container class="h-full">
            <!-- header -->
            <el-header class="h-auto" style="padding: 0px">
                <Headers
            /></el-header>
            <el-container class="h-full">
                <!-- aside -->
                <el-aside width="200px" class="h-full">
                    <Asiders />
                </el-aside>
                <!-- main -->
                <el-main>
                    <router-view />
                </el-main>
            </el-container>
        </el-container>
    </div>
</template>

<style>
</style>
