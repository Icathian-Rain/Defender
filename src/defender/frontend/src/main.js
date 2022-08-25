import {createApp} from 'vue'
import App from './App.vue'
import router from './router'   // 引入路由
import 'virtual:windi.css'      // windi.css

const app = createApp(App)
app.use(router)     // 将路由挂载到vue实例上
app.mount('#app')
