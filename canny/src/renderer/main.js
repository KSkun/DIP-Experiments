import Vue from 'vue'

import App from './App'

Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  components: {App},
  template: '<App/>'
}).$mount('#app')

const electron = require('electron').remote
console.log(electron.app.getAppPath())

const addon = electron.require('../../build/Release/node-canny')
console.log(addon.hello())
