<template>
    <el-alert
    :closable = "false"
    :title="titleList[type] + ' ' + msg['funcName']"
    :type="typeList[type]"
    :description="description"
    show-icon
    class="mb-5"
  />
</template>

<script setup>
import { ref } from "vue";
import { defineProps } from "vue";

const typeList = ["success", "warning", "error"];
const titleList = ["低风险操作", "中风险操作", "高风险操作"];
let type = ref(0);
let description = ref("");

const props = defineProps({
    msg: {
        type: Object,
        required: true,
    }
});

description.value = props.msg["info"]["time"]








</script>