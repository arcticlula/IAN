import '@babel/polyfill'
import 'mutationobserver-shim'
import Vue from 'vue'
import './plugins/bootstrap-vue'
import App from './App.vue'
import './registerServiceWorker'
import router from './router'
import store from './store'
import { BootstrapVue, BIcon, BIconCircleHalf, BIconSquareHalf, BIconTriangleHalf, BIconStarHalf, BIconArrowsAngleContract, BIconArrowsAngleExpand, BIconUpload } from 'bootstrap-vue'

Vue.use(BootstrapVue)
Vue.component('BIcon', BIcon)
Vue.component('BIconCircleHalf', BIconCircleHalf)
Vue.component('BIconSquareHalf', BIconSquareHalf)
Vue.component('BIconTriangleHalf', BIconTriangleHalf)
Vue.component('BIconStarHalf', BIconStarHalf)
Vue.component('BIconArrowsAngleContract', BIconArrowsAngleContract)
Vue.component('BIconArrowsAngleExpand', BIconArrowsAngleExpand)
Vue.component('BIconUpload', BIconUpload)
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: function (h) { return h(App) }
}).$mount('#app')
