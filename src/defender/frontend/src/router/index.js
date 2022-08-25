import { createRouter, createWebHashHistory } from "vue-router";
import Home from '../pages/Home.vue';
import Monitor from '../pages/Monitor.vue';
import Settings from '../pages/Settings.vue';


const routes = [
    { path: '/', component: Home },
    { path: '/monitor', component: Monitor },
    { path: '/settings', component: Settings },
]

const router = createRouter({
    history: createWebHashHistory(),
    routes,
})

export default router;