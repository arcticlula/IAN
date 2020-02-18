import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

let CIRCLE_NORMAL = 0x21;
let CIRCLE_GRAD = 0x22;
let CROSS_NORMAL = 0x25;
let CROSS_GRAD = 0x26;
let SQUARE_NORMAL = 0x30;
let SQUARE_GRAD = 0x31;
let LINES_H = 0x35;
let LINES_H_GRAD = 0x36;
let LINES_V = 0x37;
let LINES_V_GRAD = 0x38;

export default new Vuex.Store({
  state: {
    bluetooth: { state: "", devices: { saved: [], unpaired: [] } },
    patterns: { shape: "circle", div: 4, orientation: 0, pixel: "Circle", selected: "" },
    optionsShape: { circle: [{ text: "Normal", value: CIRCLE_NORMAL }, { text: "Gradiente", value: CIRCLE_GRAD }], cross: [{ text: "Normal", value: CROSS_NORMAL }, { text: "Gradiente", value: CROSS_GRAD }], square: [{ text: "Normal", value: SQUARE_NORMAL }, { text: "Gradiente", value: SQUARE_GRAD }], lines: [{ text: "Horizontal", value: LINES_H }, { text: "H. Gradiente", value: LINES_H_GRAD }, { text: "Vertical", value: LINES_V }, { text: "V. Gradiente", value: LINES_V_GRAD }], triangle: ["tr"], star: ["st"] },
    optionsDiv: { circle: { [CIRCLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [CIRCLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, cross: { [CROSS_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [CROSS_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, square: { [SQUARE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [SQUARE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, lines: { [LINES_H]: [2, 3, 4, 5, 6, 7, 8, 9], [LINES_H_GRAD]: [4, 5, 6, 7, 8, 9], [LINES_V]: [2, 3, 4, 5, 6, 7, 8, 9], [LINES_V_GRAD]: [4, 5, 6, 7, 8, 9] }, triangle: ["tr"], star: ["st"] }
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
