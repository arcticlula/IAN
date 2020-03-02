#include "stdint.h"
#include "definitions.h"
#include "ws2812.h"
#include "main.h"

volatile uint8_t WS2812_TC = 1;
uint16_t WS2812_IO_High = 0xFFFF;
uint16_t WS2812_IO_Low = 0x0000;

void WS2812_sendbuf(uint32_t buffersize)
{
  // transmission complete flag, indicate that transmission is taking place
  WS2812_TC = 0;

  // clear all relevant DMA flags
  LL_DMA_ClearFlag_TE5(DMA2);
  LL_DMA_ClearFlag_TE1(DMA2);
  LL_DMA_ClearFlag_TE2(DMA2);
  LL_DMA_ClearFlag_TC5(DMA2);
  LL_DMA_ClearFlag_TC1(DMA2);
  LL_DMA_ClearFlag_TC2(DMA2);

  // configure the number of bytes to be transferred by the DMA controller
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_5, buffersize);
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, buffersize);
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, buffersize);

  // clear all TIM1 flags
  TIM1->SR = 0;

  // enable the corresponding DMA streams
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_5);
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);

  // IMPORTANT: enable the TIM1 DMA requests AFTER enabling the DMA streams!
  LL_TIM_EnableDMAReq_CC1(TIM1);
  LL_TIM_EnableDMAReq_CC2(TIM1);
  LL_TIM_EnableDMAReq_UPDATE(TIM1);

  // preload counter with 104 so TIM1 generates UEV directly to start DMA transfer
  LL_TIM_SetCounter(TIM1, 104);

  // start TIM1
  LL_TIM_EnableCounter(TIM1);
}

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
