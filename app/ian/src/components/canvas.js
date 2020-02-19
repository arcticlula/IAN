import store from "../store/index"

export default function canvasShapes(canvas, context) {
	this.canvas = canvas;
	this.context = context;
	this.NCOLS = 45;
	this.z = this.NCOLS / 2;
	this.rz = Math.floor(this.z)
	this.canvas.height = this.canvas.width;
	this.divWidth = this.canvas.width / this.NCOLS;
	this.divHeight = this.canvas.height / this.NCOLS;
	this.colors =
		['rgb(84, 45, 220)', 'rgb(31, 83, 112)', 'rgb(139, 103, 87)', 'rgb(97, 164, 18)', 'rgb(5, 250, 24)', 'rgb(6, 51, 105)', 'rgb(10, 157, 84)', 'rgb(108, 16, 6)', 'rgb(86, 116, 84)', 'rgb(143, 234, 173)']

	// this.colors =
	// 	['rgba(84, 45, 220,0.5)', 'rgba(31, 83, 112,0.5)', 'rgba(139, 103, 87,0.5)', 'rgba(97, 164, 18,0.5)', 'rgba(5, 250, 24,0.5)', 'rgba(6, 51, 105,0.5)', 'rgba(10, 157, 84,0.5)', 'rgba(108, 16, 6,0.5)', 'rgba(86, 116, 84,0.5)', 'rgba(143, 234, 173,0.5)']

	let scopeCanvas = this;

	this.draw = function () {
		// console.log("draw canvas", this.canvas.height, this.canvas.width)
		this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
		this.drawGrid();
		let patterns = store.state.patterns;
		let optionsShape = store.state.optionsShape;
		// console.log(patterns.selected, optionsShape[patterns.shape])
		switch (patterns.shape) {
			case "circle":
				if (patterns.selected == optionsShape[patterns.shape][0].value) {
					this.drawCircleNote();
				}
				else if (patterns.selected == optionsShape[patterns.shape][1].value) {
					this.drawCircleNoteGrad();
				}
				break;
			case "cross":
				if (patterns.selected == optionsShape[patterns.shape][0].value) {
					this.drawCrossNote();
				}
				else if (patterns.selected == optionsShape[patterns.shape][1].value) {
					this.drawCrossNoteGrad();
				}
				break;
			case "square":
				if (patterns.selected == optionsShape[patterns.shape][0].value) {
					this.drawSquareNote();
				}
				else if (patterns.selected == optionsShape[patterns.shape][1].value) {
					this.drawSquareNoteGrad();
				}
				break;
			case "lines":
				if (patterns.selected == optionsShape[patterns.shape][0].value) {
					this.drawLineHNote();
				}
				else if (patterns.selected == optionsShape[patterns.shape][1].value) {
					this.drawLineHNoteGrad();
				}
				else if (patterns.selected == optionsShape[patterns.shape][2].value) {
					this.drawLineVNote();
				}
				else if (patterns.selected == optionsShape[patterns.shape][3].value) {
					this.drawLineVNoteGrad();
				}
				break;
			case "triangle":
				break;
		}
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
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invI;
		let color = this.colors;
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				invI = orientation ? div - 1 : 0;
				for (let i = 0; i < div - 1; i++) {
					let perc = (i + 1) / div;
					if ((x * x) + (y * y) <= (z * z) * (perc)) {
						invI = orientation ? i : div - 1 - i;
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, color[invI]);
			}
		}
	}

	this.drawCircleNoteGrad = function () {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invO = orientation ? 0 : 1;
		let color = [[225, 0, 200, 1], [0, 0, 100, 1]];
		let colorTemp;
		let diff = [(color[orientation][0] - color[invO][0]) / div, (color[orientation][1] - color[invO][1]) / div, (color[orientation][2] - color[invO][2]) / div];
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				colorTemp = "rgba(";
				for (let j = 0; j < 3; j++) {
					colorTemp += color[orientation][j] + ',';
				}
				colorTemp += "1)";
				for (let i = 0; i < div - 1; i++) {
					let perc = (i + 1) / div;
					if ((x * x) + (y * y) <= (z * z) * (perc)) {
						colorTemp = "rgba(";
						for (let j = 0; j < 3; j++) {
							colorTemp += color[invO][j] + ((i + 1) * diff[j]) + ',';
						}
						colorTemp += "1)";
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, colorTemp);
			}
		}
	}

	this.drawCrossNote = function () {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invI;
		let color = this.colors;
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				invI = orientation ? div - 1 : 0;
				for (let i = 0; i < div - 1; i++) {
					let perc = z - ((i + 1) / div) * z;
					if ((x <= -perc || x >= perc) && (y <= -perc || y >= perc)) {
						invI = orientation ? i : div - 1 - i;
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, color[invI]);
			}
		}
	}

	this.drawCrossNoteGrad = function () {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invO = orientation ? 0 : 1;
		let color = [[225, 0, 200, 1], [0, 0, 100, 1]];
		let colorTemp;
		let diff = [(color[orientation][0] - color[invO][0]) / div, (color[orientation][1] - color[invO][1]) / div, (color[orientation][2] - color[invO][2]) / div];
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				colorTemp = "rgba(";
				for (let j = 0; j < 3; j++) {
					colorTemp += color[orientation][j] + ',';
				}
				colorTemp += "1)";
				for (let i = 0; i < div - 1; i++) {
					let perc = z - ((i + 1) / div) * z;
					if ((x <= -perc || x >= perc) && (y <= -perc || y >= perc)) {
						colorTemp = "rgba(";
						for (let j = 0; j < 3; j++) {
							colorTemp += color[invO][j] + ((i + 1) * diff[j]) + ',';
						}
						colorTemp += "1)";
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, colorTemp);
			}
		}
	}

	this.drawSquareNote = function () {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invI;
		let color = this.colors;
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				invI = orientation ? div - 1 : 0;
				for (let i = 0; i < div - 1; i++) {
					let perc = ((i + 1) / div) * z;
					if ((x >= -perc && x <= perc) && (y >= -perc && y <= perc)) {
						invI = orientation ? i : div - 1 - i;
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, color[invI]);
			}
		}
	}

	this.drawSquareNoteGrad = function () {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invO = orientation ? 0 : 1;
		let color = [[225, 0, 200, 1], [0, 0, 100, 1]];
		let colorTemp;
		let diff = [(color[orientation][0] - color[invO][0]) / div, (color[orientation][1] - color[invO][1]) / div, (color[orientation][2] - color[invO][2]) / div];
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				colorTemp = "rgba(";
				for (let j = 0; j < 3; j++) {
					colorTemp += color[orientation][j] + ',';
				}
				colorTemp += "1)";
				for (let i = 0; i < div - 1; i++) {
					let perc = ((i + 1) / div) * z;
					if ((x >= -perc && x <= perc) && (y >= -perc && y <= perc)) {
						colorTemp = "rgba(";
						for (let j = 0; j < 3; j++) {
							colorTemp += color[invO][j] + ((i + 1) * diff[j]) + ',';
						}
						colorTemp += "1)";
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, colorTemp);
			}
		}
	}

	function horizontal(x, y, perc) {
		return ((x >= -perc || x <= perc) && (y <= -perc || y >= perc));
	}

	function vertical(x, y, perc) {
		return ((x <= -perc || x >= perc) && (y >= -perc || y <= perc))
	}

	this.drawLineNote = function (callback) {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invI;
		let color = this.colors;
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				invI = orientation ? div - 1 : 0;
				for (let i = 0; i < div - 1; i++) {
					let perc = z - ((i + 1) / div) * z;
					if (callback(x, y, perc)) {
						invI = orientation ? i : div - 1 - i;
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, color[invI]);
			}
		}
	}

	this.drawLineNoteGrad = function (callback) {
		let z = this.z;
		let rz = this.rz;
		let orientation = store.state.patterns.orientation;
		let div = store.state.patterns.div;
		let invO = orientation ? 0 : 1;
		let color = [[225, 0, 200, 1], [0, 0, 100, 1]];
		let colorTemp;
		let diff = [(color[orientation][0] - color[invO][0]) / div, (color[orientation][1] - color[invO][1]) / div, (color[orientation][2] - color[invO][2]) / div];
		for (let y = -rz; y <= rz; y++) {
			for (let x = -rz; x <= rz; x++) {
				colorTemp = "rgba(";
				for (let j = 0; j < 3; j++) {
					colorTemp += color[orientation][j] + ',';
				}
				colorTemp += "1)";
				for (let i = 0; i < div - 1; i++) {
					let perc = z - ((i + 1) / div) * z;
					if (callback(x, y, perc)) {
						colorTemp = "rgba(";
						for (let j = 0; j < 3; j++) {
							colorTemp += color[invO][j] + ((i + 1) * diff[j]) + ',';
						}
						colorTemp += "1)";
						break;
					}
				}
				this.drawPixel(x + rz, y + rz, colorTemp);
			}
		}
	}

	this.drawLineHNote = function () {
		this.drawLineNote(horizontal);
	}

	this.drawLineVNote = function () {
		this.drawLineNote(vertical);
	}

	this.drawLineHNoteGrad = function () {
		this.drawLineNoteGrad(horizontal);
	}

	this.drawLineVNoteGrad = function () {
		this.drawLineNoteGrad(vertical);
	}

	this.drawPixel = function (x, y, color) {
		let pixel = store.state.patterns.pixel;
		this.context.fillStyle = color;
		// console.log(pixel)
		switch (pixel) {
			case "Circle":
				this.context.beginPath();
				this.context.arc(x * this.divWidth + this.divWidth / 2, y * this.divHeight + this.divHeight / 2, this.divWidth / 2, 0, 2 * Math.PI, false);
				this.context.fill();
				break;
			case "Square":
				this.context.fillRect(x * this.divWidth, y * this.divHeight, this.divWidth, this.divHeight);
				break;
		}

	}
}