<template>
    <div class="flex flex-row justify-between items-center">
        <span> 当前信息数: {{ num }} </span>
        <div>
            <el-button
                type="danger"
                @click="ClearMsgs().then(reFreshMsgs())"
                class="mr-4"
                >清空信息</el-button
            >
            <div class="inline-block cursor-pointer" @click="showSelector = !showSelector">
                <img
                    src="../assets/icon/select.svg"
                    alt=""
                    class="inline h-7"
                />
                筛选
            </div>
        </div>
    </div>
    <Selectors @setSelectedTypes="setSelectedTypes" v-if="showSelector" />
    <el-divider />
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
        class="relative"
        :hide-on-single-page="true"
    />
</template>

<script setup>
import { ref } from "vue";
import { EventsOn } from "../../wailsjs/runtime/runtime";
import { GetMsgs, ClearMsgs } from "../../wailsjs/go/main/App";
import { EventsOff } from "../../wailsjs/runtime/runtime";
import MsgAlert from "../components/MsgAlert.vue";
import Selectors from "../components/Selectors.vue";

const msgs = ref([]);
const num = ref(0);
const size = ref(6);
const now_page = ref(1);
const selectedTypes = ref([]);
const showSelector = ref(true);

let reFreshMsgs = () => {
    GetMsgs().then((data) => {
        msgs.value = [];
        num.value = data.length;
        for (let i = 0; i < data.length; i++) {
            let msg = JSON.parse(data[i]);
            if (selectedTypes.value.includes(msg["funcName"])) {
                msgs.value.push(msg);
            }
        }
    });
};

EventsOn("UDPMessage", () => {
    reFreshMsgs();
});

let setSelectedTypes = (val) => {
    selectedTypes.value = val;
    reFreshMsgs();
};

reFreshMsgs();
</script>