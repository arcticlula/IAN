import Vue from 'vue'
import Vuex from 'vuex'
import GV from "../components/globalVars"

Vue.use(Vuex)

export default new Vuex.Store({
	state: {
		bluetooth: {
			state: "", devices: {
				saved: [], unpaired: []
			}
		},
		settings: { power: { back: "On", text: "Off" }, brightness: 1 },
		patterns: { shape: "circle", div: 4, orientation: 0, pixel: "Circle", selected: "", color: GV.BACK_COLORS_OG },
		optionsShape: { circle: [{ text: "Normal", value: GV.CIRCLE_NORMAL }, { text: "Gradiente", value: GV.CIRCLE_GRAD }], cross: [{ text: "Normal", value: GV.CROSS_NORMAL }, { text: "Gradiente", value: GV.CROSS_GRAD }], square: [{ text: "Normal", value: GV.SQUARE_NORMAL }, { text: "Gradiente", value: GV.SQUARE_GRAD }], lines: [{ text: "Horizontal", value: GV.LINES_H }, { text: "H. Gradiente", value: GV.LINES_H_GRAD }, { text: "Vertical", value: GV.LINES_V }, { text: "V. Gradiente", value: GV.LINES_V_GRAD }], triangle: [{ text: "Normal", value: GV.TRIANGLE_NORMAL }, { text: "Gradiente", value: GV.TRIANGLE_GRAD }], star: ["st"] },
		optionsDiv: { circle: { [GV.CIRCLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.CIRCLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, cross: { [GV.CROSS_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.CROSS_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, square: { [GV.SQUARE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.SQUARE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, lines: { [GV.LINES_H]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.LINES_H_GRAD]: [4, 5, 6, 7, 8, 9], [GV.LINES_V]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.LINES_V_GRAD]: [4, 5, 6, 7, 8, 9] }, triangle: { [GV.TRIANGLE_NORMAL]: [2, 3, 4, 5, 6, 7, 8, 9], [GV.TRIANGLE_GRAD]: [3, 4, 5, 6, 7, 8, 9] }, star: ["st"] },
		optionsColors: [{ text: "Original", value: GV.BACK_COLORS_OG }, { text: "BW", value: GV.BACK_COLORS_BW }],
		customPatterns: {      // Brilho 0-20, Modo Back on/off, Pattern - Value, Max Div = Number, Orientation 0 - 1, Modo Text on/off, Words, Colors - Value
			WTECS: [
				GV.MODE_CUSTOM,
				3,
				1,
				GV.LINES_H,
				5,
				1,
				1,
				GV.TEXT_WORDS_WTECS,
				GV.BACK_COLORS_OG
			],
			EW1: [
				GV.MODE_CUSTOM,
				10,
				1,
				GV.CIRCLE_NORMAL,
				3,
				0,
				0,
				GV.TEXT_WORDS_WTECS,
				GV.BACK_COLORS_OG
			],
			EW2: [
				GV.MODE_CUSTOM,
				12,
				1,
				GV.CROSS_NORMAL,
				5,
				0,
				0,
				GV.TEXT_WORDS_WTECS,
				GV.BACK_COLORS_OG
			],
			BN: [
				GV.MODE_CUSTOM,
				1,
				0,
				GV.LINES_H,
				1,
				0,
				0,
				GV.TEXT_WORDS_WTECS,
				GV.BACK_COLORS_OG
			],
			BC: [
				GV.MODE_CUSTOM,
				4,
				2,
				GV.CIRCLE_GRAD,
				9,
				0,
				1,
				GV.TEXT_WORDS_OET,
				GV.BACK_COLORS_OG
			],
			NN1: [
				GV.MODE_CUSTOM,
				20,
				1,
				GV.CIRCLE_GRAD,
				8,
				0,
				0,
				GV.TEXT_WORDS_OET,
				GV.BACK_COLORS_BW
			],
			NN2: [
				GV.MODE_CUSTOM,
				19,
				1,
				GV.SQUARE_GRAD,
				8,
				1,
				0,
				GV.TEXT_WORDS_OET,
				GV.BACK_COLORS_BW
			]
		}
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
