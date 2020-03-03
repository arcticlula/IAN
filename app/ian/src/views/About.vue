<template>
	<b-col cols="12" md="4">
		<b-card>
			<b-tabs content-class="mt-3" style="font-size: 14px;">
				<b-tab title="Patterns" active variant="info">
					<b-row class="mb-2" style="text-align: left;">
						<b-col cols="8">
							<b-button
								v-bind:class="{ active: patterns.shape == 'circle'}"
								@click="setShape('circle')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="circleHalf"></b-icon>
							</b-button>
							<b-button
								v-bind:class="{ active: patterns.shape == 'square'}"
								@click="setShape('square')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="squareHalf"></b-icon>
							</b-button>
							<b-button
								v-bind:class="{ active: patterns.shape == 'triangle'}"
								@click="setShape('triangle')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="triangleHalf"></b-icon>
							</b-button>
							<b-button
								v-bind:class="{ active: patterns.shape == 'cross'}"
								@click="setShape('cross')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="grid-fill"></b-icon>
							</b-button>

							<b-button
								v-bind:class="{ active: patterns.shape == 'lines'}"
								@click="setShape('lines')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="pause"></b-icon>
							</b-button>
						</b-col>
						<b-col cols="4">
							<b-form-checkbox
								style="text-align: right;"
								@change="setSingleDef('back')"
								v-model="settings.power.back"
								switch
								value="On"
								unchecked-value="Off"
								size="md"
							>{{settings.power.back}}</b-form-checkbox>
							<!-- <b-button
								v-bind:class="{ active: patterns.shape == 'star'}"
								@click="setShape('star')"
								size="sm"
								variant="info"
							>
								<b-icon icon="starHalf"></b-icon>
							</b-button>-->
						</b-col>
					</b-row>
					<b-row class="mb-2">
						<b-col cols="8" class="pr-0">
							<b-form-select
								@change="setShapeDraw"
								v-model="patterns.selected"
								:options="currOptionShape"
								size="sm"
							></b-form-select>
						</b-col>
						<b-col cols="4">
							<b-form-select
								@change="canvasObj.draw()"
								v-model="patterns.div"
								:options="currOptionDiv"
								size="sm"
							>
								<!-- <option v-for="opt in currOptionDiv" :value="opt" :key="opt">{{opt}}</option> -->
							</b-form-select>
						</b-col>
					</b-row>
					<b-row class="mb-2">
						<b-col cols="12">
							<!-- <b-form-select size="sm"></b-form-select> -->
						</b-col>
					</b-row>
					<b-row class="mb-2" style="text-align: left;">
						<b-col cols="12">
							<b-button
								v-bind:class="{ active: patterns.orientation == 0}"
								@click="setOrientation(0)"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="arrowsAngleExpand"></b-icon>
							</b-button>
							<b-button
								v-bind:class="{ active: patterns.orientation == 1}"
								@click="setOrientation(1)"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="arrowsAngleContract"></b-icon>
							</b-button>
						</b-col>
					</b-row>
					<b-row class="mb-2">
						<b-col cols="12" style="border-radius: 5px; width: 100%; margin: 0 auto; text-align: center;">
							<canvas id="shapeCanvas" style="width: 100%;"></canvas>
						</b-col>
					</b-row>
					<b-row>
						<b-col cols="12">
							<b-form-checkbox
								@change="setPixel"
								v-model="patterns.pixel"
								switch
								value="Circle"
								unchecked-value="Square"
								size="sm"
							>{{patterns.pixel}}</b-form-checkbox>
						</b-col>
					</b-row>
					<b-row style="text-align: right;">
						<b-col cols="12">
							<b-button @click="sendToDevice" style="background: #3793a0;" size="sm">
								<b-icon icon="upload"></b-icon>
							</b-button>
						</b-col>
					</b-row>
				</b-tab>
				<b-tab title="Text" color="info">
					<b-row class="mb-2">
						<b-col cols="8">
							<b-form-select size="sm"></b-form-select>
						</b-col>
						<b-col cols="4">
							<b-form-checkbox
								style="text-align: right;"
								@change="setSingleDef('text')"
								v-model="settings.power.text"
								switch
								value="On"
								unchecked-value="Off"
								size="md"
							>{{settings.power.text}}</b-form-checkbox>
						</b-col>
					</b-row>
					<b-row style="text-align: left;">
						<b-col cols="12">
							<b-form-checkbox switch size="sm">Centrado</b-form-checkbox>
						</b-col>
					</b-row>
				</b-tab>
				<b-tab title="Colors" color="info">
					<b-row>
						<b-col cols="12" class="mb-1">
							<b-form-radio-group
								@change="setSingleDef('colors')"
								v-model="patterns.color"
								:options="optionsColors"
								button-variant="outline-info"
								buttons
								size="sm"
								name="colors"
							></b-form-radio-group>
						</b-col>
					</b-row>
					<b-row class="mt-2">
						<b-col cols="12" style="width: 100%;">
							<canvas id="colorCanvas" style="width: 100%; border-radius: 3px; "></canvas>
						</b-col>
					</b-row>
					<!-- <swatches row-length="5" colors="basic" v-model="color"></swatches>
          <swatches row-length="5" colors="basic" v-model="color"></swatches>
					<swatches row-length="5" colors="basic" v-model="color"></swatches>-->
				</b-tab>
				<b-tab title="Settings" color="info">
					<b-row>
						<b-col cols="12">
							<label class="mt-0">Brightness</label>
						</b-col>
					</b-row>
					<b-row>
						<b-col cols="12">
							<b-form-input
								@change="setSingleDef('bright')"
								type="range"
								min="1"
								max="20"
								step="1"
								v-model.number="settings.brightness"
							/>
						</b-col>
					</b-row>
					<b-row>
						<b-col cols="12" class="mb-2">
							<b-button style="width: 100%;" variant="info">What the eyes cannot see</b-button>
						</b-col>
						<b-col cols="12" class="mb-2">
							<b-button-group style="width: 100%;">
								<b-button variant="info">Electric waves A</b-button>
								<b-button variant="danger">Electric waves B</b-button>
							</b-button-group>
						</b-col>
						<b-col cols="12" class="mb-2">
							<b-button variant="info" style="width: 100%;">Boarding now</b-button>
						</b-col>
						<b-col cols="12" class="mb-2">
							<b-button variant="info" style="width: 100%;">Black Celebration</b-button>
						</b-col>
						<b-col cols="12" class="mb-2">
							<b-button variant="info" style="width: 100%;">No name</b-button>
						</b-col>
					</b-row>
				</b-tab>
			</b-tabs>
		</b-card>
	</b-col>
</template>

<script>
// @ is an alias to /src
import { mapState, mapMutations, mapGetters } from "vuex";
import canvasShapes from "@/components/canvas.js";
import canvasColors from "@/components/canvasColors.js";
import hdlc from "@/components/hdlc.js";
import Swatches from "vue-swatches";
import "vue-swatches/dist/vue-swatches.min.css";
import GV from "../components/globalVars";
// import HelloWorld from "@/components/HelloWorld.vue";

export default {
	name: "Home",
	// components: {
	//   HelloWorld
	// },
	data: function() {
		return {
			currOptionShape: [],
			currOptionDiv: [],
			canvasObj: {},
			canvasColObj: {},
			color: "",
			customPatterns: {
				"1": [
					GV.MODE_CUSTOM,
					10,
					1,
					GV.CROSS_GRAD,
					8,
					0,
					0,
					GV.BACK_COLORS_OG
				],
				"2": [
					GV.MODE_CUSTOM,
					10,
					1,
					GV.SQUARE_GRAD,
					8,
					0,
					0,
					GV.BACK_COLORS_BW
				],
				"3": []
			}
			// Brilho 0-20, Modo Back 0-1, Pattern - Value, Max Div = Number, Orientation 0 - 1, Modo Text on/off, Colors - Value
		};
	},
	components: { Swatches },
	computed: {
		...mapState([
			"bluetooth",
			"settings",
			"patterns",
			"optionsShape",
			"optionsDiv",
			"optionsColors"
		])
	},
	methods: {
		// ...mapMutations(["setDevicesSaved", "setDevicesUnpaired", "setState"]),
		// sendCommBluetooth(message) {
		//   let msg = this.enc.encode(message);
		//   console.log(msg);
		//   this.hdlc.minihdlc_send_frame(msg, msg.length);
		// },
		setPixel(px) {
			this.patterns.pixel = px;
			this.canvasObj.draw();
		},
		blueSend(data) {
			// let hex = Number(data).toString(16);
			// if (hex.length < 2) {
			//   hex = "0" + hex;
			// }

			// console.log(data, hex);
			let element = new Uint8Array([data]);
			bluetoothSerial.write(element, this.success, this.failure);
		},
		blueProcess(data, length) {
			for (let i = 0; i < length; i++) {
				console.log(data[i]);
				// console.log(String.fromCharCode(data[i]));
			}
		},
		setSingleDef(state) {
			this.$nextTick(() => {
				let msg = [];
				switch (state) {
					case "back":
						msg.push(GV.BACK_STATE);
						this.settings.power.back == "On"
							? msg.push(1)
							: msg.push(0);
						break;
					case "text":
						msg.push(GV.TEXT_STATE);
						this.settings.power.text == "On"
							? msg.push(1)
							: msg.push(0);
						break;
					case "bright":
						msg.push(GV.SETTINGS_BRIGHT);
						msg.push(this.settings.brightness);
						break;
					case "colors":
						console.log(this.patterns.color);
						this.canvasColObj.draw();
						msg.push(this.patterns.color);
						break;
				}
				this.hdlc.minihdlc_send_frame(msg, msg.length);
			});
		},
		sendToDevice() {
			let msg = [];
			msg.push(this.patterns.selected);
			msg.push(this.patterns.div);
			msg.push(this.patterns.orientation);
			console.log(msg);
			this.hdlc.minihdlc_send_frame(msg, msg.length);
		},
		sendCustom() {
			let msg = this.customPatterns["1"];
			this.hdlc.minihdlc_send_frame(msg, msg.length);
		},
		setShape(shape) {
			this.patterns.shape = shape;
			this.currOptionShape = this.optionsShape[shape];
			this.patterns.selected = this.currOptionShape[0].value;
			this.currOptionDiv = this.optionsDiv[shape][this.patterns.selected];
			this.canvasObj.draw();
		},
		setShapeDraw() {
			this.currOptionDiv = this.optionsDiv[this.patterns.shape][
				this.patterns.selected
			];
			this.patterns.div = this.currOptionDiv[0];
			this.canvasObj.draw();
		},
		setOrientation(orientation) {
			this.patterns.orientation = orientation;
			this.canvasObj.draw();
		},
		success() {
			console.log("sim");
		},
		failure() {
			console.log("nao");
		}
	},
	mounted() {
		let scope = this;
		// this.enc = new TextEncoder(); // always utf-8
		this.currOptionShape = this.optionsShape[this.patterns.shape];
		this.patterns.selected = this.currOptionShape[0].value;
		// console.log(this.optionsDiv[this.patterns.shape], this.patterns.selected);
		this.currOptionDiv = this.optionsDiv[this.patterns.shape][
			this.patterns.selected
		];
		let canvas = document.getElementById("shapeCanvas");
		let ctx = canvas.getContext("2d");
		let canvasCol = document.getElementById("colorCanvas");
		let ctxCol = canvasCol.getContext("2d");
		this.canvasObj = new canvasShapes(canvas, ctx);
		this.canvasObj.draw();
		this.canvasColObj = new canvasColors(canvasCol, ctxCol);
		this.canvasColObj.draw();

		this.hdlc = new hdlc();
		this.hdlc.minihdlc_init(this.blueSend, this.blueProcess);
		bluetoothSerial.subscribeRawData(
			function(data) {
				// console.log("hey");
				let bytes = new Uint8Array(data);
				for (let i = 0; i < bytes.length; i++) {
					const element = bytes[i];
					scope.hdlc.minihdlc_char_receiver(element);
				}
			},
			function() {
				console.log("nope");
			}
		);
	}
};
</script>
<style>
:focus {
	-webkit-box-shadow: none !important;
	box-shadow: none !important;
	outline: none !important;
}

.btn-outline-info:focus,
.btn-outline-info.focus {
	-webkit-box-shadow: none !important;
	box-shadow: none !important;
	outline: none !important;
}
</style>