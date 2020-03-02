#ifndef __WS2812_H
#define __WS2812_H

#define WS2812_DEADPERIOD 40

extern uint16_t WS2812_IO_High;
extern uint16_t WS2812_IO_Low;

extern volatile uint8_t WS2812_TC;
uint16_t WS2812_IO_framedata[24 * NLEDSCH];

void WS2812_framedata_setPixel(uint8_t row, uint16_t column, uint8_t red, uint8_t green, uint8_t blue);
void WS2812_framedata_setRow(uint8_t row, uint16_t columns, uint8_t red, uint8_t green, uint8_t blue);
void WS2812_sendbuf(uint32_t buffersize);

#endif