<template>
    <el-alert
        :closable="false"
        :title="msg['result']['title'] + ' ' + msg['funcName']"
        :type="msg['result']['type']"
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
                    :title="msg['result']['title']"
                    :type="msg['result']['type']"
                />
            </template>
            <el-descriptions-item v-for="(val, key, i) in props.msg['info']" align="center" min-width="60px">
                <template #label>
                    {{ key }}
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
import { onMounted, ref } from "vue";
import { defineProps } from "vue";
import { getEncode, getDecode } from "../utils/base64.js";

let description = ref("");
let dialogVisible = ref(false);

const props = defineProps({
    msg: {
        type: Object,
        required: true,
    },
});

description.value = props.msg["time"] + " " + props.msg["result"]["err"];
</script>