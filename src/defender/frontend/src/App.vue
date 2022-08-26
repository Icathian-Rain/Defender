<script setup>
import Headers from "./components/Headers.vue";
import Asiders from "./components/Asiders.vue";
</script>

<template>
    <!-- 主界面 -->

    <div class="common-layout h-full">
        <el-container class="h-full">
            <el-aside width="200px"><Asiders /></el-aside>
            <el-container>
                <el-header height="100px"><Headers /></el-header>
                <el-main><router-view /></el-main>
            </el-container>
        </el-container>
    </div>
</template>

<style>
</style>
