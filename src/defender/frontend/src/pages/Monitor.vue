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
    msgs.value.splice(0, msgs.value.length);
    GetMsgs().then((data) => {
        for (let i = 0; i < data.length; i++) {
            let msg = Object();
            let info = Object();
            let items = data[i].split("\n");
            msg["id"] = i;
            msg[items[0]] = items[1];
            for (let j = 2; j < items.length; j = j + 2) {
                info[items[j]] = items[j + 1];
            }
            msg["info"] = info;
            msgs.value.push(msg);
        }
    });
};

EventsOn("UDPMessage", () => {
    reFreshMsgs();
});

reFreshMsgs();
</script>