#include "stdint.h"
#include "definitions.h"
#include "ws2812.h"

volatile uint8_t WS2812_TC = 1;
uint16_t WS2812_IO_High = 0xFFFF;
uint16_t WS2812_IO_Low = 0x0000;

void WS2812_framedata_setPixel(uint8_t row, uint16_t column, uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    // clear the data for pixel
    WS2812_IO_framedata[((column * 24) + i)] &= ~(0x01 << row);
    WS2812_IO_framedata[((column * 24) + 8 + i)] &= ~(0x01 << row);
    WS2812_IO_framedata[((column * 24) + 16 + i)] &= ~(0x01 << row);
    // write new data for pixel
    WS2812_IO_framedata[((column * 24) + i)] |= ((((green << i) & 0x80) >> 7) << row);
    WS2812_IO_framedata[((column * 24) + 8 + i)] |= ((((red << i) & 0x80) >> 7) << row);
    WS2812_IO_framedata[((column * 24) + 16 + i)] |= ((((blue << i) & 0x80) >> 7) << row);
  }
}

void WS2812_framedata_setRow(uint8_t row, uint16_t columns, uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  for (i = 0; i < columns; i++)
  {
    WS2812_framedata_setPixel(row, i, red, green, blue);
  }
}
