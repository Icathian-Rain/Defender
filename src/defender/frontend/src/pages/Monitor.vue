<template>
    <div class="flex flex-row justify-between items-center mb-5">
        <span> 当前信息数: {{ msgs.length }} </span>
        <el-button type="danger" @click="ClearMsgs().then(reFreshMsgs())">清空信息</el-button>
    </div>

    <div
        v-for="(msg, i) in msgs.slice((now_page - 1) * size, now_page * size)"
        :key="msg.id"
    >
        <MsgAlert :msg="msg" />
    </div>

    <el-pagination
        background
        layout="prev, pager, next"
        :page-count="Math.ceil(msgs.length / size)"
        v-model:current-page="now_page"
        class="absolute bottom-5 left-1/2 transform -translate-x-1/2"
        :hide-on-single-page="true"
    />
</template>

<script setup>
import { ref } from "vue";
import { EventsOn } from "../../wailsjs/runtime/runtime";
import { GetMsgs, ClearMsgs } from "../../wailsjs/go/main/App";
import { EventsOff } from "../../wailsjs/runtime/runtime";
import MsgAlert from "../components/MsgAlert.vue";

const msgs = ref([]);
const size = ref(6);
const now_page = ref(1);

let reFreshMsgs = () => {
    GetMsgs().then((data) => {
        msgs.value = [];
        for (let i = 0; i < data.length; i++) {
            msgs.value.push(JSON.parse(data[i]));
        }
    });
};

EventsOn("UDPMessage", () => {
    reFreshMsgs();
});


reFreshMsgs();
</script>