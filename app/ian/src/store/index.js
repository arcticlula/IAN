import Vue from 'vue'
import Vuex from 'vuex'
import GV from "../components/globalVars"

Vue.use(Vuex)

export default new Vuex.Store({
	state: {
		bluetooth: {
			state: "", devices: {
				saved: [{
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
				}]
			}
		},
		settings: { power: { back: "On", text: "Off" }, brightness: 1 },
		patterns: { shape: "circle", div: 4, orientation: 0, pixel: "Circle", selected: "", color: GV.BACK_COLORS_OG },
		optionsShape: { circle: [{ text: "Normal", value: GV.CIRCLE_NORMAL }, { text: "Gradiente", value: GV.CIRCLE_GRAD }], cross: [{ text: "Normal", value: GV.CROSS_NORMAL }, { text: "Gradiente", value: GV.CROSS_GRAD }], square: [{ text: "Normal", value: GV.SQUARE_NORMAL }, { text: "Gradiente", value: GV.SQUARE_GRAD }], lines: [{ text: "Horizontal", value: GV.LINES_H }, { text: "H. Gradiente", value: GV.LINES_H_GRAD }, { text: "Vertical", value: GV.LINES_V }, { text: "V. Gradiente", value: GV.LINES_V_GRAD }], triangle: [{ text: "Normal", value: GV.TRIANGLE_NORMAL }, { text: "Gradiente", value: GV.TRIANGLE_GRAD }], star: ["st"] },
		optionsDiv: { circle: { [GV.CIRCLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.CIRCLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, cross: { [GV.CROSS_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.CROSS_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, square: { [GV.SQUARE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.SQUARE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, lines: { [GV.LINES_H]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.LINES_H_GRAD]: [4, 5, 6, 7, 8, 9], [GV.LINES_V]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.LINES_V_GRAD]: [4, 5, 6, 7, 8, 9] }, triangle: { [GV.TRIANGLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.TRIANGLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, star: ["st"] },
		optionsColors: [{ text: "Original", value: GV.BACK_COLORS_OG }, { text: "BW", value: GV.BACK_COLORS_BW }]
	},
	getters: {
		// optionsShape(state) {npm 
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
