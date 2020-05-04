import Vue from 'vue'
import App from './App.vue'
//import './plugins/element.js'
import ElementUI from 'element-ui'
import 'element-ui/lib/theme-chalk/index.css'

import VueRouter from 'vue-router'
import setting from './components/setting.vue';
import basicsetting from './components/basicsetting.vue';
import speedsetting from './components/speedsetting.vue';
import status from './components/status.vue';
import login from './components/login.vue';
import home from './components/home.vue';
import mapping from './components/mapping.vue';
import taskqueue from './components/taskqueue.vue';
import pathandpoint from './components/pathandpoint.vue';

Vue.config.productionTip = false
Vue.use(ElementUI);
Vue.use(VueRouter);

// 1. 定义 (路由) 组件。
// 可以从其他文件 import 进来

// 2. 定义路由
// 每个路由应该映射一个组件。 其中"component" 可以是
// 通过 Vue.extend() 创建的组件构造器，
// 或者，只是一个组件配置对象。
// 我们晚点再讨论嵌套路由。
const routes = [
  { path: '/setting', component: setting },
  { path: '/basicsetting', component: basicsetting },
  { path: '/speedsetting', component: speedsetting },
  { path: '/status', component: status },
  { path: '/login', component: login },
  { path: '/home', component: home },
  { path: '/mapping', component: mapping },
  { path: '/taskqueue', component: taskqueue },
  { path: '/pathandpoint', component:pathandpoint},
]

// 3. 创建 router 实例，然后传 `routes` 配置
// 你还可以传别的配置参数, 不过先这么简单着吧。
const router = new VueRouter({
  routes // (缩写) 相当于 routes: routes
})

// 4. 创建和挂载根实例。
// 记得要通过 router 配置参数注入路由，
// 从而让整个应用都有路由功能
new Vue({
  router,
  render: h => h(App),
}).$mount('#app')

