import store from "../store/index"
let BACK_COLORS_OG = 0x51;
let BACK_COLORS_BW = 0x52;

export default function canvasColors(canvas, context) {
	this.canvas = canvas;
	this.context = context;
	this.NCOLS = 12;
	this.canvas.height = this.canvas.width / this.NCOLS;
	this.divWidth = this.canvas.width / this.NCOLS;
	this.colors =
	{
		[BACK_COLORS_OG]: [[123, 0, 0], [123, 90, 0], [123, 123, 0], [123, 109, 0], [0, 123, 106], [90, 0, 123], [123, 0, 90], [123, 0, 123], [0, 0, 112], [98, 98, 98], [133, 133, 82], [133, 98, 82]],
		[BACK_COLORS_BW]: [[78, 78, 78], [116, 99, 94], [101, 105, 105], [72, 72, 36], [39, 39, 39], [58, 49, 47], [50, 53, 53], [36, 36, 18], [117, 117, 117], [174, 148, 141], [151, 158, 158], [108, 108, 54]]
	}
	
	

	let scopeCanvas = this;

	this.draw = function () {
		this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
		this.drawColors();
	}

	this.drawColors = function () {
		console.log(store.state.patterns.color)
		for (let i = 0; i < this.NCOLS; i++) {
			this.context.fillStyle = `rgb(
				${this.colors[store.state.patterns.color][i][0]},
				${this.colors[store.state.patterns.color][i][1]},
				${this.colors[store.state.patterns.color][i][2]}`;
			console.log(this.colors[i], i * this.divWidth, 0, this.divWidth, this.canvas.height);
			this.context.fillRect(i * this.divWidth, 0, this.divWidth, this.canvas.height);

		}
	}
}