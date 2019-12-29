/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"

#define WS2812_DEADPERIOD 19
#define NCHANNELS 4
#define NCOLS 1
#define NLINESCH 1
#define NLEDSCH NCOLS *NLINESCH
#define NCOLSL NCOLS - 1
#define NLEDS NCOLS *NCOLS
#define MAX_DIV 5

uint16_t WS2812_IO_High = 0xFFFF;
uint16_t WS2812_IO_Low = 0x0000;

volatile uint8_t WS2812_TC = 1;
volatile uint8_t TIM1_overflows = 0;

/* WS2812 framebuffer
 * buffersize = (#LEDs / 16) * 24 */
uint16_t WS2812_IO_framedata[24 * NLEDSCH];

/* Array defining 12 color triplets to be displayed */
uint8_t colors[12][3] =
    {
        {0x80, 0x00, 0x00},
        {0x80, 0x40, 0x00},
        {0x80, 0x80, 0x00},
        {0x40, 0x80, 0x00},
        {0x00, 0x80, 0x00},
        {0x00, 0x80, 0x40},
        {0x00, 0x80, 0x80},
        {0x00, 0x40, 0x80},
        {0x00, 0x00, 0x80},
        {0x40, 0x00, 0x80},
        {0x80, 0x00, 0x80},
        {0x80, 0x00, 0x40}};

uint8_t currColor[3] = {60, 25, 0};
uint8_t effect[NLEDS][3];

void Delay(__IO uint32_t nCount)
{
  while (nCount--)
    ;
}

/* DMA2 Channel6 Interrupt Handler gets executed once the complete framebuffer has been transmitted to the LEDs */
void DMA2_Stream2_IRQHandler(void)
{
  // clear DMA2_Stream2 transfer complete interrupt flag
  DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
  // enable TIM1 Update interrupt to append 50us dead period
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  // disable the DMA channels
  DMA_Cmd(DMA2_Stream5, DISABLE);
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_Cmd(DMA2_Stream2, DISABLE);
  // IMPORTANT: disable the DMA requests, too!
  TIM_DMACmd(TIM1, TIM_DMA_CC1, DISABLE);
  TIM_DMACmd(TIM1, TIM_DMA_CC2, DISABLE);
  TIM_DMACmd(TIM1, TIM_DMA_Update, DISABLE);
}

/* TIM1 Interrupt Handler gets executed on every TIM1 Update if enabled */
void TIM1_IRQHandler(void)
{
  // Clear TIM1 Interrupt Flag
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

  /* check if certain number of overflows has occured yet 
	 * this ISR is used to guarantee a 50us dead time on the data lines
	 * before another frame is transmitted */
  if (TIM1_overflows < (uint8_t)WS2812_DEADPERIOD)
  {
    // count the number of occured overflows
    TIM1_overflows++;
  }
  else
  {
    // clear the number of overflows
    TIM1_overflows = 0;
    // stop TIM1 now because dead period has been reached
    TIM_Cmd(TIM1, DISABLE);
    /* disable the TIM1 Update interrupt again 
		 * so it doesn't occur while transmitting data */
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
    // finally indicate that the data frame has been transmitted
    WS2812_TC = 1;
  }
}

void GPIO_init(void)
{
  GPIO_InitTypeDef GPIO_InitDef;
  // GPIOA Periph clock enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  // GPIOA pins WS2812 data outputs
  GPIO_InitDef.GPIO_Pin = 0xFFFF;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitDef);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitDef.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitDef);
}

void DMA2_init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  // TIM1 Update event
  /* DMA2 Channel2 configuration ----------------------------------------------*/
  DMA_DeInit(DMA2_Stream5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOA->ODR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&WS2812_IO_High;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_Channel = DMA_Channel_6;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);

  // TIM1 CC1 event
  /* DMA2 Channel5 configuration ----------------------------------------------*/
  DMA_DeInit(DMA2_Stream1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOA->ODR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&WS2812_IO_framedata;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_Channel = DMA_Channel_6;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);

  // TIM1 CC2 event
  /* DMA2 Channel6 configuration ----------------------------------------------*/
  DMA_DeInit(DMA2_Stream2);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOA->ODR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&WS2812_IO_Low;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_Channel = DMA_Channel_6;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);

  /* configure DMA2 Channel6 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* enable DMA2 Channel6 transfer complete interrupt */
  DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
}

uint32_t TimPeriod;
uint32_t TimerResetPulsePeriod;

void TIM1_init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  // uint16_t PrescalerValue = (uint16_t)(SystemCoreClock / 24000000) - 1;
  TimPeriod = SystemCoreClock / 800000;                   // 0,125us period (10 times lower the 1,25us period to have fixed math below)
  TimerResetPulsePeriod = (SystemCoreClock / (320 * 60)); // 60us just to be sure

  uint32_t cc1 = (10 * TimPeriod) / 36;
  uint32_t cc2 = (10 * TimPeriod) / 15;

  // TIM1 Periph clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_Period = TimPeriod; // 800kHz
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_ARRPreloadConfig(TIM1, DISABLE);

  /* Timing Mode configuration: Channel 1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = cc1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* Timing Mode configuration: Channel 2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = cc2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* configure TIM1 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

void WS2812_sendbuf(uint32_t buffersize)
{
  // transmission complete flag, indicate that transmission is taking place
  WS2812_TC = 0;

  // clear all relevant DMA flags
  DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TEIF5);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TEIF1);
  DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_HTIF2 | DMA_FLAG_TEIF2);

  // configure the number of bytes to be transferred by the DMA controller
  DMA_SetCurrDataCounter(DMA2_Stream5, buffersize);
  DMA_SetCurrDataCounter(DMA2_Stream1, buffersize);
  DMA_SetCurrDataCounter(DMA2_Stream2, buffersize);

  // clear all TIM1 flags
  TIM1->SR = 0;

  // enable the corresponding DMA channels
  DMA_Cmd(DMA2_Stream5, ENABLE);
  DMA_Cmd(DMA2_Stream1, ENABLE);
  DMA_Cmd(DMA2_Stream2, ENABLE);

  // IMPORTANT: enable the TIM1 DMA requests AFTER enabling the DMA channels!
  TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
  TIM_DMACmd(TIM1, TIM_DMA_CC2, ENABLE);
  TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);

  // preload counter with 29 so TIM1 generates UEV directly to start DMA transfer
  TIM_SetCounter(TIM1, TimPeriod);

  // start TIM1
  TIM_Cmd(TIM1, ENABLE);
}

int main(void)
{
  uint8_t i;
  // RCC_GetClocksFreq(&RCC_Clocks);
  // SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  SystemInit();
  GPIO_init();
  DMA2_init();
  TIM1_init();
  // WS2812_framedata_setPixel(0, 0, colors[0][0], colors[0][1], colors[0][2]);
  // WS2812_framedata_setPixel(1, 0, colors[1][0], colors[1][1], colors[1][2]);
  // WS2812_framedata_setPixel(2, 0, colors[2][0], colors[2][1], colors[2][2]);
  // WS2812_framedata_setPixel(3, 0, colors[3][0], colors[3][1], colors[3][2]);
  // WS2812_sendbuf(24 * NLEDSCH);

  while (1)
  {
    GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
    Delay(5000000L);
    for (i = 0; i < 12; i++)
    {
      //   // wait until the last frame was transmitted
      //   while (!WS2812_TC)
      //     ;
      WS2812_framedata_setPixel(0, 0, colors[i][0], colors[i][1], colors[i][2]);
      WS2812_framedata_setPixel(1, 0, colors[i][0], colors[i][1], colors[i][2]);
      WS2812_framedata_setPixel(2, 0, colors[i][0], colors[i][1], colors[i][2]);
      WS2812_framedata_setPixel(3, 0, colors[i][0], colors[i][1], colors[i][2]);
      WS2812_sendbuf(24 * NLEDSCH);
      //   // wait some amount of time
      Delay(2000000L);
    }
  }
}
