import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    bluetooth: { state: "", devices: { saved: [], unpaired: [] } },
    patterns: { shape: "circle", div: 4, orientation: 0, optionsShape: { circle: ["Normal", "Gradiente"], cross: ["Normal", "Gradiente"], square: ["Normal", "Gradiente"], lines: ["Horizontal", "H. Gradiente", "Vertical", "V. Gradiente"], triangle: ["tr"] }, optionsDiv: { circle: { "Normal": [2, 3, 4, 5, 6, 7, 8, 9, 10], "Gradiente": [2, 3, 4, 5, 6, 7, 8, 9, 10] }, cross: { "Normal": [2, 3, 4, 5, 6, 7, 8, 9, 10], "Gradiente": [2, 3, 4, 5, 6, 7, 8, 9, 10] }, square: { "Normal": [2, 3, 4, 5, 6, 7, 8, 9, 10], "Gradiente": [2, 3, 4, 5, 6, 7, 8, 9, 10] }, lines: { "Horizontal": [2, 3, 4, 5, 6, 7, 8, 9, 10], "H. Gradiente": [2, 3, 4, 5, 6, 7, 8, 9, 10], "Vertical": [2, 3, 4, 5, 6, 7, 8, 9, 10], "V. Gradiente": [2, 3, 4, 5, 6, 7, 8, 9, 10] }, triangle: ["tr"], star: ["st"] }, selected: "" }
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
