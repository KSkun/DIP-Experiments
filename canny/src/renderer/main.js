import Vue from 'vue'
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css'
import VueMathjax from 'vue-mathjax'

import App from './App'

Vue.config.productionTip = false
Vue.use(ElementUI)
Vue.use(VueMathjax)

/* eslint-disable no-new */
new Vue({
  components: {App},
  template: '<App/>'
}).$mount('#app')

const electron = require('electron').remote
console.log(electron.app.getAppPath())

const addon = electron.require('../../build/Release/node-canny')
console.log(addon.hello())
