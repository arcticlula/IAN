<template>
	<b-row class="about px-4">
		<b-col cols="12">
			<b-card>
				<b-tabs content-class="mt-3">
					<b-tab title="Patterns" active>
						<b-row class="mb-2" style="text-align: left;">
							<b-col cols="12">
								<b-button
									v-bind:class="{ active: patterns.shape == 'circle'}"
									@click="setShape('circle')"
									style="background: #3793a0;"
									size="sm"
									class="mr-1"
								>Círculo</b-button>
								<b-button
									v-bind:class="{ active: patterns.shape == 'square'}"
									@click="setShape('square')"
									style="background: #3793a0;"
									size="sm"
									class="mr-1"
								>Quadrado</b-button>
								<b-button
									v-bind:class="{ active: patterns.shape == 'triangle'}"
									@click="setShape('triangle')"
									style="background: #3793a0;"
									size="sm"
								>Triângulo</b-button>
							</b-col>
						</b-row>
						<b-row class="mb-2">
							<b-col cols="8" class="pr-0">
								<b-form-select
									@change="canvasObj.draw()"
									v-model="patterns.selected"
									:options="currOptionShape"
									size="sm"
								></b-form-select>
							</b-col>
							<b-col cols="4">
								<b-form-input size="sm"></b-form-input>
							</b-col>
						</b-row>
						<b-row class="mb-2">
							<b-col cols="12">
								<b-form-select size="sm"></b-form-select>
							</b-col>
						</b-row>
						<b-row class="mb-2" style="text-align: left;">
							<b-col cols="12">
								<b-button style="background: #3793a0;" size="sm" class="mr-1">Centro Fora</b-button>
								<b-button style="background: #3793a0;" size="sm" class="mr-1">Centro Dentro</b-button>
							</b-col>
						</b-row>
						<b-row class="mb-2">
							<b-col cols="12" style="border-radius: 5px; width: 100%;">
								<canvas id="shapeCanvas" style="border: 1px solid #3793a0;"></canvas>
							</b-col>
						</b-row>
						<b-row style="text-align: right;">
							<b-col cols="12">
								<b-button style="background: #3793a0;" size="sm">SET</b-button>
							</b-col>
						</b-row>
					</b-tab>
					<b-tab title="Text">
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
				</b-tabs>
			</b-card>
		</b-col>
	</b-row>
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
			this.canvasObj.draw();
			// bluetoothSerial.write("S", this.success, this.failure);
		},
		setShapeDraw() {
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
		let canvas = document.getElementById("shapeCanvas");
		let ctx = canvas.getContext("2d");
		this.canvasObj = new canvasShapes(canvas, ctx);
		this.canvasObj.draw();
	}
};
</script>
