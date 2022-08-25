<script setup>
import Headers from "./components/Headers.vue";
import Asiders from "./components/Asiders.vue";
</script>

<template>
    <div class="common-layout bg-light-50 h-full">
        <el-container class="h-full">
            <el-header class="h-auto" style="padding: 0px">
                <Headers
            /></el-header>
            <el-container class="h-full">
                <el-aside width="200px" class="h-full">
                    <Asiders />
                </el-aside>
                <el-main>
                    <router-view />
                </el-main>
            </el-container>
        </el-container>
    </div>
</template>

<style>
</style>
