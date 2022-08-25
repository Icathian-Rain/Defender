<template>
    <div v-for="msg in msgs">
        {{ msg }}
    </div>
</template>

<script setup>
import { ref } from "vue";
import { EventsOn } from "../../wailsjs/runtime/runtime";
import { onBeforeUnmount } from "vue";
import { EventsOff } from "../../wailsjs/runtime/runtime";

const msgs = ref([]);
msgs.value = JSON.parse(localStorage.getItem("msgs") || "[]");
EventsOn("UDPMessage", (msg) => {
    msgs.value.push(msg);
});

onBeforeUnmount(() => {
    localStorage.setItem("msgs", JSON.stringify(msgs.value));
    EventsOff("UDPMessage");
});

</script>