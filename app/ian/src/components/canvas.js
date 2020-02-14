import store from "../store/index"

export default function canvasShapes(canvas, context) {
	this.canvas = canvas;
	this.context = context;
	this.NCOLS = 45;
	this.canvas.height = this.canvas.width;
	this.divWidth = this.canvas.width / this.NCOLS;
	this.divHeight = this.canvas.height / this.NCOLS;
	this.colors =
		['rgb(84, 45, 220)', 'rgb(31, 83, 112)', 'rgb(139, 103, 87)', 'rgb(97, 164, 18)', 'rgb(5, 250, 24)', 'rgb(6, 51, 105)', 'rgb(10, 157, 84)', 'rgb(108, 16, 6)', 'rgb(86, 116, 84)', 'rgb(143, 234, 173)']

	let scopeCanvas = this;

	this.draw = function () {
		console.log("draw canvas", this.canvas.height, this.canvas.width)
		this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
		this.drawGrid();
		let patterns = store.state.patterns;
		console.log(patterns.shape, patterns.selected)
		switch (patterns.shape) {
			case "circle":
				if (patterns.selected == patterns.optionsShape[patterns.shape][0]) {
					this.drawCircleNote();
				}
				else if (patterns.selected == patterns.optionsShape[patterns.shape][1]) {
					this.drawCircleNoteGrad();
				}
				break;
			case "square":
				if (patterns.selected == patterns.optionsShape[patterns.shape][0]) {
					this.drawSquareNote();
				}
				break;
			case "triangle":
				break;
		}

		// for (var i = 0; i < this.channels.length; i++) {
		// 	// console.log(this.offsetX, this.offsetY)
		// 	this.channels[i].draw(this.offsetX, this.offsetY);
		// }
		// if (this.measureMode) {
		// 	this.measure.draw();
		// 	if (scopeViewer.mouseClickNum != 0) this.measure.drawLabel(prevPos.x, prevPos.y); //Only draw label when the measure exists
		// }

		// if (this.toggleStripMode) {
		// 	this.addStripMode.draw();
		// }
	}

	this.drawGrid = function () {
		this.context.globalAlpha = 0.1;
		this.context.strokeStyle = 'black';
		this.context.lineWidth = 1;
		let x = 0;
		this.context.beginPath();
		while (x < this.canvas.width) {
			this.context.moveTo(x, 0);
			this.context.lineTo(x, this.canvas.height);
			x = x + this.divWidth;
		}

		var y = 0;
		while (y < this.canvas.height) {
			this.context.moveTo(0, y);
			this.context.lineTo(this.canvas.width, y);
			y = y + this.divHeight;
		}
		this.context.stroke();
		this.context.globalAlpha = 1;
	}

	this.drawCircleNote = function () {
		let z = (this.NCOLS / 2);
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invI;
		let color = this.colors;
		for (let y = -z; y <= z; y++) {
			for (let x = -z; x <= z; x++) {
				for (let i = div - 1; i > -1; i--) {
					// boarder = 1;
					let perc = 1 - (i / div);
					if ((x * x) + (y * y) > (z * z)) {
						invI = orientation ? div : 0;
						this.drawPixel(x + z, y + z, color[invI]);
						break;
					}
					if ((x * x) + (y * y) <= (z * z) * (perc)) {
						invI = orientation ? div - 1 - i : i + 1;
						this.drawPixel(x + z, y + z, color[invI]);
						break;
					}
				}
			}
		}
	}

	this.drawSquareNote = function () {
		let z = (this.NCOLS / 2);
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invI;
		let color = this.colors;
		for (let y = -z; y <= z; y++) {
			for (let x = -z; x <= z; x++) {
				for (let i = div - 1; i > -1; i--) {
					// boarder = 1;
					let perc = 1 - (i / div);
					if ((x * x) + (y * y) > (z * z)) {
						invI = orientation ? div : 0;
						this.drawPixel(x + z, y + z, color[invI]);
						break;
					}
					if ((x * x) + (y * y) <= (z * z) * (perc)) {
						invI = orientation ? div - 1 - i : i + 1;
						this.drawPixel(x + z, y + z, color[invI]);
						break;
					}
				}
			}
		}
	}

	this.drawCircleNoteGrad = function () {
		let z = (this.NCOLS / 2);
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let boarder, invO;
		let diff = [];
		let color = [[225, 0, 200, 0.5], [0, 0, 100, 0.5]];
		let colorTemp;
		invO = orientation ? 0 : 1;
		for (let j = 0; j < 3; j++) {
			diff[j] = (color[orientation][j] - color[invO][j]) / div;
		}
		console.log(diff)
		console.log(color[orientation], color[invO])
		for (let y = -z; y <= z; y++) {
			for (let x = -z; x <= z; x++) {
				for (let i = 0; i < div; i++) {
					boarder = 1;
					let perc = 1 - (i / div);
					// printf("%0.1f\n", perc);
					if ((x * x) + (y * y) <= (z * z) * (perc)) {
						colorTemp = "rgba(";
						for (let j = 0; j < 3; j++) {
							colorTemp += color[orientation][j] - ((i + 1) * diff[j]) + ',';
						}
						colorTemp += "1)";
						// console.log(i, colorTemp);
						boarder = 0;
						this.drawPixel(x + z, y + z, colorTemp);
					}
					if (boarder) {
						if ((x * x) + (y * y) >= (z * z)) {
							colorTemp = "rgba(";
							for (let j = 0; j < 3; j++) {
								colorTemp += color[orientation][j] + ',';
							}
							colorTemp += "1)";
							this.drawPixel(x + z, y + z, colorTemp);
						}
					}
				}
			}
		}
	}

	// this.drawCircleNoteGrad = function () {
	// 	let z = (this.NCOLS / 2);
	// 	let orientation = store.state.patterns.orientation;
	// 	let div = store.state.patterns.div;
	// 	let boarder, invO;
	// 	let diff = [];
	// 	let color = [[225, 0, 200, 0.5], [0, 0, 100, 0.5]];
	// 	let colorTemp;
	// 	invO = orientation ? 0 : 1;
	// 	for (let j = 0; j < 3; j++) {
	// 		diff[j] = (color[orientation][j] - color[invO][j]) / div;
	// 	}
	// 	console.log(diff)
	// 	console.log(color[orientation], color[invO])
	// 	for (let y = -z; y <= z; y++) {
	// 		for (let x = -z; x <= z; x++) {
	// 			for (let i = 0; i < div; i++) {
	// 				boarder = 1;
	// 				let perc = 1 - (i / div);
	// 				// printf("%0.1f\n", perc);
	// 				if ((x * x) + (y * y) <= (z * z) * (perc)) {
	// 					colorTemp = "rgba(";
	// 					for (let j = 0; j < 3; j++) {
	// 						colorTemp += color[orientation][j] - ((i + 1) * diff[j]) + ',';
	// 					}
	// 					colorTemp += "1)";
	// 					// console.log(i, colorTemp);
	// 					boarder = 0;
	// 					this.drawPixel(x + z, y + z, colorTemp);
	// 				}
	// 				if (boarder) {
	// 					if ((x * x) + (y * y) >= (z * z)) {
	// 						colorTemp = "rgba(";
	// 						for (let j = 0; j < 3; j++) {
	// 							colorTemp += color[orientation][j] + ',';
	// 						}
	// 						colorTemp += "1)";
	// 						this.drawPixel(x + z, y + z, colorTemp);
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	this.drawPixel = function (x, y, color) {
		this.context.fillStyle = color;
		this.context.fillRect(x * this.divWidth, y * this.divHeight, this.divWidth, this.divHeight);
	}
}