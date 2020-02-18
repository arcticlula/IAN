/* HDLC Asynchronous framing */
/* The frame boundary octet is 01111110, (7E in hexadecimal notation) */
const FRAME_BOUNDARY_OCTET = 0x7E

/* A "control escape octet", has the bit sequence '01111101', (7D hexadecimal) */
const CONTROL_ESCAPE_OCTET = 0x7D

/* If either of these two octets appears in the transmitted data, an escape octet is sent, */
/* followed by the original data octet with bit 5 inverted */
const INVERT_OCTET = 0x20

/* The frame check sequence (FCS) is a 16-bit CRC-CCITT */
/* AVR Libc CRC function is _crc_ccitt_update() */
/* Corresponding CRC function in Qt (www.qt.io) is qChecksum() */
const CRC16_CCITT_INIT_VAL = 0x0000FFFF

/* 16bit low and high bytes copier */
function low(x) { return (x & 0x000000FF) }
function high(x) { return (((x) >> 8) & 0x000000FF) }

function lo8(x) { return (x & 0x000000ff) }
function hi8(x) { return (x >> 8) }

const MINIHDLC_MAX_FRAME_LENGTH = 64;

export default function hdlc() {
	this.mhst =
	{
		sendchar_function: "",
		frame_handler_type: "",
		escape_character: "",
		frame_position: "",
		frame_checksum: "",
		receive_frame_buffer: []
	}
	let frame_buffer = new Uint8Array(MINIHDLC_MAX_FRAME_LENGTH + 1);
	let frame_buffer_size = new Uint32Array([0]);

	/*
 Polynomial: x^16 + x^12 + x^5 + 1 (0x8408) Initial value: 0xffff
 This is the CRC used by PPP and IrDA.
 See RFC1171 (PPP protocol) and IrDA IrLAP 1.1
 */
	this._crc_ccitt_update = function (crc, data) {
		data ^= lo8(crc);
		data ^= (data << 4) & 0xFF;
		return ((data << 8) | hi8(crc)) ^ (data >> 4) ^ (data << 3);
	}

	this.minihdlc_init = function (sendchar_function, frame_hander_function) {
		this.mhst.sendchar_function = sendchar_function;
		this.mhst.frame_handler = frame_hander_function;
		this.mhst.frame_position = 0;
		this.mhst.frame_checksum = CRC16_CCITT_INIT_VAL;
		this.mhst.escape_character = false;
	}

	/* Function to send a byte throug USART, I2C, SPI etc.*/
	this.minihdlc_sendchar = function (data) {
		if (this.mhst.sendchar_function) {
			this.mhst.sendchar_function(data);
		}
	}

	/* Function to find valid HDLC frame from incoming data */
	this.minihdlc_char_receiver = function (data) {
		/* FRAME FLAG */
		let hex = Number(data).toString(16);
		if (hex.length < 2) {
			hex = "0" + hex;
		}
		console.log(data, hex)
		if (data == FRAME_BOUNDARY_OCTET) {
			if (this.mhst.escape_character == true) {
				this.mhst.escape_character = false;
			}
			/* If a valid frame is detected */
			else if ((this.mhst.frame_position >= 2) && (this.mhst.frame_checksum == ((this.mhst.receive_frame_buffer[this.mhst.frame_position - 1]
				<< 8) |
				(this.mhst.receive_frame_buffer[this.mhst.frame_position - 2] & 0xff)))) // (msb << 8 ) | (lsb & 0xff)
			{
				/* Call the user defined function and pass frame to it */
				this.mhst.frame_handler(this.mhst.receive_frame_buffer,
					this.mhst.frame_position - 2);
			}
			this.mhst.frame_position = 0;
			this.mhst.frame_checksum = CRC16_CCITT_INIT_VAL;
			return;
		}

		if (this.mhst.escape_character) {
			this.mhst.escape_character = false;
			data ^= INVERT_OCTET;
		}
		else if (data == CONTROL_ESCAPE_OCTET) {
			this.mhst.escape_character = true;
			return;
		}

		this.mhst.receive_frame_buffer[this.mhst.frame_position] = data;

		if (this.mhst.frame_position - 2 >= 0) {
			this.mhst.frame_checksum = this._crc_ccitt_update(this.mhst.frame_checksum,
				this.mhst.receive_frame_buffer[this.mhst.frame_position - 2]);
		}

		this.mhst.frame_position++;

		if (this.mhst.frame_position == MINIHDLC_MAX_FRAME_LENGTH) {
			this.mhst.frame_position = 0;
			this.mhst.frame_checksum = CRC16_CCITT_INIT_VAL;
		}
	}

	/* Wrap given data in HDLC frame and send it out byte at a time*/
	this.minihdlc_send_frame = function (frame_buffer, frame_length) {
		let i = 0;
		let data;
		let fcs = CRC16_CCITT_INIT_VAL;

		this.minihdlc_sendchar(FRAME_BOUNDARY_OCTET);
		while (frame_length) {
			data = frame_buffer[i++];
			fcs = this._crc_ccitt_update(fcs, data);
			if ((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET)) {
				this.minihdlc_sendchar(CONTROL_ESCAPE_OCTET);
				data ^= INVERT_OCTET;
			}
			this.minihdlc_sendchar(data);
			frame_length--;
		}
		data = low(fcs);
		if ((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET)) {
			this.minihdlc_sendchar(CONTROL_ESCAPE_OCTET);
			data ^= INVERT_OCTET;
		}
		this.minihdlc_sendchar(data);
		data = high(fcs);
		if ((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET)) {
			this.minihdlc_sendchar(CONTROL_ESCAPE_OCTET);
			data ^= INVERT_OCTET;
		}
		this.minihdlc_sendchar(data);
		this.minihdlc_sendchar(FRAME_BOUNDARY_OCTET);
	}

	/* Wrap given data in HDLC frame and send it to static buffer*/

	this.buffer_init = function () {
		frame_buffer_size = 0;
	}

	this.buffer_push = function (data) {
		if (frame_buffer_size >= MINIHDLC_MAX_FRAME_LENGTH) {
			return;
		}
		frame_buffer[frame_buffer_size] = data;
		frame_buffer_size++;
	}

	this.minihdlc_send_frame_to_buffer = function (frame_buffer, frame_length) {
		this.mhst.sendchar_function = this.buffer_push;
		this.buffer_init();
		this.minihdlc_send_frame(frame_buffer, frame_length);
	}

	this.minihdlc_get_buffer = function () {
		return frame_buffer;
	}

	this.minihdlc_get_buffer_size = function () {
		return frame_buffer_size;
	}
}