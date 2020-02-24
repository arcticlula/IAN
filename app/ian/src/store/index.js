import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    bluetooth: { state: "", devices: { saved: [{
      "class": 276,
      "id": "10:BF:48:CB:00:00",
      "address": "10:BF:48:CB:00:00",
      "name": "Nexus 7"
  }, {
      "class": 7936,
      "id": "00:06:66:4D:00:00",
      "address": "00:06:66:4D:00:00",
      "name": "RN42"
  }], unpaired: [] } },
    patterns: { shape: "circle", div: 4, orientation: 0, optionsShape: { circle: ["Normal", "Gradiente"], square: ["sq"], triangle: ["tr"] }, optionsDiv: { circle: { "Normal": [2, 3, 4, 5, 6, 7, 8, 9, 10], "Gradiente": [2, 3, 4, 5, 6, 7, 8, 9, 10] }, square: ["sq"], triangle: ["tr"], star: ["st"] }, selected: "" }
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
