import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

let ACK = 0x01;
let NACK = 0x02;
let SETTINGS_BRIGHT = 0x11;
let BACK_STATE = 0x20;
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
let TRIANGLE_NORMAL = 0x40;
let TRIANGLE_GRAD = 0x41;
let TEXT_STATE = 0x60;

export default new Vuex.Store({
  state: {
	bluetooth: { state: "", devices: { saved: [{
	  "class": 276,
	  "id": "10:BF:48:CB:00:00",
	  "address": "10:BF:48:CB:00:00",
	  "name": "Nexus 7 2123151511"
  }, {
	  "class": 7936,
	  "id": "00:06:66:4D:00:00",
	  "address": "00:06:66:4D:00:00",
	  "name": "RN42"
  }], unpaired: [{
	"class": 276,
	"id": "10:BF:48:CB:00:00",
	"address": "10:BF:48:CB:00:00",
	"name": "Nexus 7"
}, {
	"class": 7936,
	"id": "00:06:66:4D:00:00",
	"address": "00:06:66:4D:00:00",
	"name": "RN42"
}] } },
	settings: { power: { back: "On", text: "Off" }, brightness: 1 },
	patterns: { shape: "circle", div: 4, orientation: 0, pixel: "Circle", selected: "" },
	optionsShape: { circle: [{ text: "Normal", value: CIRCLE_NORMAL }, { text: "Gradiente", value: CIRCLE_GRAD }], cross: [{ text: "Normal", value: CROSS_NORMAL }, { text: "Gradiente", value: CROSS_GRAD }], square: [{ text: "Normal", value: SQUARE_NORMAL }, { text: "Gradiente", value: SQUARE_GRAD }], lines: [{ text: "Horizontal", value: LINES_H }, { text: "H. Gradiente", value: LINES_H_GRAD }, { text: "Vertical", value: LINES_V }, { text: "V. Gradiente", value: LINES_V_GRAD }], triangle: [{ text: "Normal", value: TRIANGLE_NORMAL }, { text: "Gradiente", value: TRIANGLE_GRAD }], star: ["st"] },
	optionsDiv: { circle: { [CIRCLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [CIRCLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, cross: { [CROSS_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [CROSS_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, square: { [SQUARE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [SQUARE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, lines: { [LINES_H]: [2, 3, 4, 5, 6, 7, 8, 9], [LINES_H_GRAD]: [4, 5, 6, 7, 8, 9], [LINES_V]: [2, 3, 4, 5, 6, 7, 8, 9], [LINES_V_GRAD]: [4, 5, 6, 7, 8, 9] }, triangle: { [TRIANGLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [TRIANGLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, star: ["st"] }
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
