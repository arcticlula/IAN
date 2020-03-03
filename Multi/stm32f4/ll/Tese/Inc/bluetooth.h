#ifndef __bluetooth_H
#define __bluetooth_H

// #define MIDI_OFF 0x10
#define ACK 0x01
#define NACK 0x02
#define SETTINGS_BRIGHT 0x11
#define MODE_BACK_STATE 0x20
#define MODE_BACK_CIRCLE 0x21
#define MODE_BACK_CIRCLE_GRAD 0x22
#define MODE_BACK_CROSS 0x25
#define MODE_BACK_CROSS_GRAD 0x26
#define MODE_BACK_SQUARE 0x30
#define MODE_BACK_SQUARE_GRAD 0x31
#define MODE_BACK_LINES_H 0x35
#define MODE_BACK_LINES_H_GRAD 0x36
#define MODE_BACK_LINES_V 0x37
#define MODE_BACK_LINES_V_GRAD 0x38
#define MODE_BACK_TRIANGLE 0x40
#define MODE_BACK_TRIANGLE_GRAD 0x41
#define MODE_BACK_COLORS 0x50
#define MODE_BACK_COLORS_OG 0x51
#define MODE_BACK_COLORS_BW 0x52
#define MODE_TEXT_STATE 0x60
#define MODE_CUSTOM 0x70

void startReception(void);

#endif /* __bluetooth_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
