<template>
    <el-alert
        :closable="false"
        :title="titleList[type] + ' ' + msg['funcName']"
        :type="typeList[type]"
        :description="description"
        @click="dialogVisible = true"
        show-icon
        class="mb-5 cursor-pointer"
    />
    <el-dialog v-model="dialogVisible" title="操作详细信息" width="70%">
        <el-descriptions :title="msg['funcName']" :column="3" border direction="vertical">
            <template #extra>
                <el-alert
                    :closable="false"
                    :title="titleList[type]"
                    :type="typeList[type]"
                />
            </template>
            <el-descriptions-item v-for="(val, key, i) in props.msg['info']" align="center" min-width="60px">
                <template #label>
                    {{ getDecode(key) }}
                </template>
                <el-scrollbar max-height="100px">
                        {{ getDecode(val) }}
                </el-scrollbar>
            </el-descriptions-item>
        </el-descriptions>
        <template #footer>
            <span class="dialog-footer">
                <el-button type="primary" @click="dialogVisible = false"
                    >确认</el-button
                >
            </span>
        </template>
    </el-dialog>
</template>

<script setup>
import { ref } from "vue";
import { defineProps } from "vue";
import { getEncode64, getDecode } from "../utils/base64.js";

const typeList = ["success", "warning", "error"];
const titleList = ["低风险操作", "中风险操作", "高风险操作"];
let type = ref(0);
let description = ref("");
let dialogVisible = ref(false);

const props = defineProps({
    msg: {
        type: Object,
        required: true,
    },
});

description.value = props.msg["time"];
</script>