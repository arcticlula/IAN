<template>
	<b-col cols="12" md="4">
		<b-card>
			<b-tabs content-class="mt-3">
				<b-tab title="Patterns" active variant="info">
					<b-row class="mb-2" style="text-align: left;">
						<b-col cols="12">
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
								v-bind:class="{ active: patterns.shape == 'cross'}"
								@click="setShape('cross')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="grid-fill"></b-icon>
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
								v-bind:class="{ active: patterns.shape == 'lines'}"
								@click="setShape('lines')"
								size="sm"
								class="mr-1"
								variant="info"
							>
								<b-icon icon="pause"></b-icon>
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
								v-bind:class="{ active: patterns.shape == 'star'}"
								@click="setShape('star')"
								size="sm"
								variant="info"
							>
								<b-icon icon="starHalf"></b-icon>
							</b-button>
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
							<b-form-select size="sm"></b-form-select>
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
								v-bind:class="{ active: patterns.shape == 1}"
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
							<canvas style="width: 100%;" id="shapeCanvas"></canvas>
						</b-col>
					</b-row>
					<b-row style="text-align: right;">
						<b-col cols="12">
							<b-button style="background: #3793a0;" size="sm">
								<b-icon icon="upload"></b-icon>
							</b-button>
						</b-col>
					</b-row>
				</b-tab>
				<b-tab title="Text" color="info">
					<b-row class="mb-2">
						<b-col cols="12">
							<b-form-select size="sm"></b-form-select>
						</b-col>
					</b-row>
					<b-row style="text-align: left;">
						<b-col cols="12">
							<b-form-checkbox switch size="sm">Centrado</b-form-checkbox>
						</b-col>
					</b-row>
				</b-tab>
				<b-tab title="Colors" color="info"></b-tab>
			</b-tabs>
		</b-card>
	</b-col>
</template>

<script>
// @ is an alias to /src
import { mapState, mapMutations, mapGetters } from "vuex";
import canvasShapes from "@/components/canvas.js";
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
			canvasObj: {}
		};
	},
	computed: {
		...mapState(["bluetooth", "patterns"]),
		...mapGetters(["optionsShape"])
	},
	methods: {
		// ...mapMutations(["setDevicesSaved", "setDevicesUnpaired", "setState"]),
		setShape(shape) {
			this.patterns.shape = shape;
			this.currOptionShape = this.patterns.optionsShape[shape];
			this.patterns.selected = this.currOptionShape[0];
			this.currOptionDiv = this.patterns.optionsDiv[shape][
				this.patterns.selected
			];
			this.canvasObj.draw();
			// bluetoothSerial.write("S", this.success, this.failure);
		},
		setShapeDraw() {
			this.currOptionDiv = this.patterns.optionsDiv[this.patterns.shape][
				this.patterns.selected
			];
			this.patterns.div = this.currOptionDiv[0];
			console.log(this.currOptionDiv);
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
		this.currOptionShape = this.patterns.optionsShape[this.patterns.shape];
		this.patterns.selected = this.currOptionShape[0];
		this.currOptionDiv = this.patterns.optionsDiv[this.patterns.shape][
			this.patterns.selected
		];
		let canvas = document.getElementById("shapeCanvas");
		let ctx = canvas.getContext("2d");
		this.canvasObj = new canvasShapes(canvas, ctx);
		this.canvasObj.draw();
	}
};
</script>
