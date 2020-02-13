import store from "../store/index"

export default function canvasShapes(canvas, context) {
	this.canvas = canvas;
	this.context = context;
	this.NCOLS = 45;
	this.canvas.height = this.canvas.width;
	this.divWidth = this.canvas.width / this.NCOLS;
	this.divHeight = this.canvas.height / this.NCOLS;
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
					this.drawCircleNote(0);
				}
				else if (patterns.selected == patterns.optionsShape[patterns.shape][1]) {
					this.drawCircleNoteGrad(0);
				}
				break;
			case "square":
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

	// this.drawCircleNote = function (orientation) {
	// 	let z = (this.NCOLS / 2);
	// 	let color = ['rgba(225,0,0,0.5)', 'rgba(0,225,0,0.5)', 'rgba(0,0,225,0.5)'];
	// 	let boarder;
	// 	let invI;
	// 	for (let y = -z; y <= z; y++) {
	// 		for (let x = -z; x <= z; x++) {
	// 			for (let i = 0; i < 3; i++) {
	// 				invI = orientation ? 3 - 1 - i : i + 1;
	// 				boarder = 1;
	// 				let perc = 1 - (i / 3);
	// 				if ((x * x) + (y * y) <= (z * z) * (perc)) {
	// 					this.drawPixel(x + z, y + z, color[invI]);
	// 					boarder = 0;
	// 				}
	// 			}
	// 			if (boarder) {
	// 				if ((x * x) + (y * y) >= (z * z)) {
	// 					invI = orientation ? 3 : 0;
	// 					this.drawPixel(x + z, y + z, color[invI]);
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	this.drawCircleNote = function (orientation) {
		let z = (this.NCOLS / 2);
		let div = store.state.patterns.div;
		let color = ['rgba(225,0,0,0.5)', 'rgba(0,225,0,0.5)', 'rgba(0,0,225,0.5)', 'rgba(120,20,200,0.5)', 'rgba(200,200,50,0.5)', 'rgba(56,200,150,0.5)'];
		let boarder;
		let invI;
		for (let y = -z; y <= z; y++) {
			for (let x = -z; x <= z; x++) {
				for (let i = 0; i < div; i++) {
					invI = orientation ? div - 1 - i : i + 1;
					boarder = 1;
					let perc = 1 - (i / div);
					if ((x * x) + (y * y) <= (z * z) * (perc)) {
						this.drawPixel(x + z, y + z, color[invI]);
						boarder = 0;
					}
				}
				if (boarder) {
					if ((x * x) + (y * y) >= (z * z)) {
						invI = orientation ? div : 0;
						this.drawPixel(x + z, y + z, color[invI]);
					}
				}
			}
		}
	}

	this.drawCircleNoteGrad = function (orientation) {
		let z = (this.NCOLS / 2);
		let div = store.state.patterns.div;
		let boarder, invO;
		let diff = [];
		let color = [[225, 0, 0, 0.5], [0, 200, 0, 0.5]];
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

	this.drawPixel = function (x, y, color) {
		this.context.fillStyle = color;
		this.context.fillRect(x * this.divWidth, y * this.divHeight, this.divWidth, this.divHeight);
	}
}