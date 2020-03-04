import '@babel/polyfill'
import 'mutationobserver-shim'
import Vue from 'vue'
import './plugins/bootstrap-vue'
import App from './App.vue'
import './registerServiceWorker'
import router from './router'
import store from './store'
import { BootstrapVue, BIcon, BIconCircleHalf, BIconSquareHalf, BIconTriangleHalf, BIconStarHalf, BIconArrowsAngleContract, BIconArrowsAngleExpand, BIconUpload, BIconArrowRepeat, BIconGridFill, BIconPause, BIconPlus, BIconDash } from 'bootstrap-vue'

Vue.use(BootstrapVue)
Vue.component('BIcon', BIcon)
Vue.component('BIconCircleHalf', BIconCircleHalf)
Vue.component('BIconGridFill', BIconGridFill)
Vue.component('BIconSquareHalf', BIconSquareHalf)
Vue.component('BIconPause', BIconPause)
Vue.component('BIconTriangleHalf', BIconTriangleHalf)
Vue.component('BIconStarHalf', BIconStarHalf)
Vue.component('BIconArrowsAngleContract', BIconArrowsAngleContract)
Vue.component('BIconArrowsAngleExpand', BIconArrowsAngleExpand)
Vue.component('BIconUpload', BIconUpload)
Vue.component('BIconArrowRepeat', BIconArrowRepeat)
Vue.component('BIconPlus', BIconPlus)
Vue.component('BIconDash', BIconDash)
Vue.config.productionTip = false

new Vue({
	router,
	store,
	render: function (h) { return h(App) }
}).$mount('#app')
