import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    bluetooth: { state: "", devices: { saved: [], unpaired: [] } },
    patterns: { shape: "circle", optionsShape: { circle: ["Normal", "Gradiente"], square: ["sq"], triangle: ["tr"] }, selected: "" }
  },
  getters: {
    // optionsShape(state) {
    //   console.log(state.patterns.shape, state.patterns.optionsShape[state.patterns.shape])
    //   return state.patterns.optionsShape[state.patterns.shape]
    // }
  },
  mutations: {
    setState(state, estado) {
      state.bluetooth.state = estado;
    },
    setDevicesSaved(state, devices) {
      state.bluetooth.devices.saved = devices;
    },
    setDevicesUnpaired(state, devices) {
      state.bluetooth.devices.unpaired = devices;
    },
  },
  actions: {
  },
  modules: {
  }
})
