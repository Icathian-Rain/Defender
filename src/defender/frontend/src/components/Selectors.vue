<template>
    <el-checkbox
        v-model="checkAll"
        :indeterminate="isIndeterminate"
        @change="handleCheckAllChange"
        >全选</el-checkbox
    >
    <el-checkbox-group
        v-model="checkedMsgTypes"
        @change="handleCheckedTypesChange"
    >
        <el-checkbox v-for="type in msgTypes" :key="type" :label="type">{{
            type
        }}</el-checkbox>
    </el-checkbox-group>
</template>


<script setup>
import { ref } from "vue";

const emit = defineEmits(["setSelectedTypes"]);
const msgTypes = [
    "MessageBoxA",
    "MessageBoxW",

    "HeapCreate",
    "HeapDestroy",
    "HeapAlloc",
    "HeapFree",

    "CreateFile",
    "WriteFile",
    "ReadFile",

    "RegCreateKeyEx",
    "RegSetValueEx",
    "RegOpenKeyEx",
    "RegCloseKey",
    "RegDeleteKey",
    "RegDeleteValue",

    "socket",
    "bind",
    "listen",
    "accept",
    "connect",
    "send",
    "recv",

    "sendto",
    "recvfrom",
];

const checkAll = ref(true);
const isIndeterminate = ref(false);
const checkedMsgTypes = ref([]);
checkedMsgTypes.value = msgTypes;
emit("setSelectedTypes", checkedMsgTypes.value);

const handleCheckAllChange = (val) => {
    checkedMsgTypes.value = val ? msgTypes : [];
    isIndeterminate.value = false;
    emit("setSelectedTypes", val ? msgTypes : []);   
};
const handleCheckedTypesChange = (value) => {
    const checkedCount = value.length;
    checkAll.value = checkedCount === msgTypes.length;
    isIndeterminate.value =
        checkedCount > 0 && checkedCount < msgTypes.length;
    emit("setSelectedTypes", value);    
};

</script>