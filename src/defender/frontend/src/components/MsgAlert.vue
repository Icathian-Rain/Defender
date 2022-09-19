<template>
    <el-alert
        :closable="false"
        :title="result.title + ' ' + msg['funcName']"
        :type="result.type"
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
                    :title="result.title"
                    :type="result.type"
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
import { ref } from "vue";
import { defineProps } from "vue";
import { getEncode, getDecode } from "../utils/base64.js";
import { GetFilePath } from "../../wailsjs/go/main/App"
import analysis from "../utils/analysis";

let result = ref({});
let description = ref("");
let dialogVisible = ref(false);
let testInstanceName = "";
let heapList = [];
let dirList = [];

const props = defineProps({
    msg: {
        type: Object,
        required: true,
    },
});

GetFilePath().then((res) => {
    testInstanceName = res;
});

result.value = analysis(props.msg, testInstanceName, heapList, dirList);
description.value = props.msg["time"] + " " + result.value["err"];
</script>