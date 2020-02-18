/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "definitions.h"
#include "ws2812.h"
#include "notes.h"
#include "draw.h"
#include "minihdlc-master/minihdlc.h"
/* this define sets the number of TIM2 overflows
 * to append to the data frame for the LEDs to 
 * load the received data into their registers */

volatile uint8_t TIM2_overflows = 0;
drawshape_type drawshape_function;
drawshape_type drawtext_function;

/* WS2812 framebuffer
 * buffersize = (#LEDs / 16) * 24 */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
#if USART_MIDI
static void MX_USART3_UART_Init(void);
#endif
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);

void WS2812_sendbuf(uint32_t buffersize)
{
  // transmission complete flag, indicate that transmission is taking place
  WS2812_TC = 0;

  // clear all relevant DMA flags
  LL_DMA_ClearFlag_TE2(DMA1);
  LL_DMA_ClearFlag_TE5(DMA1);
  LL_DMA_ClearFlag_TE7(DMA1);

  // configure the number of bytes to be transferred by the DMA controller
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, buffersize);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, buffersize);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, buffersize);

  // clear all TIM2 flags
  TIM2->SR = 0;

  // enable the corresponding DMA channels
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

  // IMPORTANT: enable the TIM2 DMA requests AFTER enabling the DMA channels!
  LL_TIM_EnableDMAReq_CC1(TIM2);
  LL_TIM_EnableDMAReq_CC2(TIM2);
  LL_TIM_EnableDMAReq_UPDATE(TIM2);

  // preload counter with 29 so TIM2 generates UEV directly to start DMA transfer
  LL_TIM_SetCounter(TIM2, 29);

  // start TIM2
  LL_TIM_EnableCounter(TIM2);
}

void sendDataBluetooth(uint8_t data)
{
  while (!LL_USART_IsActiveFlag_TXE(USART1))
  {
  }
  LL_USART_TransmitData8(USART1, data);
}

#define MIDI_OFF 0x10
#define MODE_BACK 0x20
#define MODE_BACK_CIRCLE 0x21
#define MODE_BACK_CIRCLE_GRAD 0x22
#define MODE_BACK_CROSS 0x25
#define MODE_BACK_CROSS_GRAD 0x26
#define MODE_BACK_SQUARE 0x30
#define MODE_BACK_SQUARE_GRAD 0x31
#define MODE_BACK_LINES_H 0x35;
#define MODE_BACK_LINES_H_GRAD 0x36;
#define MODE_BACK_LINES_V 0x37;
#define MODE_BACK_LINES_V_GRAD 0x38;

// #define MODE_BACK_TRIANGLE 0x35
// #define MODE_BACK_TRIANGLE_GRAD 0x36
#define MODE_TEXT 0x06

static inline void drawShape()
{
  if (drawshape_function)
  {
    (*drawshape_function)();
  }
}

static inline void drawText(uint8_t data)
{
  if (drawtext_function)
  {
    (*drawtext_function)(data);
  }
}

void frame_handler_function(const uint8_t *frame_buffer, uint16_t frame_length)
{
  uint8_t data;
  data = *frame_buffer++;
  switch (data)
  {
  case MODE_BACK_CIRCLE:
    drawshape_function = drawCircleNote;
    break;
  case MODE_BACK_CIRCLE_GRAD:
    drawshape_function = drawCircleNoteGrad;
    break;
  case MODE_BACK_CROSS:
    drawshape_function = drawCrossNote;
    break;
  case MODE_BACK_CROSS_GRAD:
    drawshape_function = drawCircleNoteGrad;
    break;
  case MODE_BACK_SQUARE:
    drawshape_function = drawCrossNote;
    break;
  case MODE_BACK_SQUARE_GRAD:
    drawshape_function = drawCircleNoteGrad;
    break;
  case MODE_BACK_LINES_H:
    drawshape_function = drawLineNote;
    callback_function = horizontal;
    break;
  case MODE_BACK_LINES_H_GRAD:
    drawshape_function = drawLineNoteGrad;
    callback_function = horizontal;
    break;
  case MODE_BACK_LINES_V:
    drawshape_function = drawLineNote;
    callback_function = vertical;
    break;
  case MODE_BACK_LINES_V_GRAD:
    drawshape_function = drawLineNoteGrad;
    callback_function = vertical;
    break;
  default:
    break;
  }
  data = *frame_buffer++;
  MAX_DIV = data;
  data = *frame_buffer++;
  orientation = data
  // while (frame_length)
  // {
  //   sendDataBluetooth(data);
  //   frame_length--;
  //   data = *frame_buffer++;
  // }
  // minihdlc_send_frame(frame_buffer, frame_length);
}

void startReception(void)
{
  minihdlc_init(sendDataBluetooth, frame_handler_function);
  /* Clear Overrun flag, in case characters have already been sent to USART */
  LL_USART_ClearFlag_ORE(USART1);

  /* Enable RXNE and Error interrupts */
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_ERROR(USART1);
  uint8_t buffer[] = "OLA amigo!";

  minihdlc_send_frame(buffer, ARRAY_LEN(buffer));
}

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled 
  */
  LL_GPIO_AF_DisableRemap_SWJ();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
#if USART_MIDI
  MX_USART3_UART_Init();
#endif
  MX_DMA_Init();
  MX_TIM2_Init();

  // drawLetter('C', 0, 1);
  // drawLetter('5', 4, 1);

  startReception();

  clearNoteBuffer();
  clearBack();
  clearText();
  clearFramebuffer();
  // uint8_t blue[3] = {1, 1, 3};
  // uint8_t red[3] = {5, 0, 1};

  setNote(24);
  setNote(15);
  setNote(87);

  // setBackColor(blue);
  // fillFramebuffer();
  // WS2812_sendbuf(24 * NLEDSCH);

  // WS2812_sendbuf(24 * NLEDSCH);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // setTextColor(red);
  // drawString("ola", 1, 1, 1, 0);
  // drawScrollingString("OLA meninos", 500, 1, 1, 0);
  // fillFramebuffer();
  // WS2812_sendbuf(24 * NLEDSCH);
  // drawLine(0, 0);
  // drawPixel(1, 1);

  while (1)
  {
    // set two pixels (columns) in the defined row (channel 0) to the
    // color values defined in the colors array
    //   // wait until the last frame was transmitted
    //   while (!WS2812_TC)
    //     ;
    //   //   // move(1, 1);
    //   //   // fillFramebuffer();
    //   //   // WS2812_sendbuf(24 * NLEDSCH);
    //   //   // LL_mDelay(500);
    //   //   // }
    //   //   // drawString("Ordem dos Engenheiros Tecnicos", 1, 1, 1, 0);
    //   //   // rotate()
    //   //   // drawColor(arrCol[i]);
    //   //   // fillFramebuffer();
    //   //   // WS2812_sendbuf(24 * NLEDSCH);
    setNote(28);
    drawShape();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    LL_mDelay(2000);
    setNote(58);
    drawShape();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    LL_mDelay(2000);
    setNote(17);
    drawShape();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    LL_mDelay(2000);
    setNote(83);
    drawShape();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    LL_mDelay(2000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
    Error_Handler();
  }
  LL_RCC_HSE_Enable();

  /* Wait till HSE is ready */
  while (LL_RCC_HSE_IsReady() != 1)
  {
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

  /* Wait till PLL is ready */
  while (LL_RCC_PLL_IsReady() != 1)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  /* Wait till System clock is ready */
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }
  LL_Init1msTick(72000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(72000000);
}

LL_TIM_InitTypeDef TIM_InitStruct = {0};
LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
LL_DMA_InitTypeDef DMA_InitStruct = {0};
LL_USART_InitTypeDef USART_InitStruct = {0};

static void MX_TIM2_Init(void)
{
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 DMA Init */
  LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_2);
  DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_High;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.NbData = 0;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_VERYHIGH;
  LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &DMA_InitStruct);

  LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_5);
  DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_framedata;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.NbData = 0;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_VERYHIGH;
  LL_DMA_Init(DMA1, LL_DMA_CHANNEL_5, &DMA_InitStruct);

  /* TIM2_CH2_CH4 Init */
  LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_7);
  DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_Low;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.NbData = 0;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_VERYHIGH;
  LL_DMA_Init(DMA1, LL_DMA_CHANNEL_7, &DMA_InitStruct);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);

  uint16_t PrescalerValue;

  PrescalerValue = (uint16_t)(SystemCoreClock / 24000000) - 1;
  /* Time base configuration */
  TIM_InitStruct.Autoreload = 29; // 800kHz
  TIM_InitStruct.Prescaler = PrescalerValue;
  TIM_InitStruct.ClockDivision = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);

  /* Timing Mode configuration: Channel 1 */
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_FROZEN;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 8;
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisablePreload(TIM2, LL_TIM_CHANNEL_CH1);

  /* Timing Mode configuration: Channel 2 */
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 17;
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
  LL_TIM_OC_DisablePreload(TIM2, LL_TIM_CHANNEL_CH2);
}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  // NVIC_SetPriority(DMA1_Channel2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  // /* DMA1_Channel5_IRQn interrupt configuration */
  // NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  // GPIOA Periph clock enable

  // GPIOA pins WS2812 data outputs
  GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

static void MX_USART1_UART_Init(void)
{

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  /**USART1 GPIO Configuration  
  PB6   ------> USART1_TX
  PB7   ------> USART1_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  LL_GPIO_AF_EnableRemap_USART1();
  // __HAL_AFIO_REMAP_USART1_ENABLE();

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 9600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */
  LL_USART_EnableIT_RXNE(USART1);
  /* USER CODE END USART1_Init 2 */
}

#if USART_MIDI
static void MX_USART3_UART_Init(void)
{

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

  /**USART3 GPIO Configuration
  PB10   ------> USART3_TX
  PB11   ------> USART3_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate = 31250;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */
  LL_USART_EnableIT_RXNE(USART3);
  /* USER CODE END USART3_Init 2 */
}
#endif

void DMA1_Channel7_IRQHandler(void)
{
  LL_DMA_ClearFlag_TC7(DMA1);
  // enable TIM2 Update interrupt to append 50us dead period
  LL_TIM_EnableIT_UPDATE(TIM2);
  // disable the DMA channels
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
  // IMPORTANT: disable the DMA requests, too!
  LL_TIM_DisableDMAReq_CC1(TIM2);
  LL_TIM_DisableDMAReq_CC2(TIM2);
  LL_TIM_DisableDMAReq_UPDATE(TIM2);
}

void TIM2_IRQHandler(void)
{
  // Clear TIM2 Interrupt Flag
  LL_TIM_ClearFlag_UPDATE(TIM2);

  /* check if certain number of overflows has occured yet 
	 * this ISR is used to guarantee a 50us dead time on the data lines
	 * before another frame is transmitted */
  if (TIM2_overflows < (uint8_t)WS2812_DEADPERIOD)
  {
    // count the number of occured overflows
    TIM2_overflows++;
  }
  else
  {
    // clear the number of overflows
    TIM2_overflows = 0;
    // stop TIM2 now because dead period has been reached
    LL_TIM_DisableCounter(TIM2);
    /* disable the TIM2 Update interrupt again 
		 * so it doesn't occur while transmitting data */
    LL_TIM_DisableIT_UPDATE(TIM2);
    // finally indicate that the data frame has been transmitted
    WS2812_TC = 1;
  }
}

void USART1_IRQHandler(void)
{
  uint8_t RxBuffer;
  if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    RxBuffer = LL_USART_ReceiveData8(USART1);
    minihdlc_char_receiver(RxBuffer);
    sendDataBluetooth(RxBuffer);
  }
}

#if USART_MIDI
void USART3_IRQHandler(void)
{
  static uint8_t usart_rx_buffer;
  static uint8_t usart_rx_message[3];
  char str[100];
  long temp;
  static uint8_t sizeMsg;
  // volatile uint8_t colorLetter[] = {0, 0, 0};
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  if (LL_USART_IsActiveFlag_RXNE(USART3) && LL_USART_IsEnabledIT_RXNE(USART3))
  {
    usart_rx_buffer = LL_USART_ReceiveData8(USART3);
    if (cntMessage == 0 && waitForMessage == 0)
    {
      switch (usart_rx_buffer)
      {
      case NOTE_ON: //NOTE_ON Channel 1
        sizeMsg = 3;
        cntMessage = 3;
        waitForMessage = 1;
        break;

      case NOTE_OFF: //NOTE_OFF Channel 1
        sizeMsg = 3;
        cntMessage = 3;
        waitForMessage = 1;
        break;

      case CTRL_CHANGE: //CTRL_CHANGE Channel 1
        sizeMsg = 3;
        cntMessage = 3;
        waitForMessage = 1;
        break;
      }
    }
    if (cntMessage > 1)
    {
      usart_rx_message[sizeMsg - cntMessage] = usart_rx_buffer;
      cntMessage--;
    }
    else if (waitForMessage)
    {
      usart_rx_message[sizeMsg - cntMessage] = usart_rx_buffer;
      switch (usart_rx_message[0])
      {
      case NOTE_ON: //NOTE_ON Channel 1
        for (int i = 0; i < ARRAY_LEN(usart_rx_message); i++)
        {
          sprintf(&str[i * 4], "%03x ", usart_rx_message[i]);
          temp = strtol(&str[i * 4], NULL, 16);
          if (i != 0)
            sprintf(&str[i * 4], "%03ld ", temp);
          if (i == 1)
          {
            setNote(temp);
          }
          if (i == 2)
            setVelocity(temp);
        }

        // calculateLeds();
        // clearText();
        // drawNote();
        // drawStringArray(1, 1, 1, 1);
        drawShape();
        fillFramebuffer();
        WS2812_sendbuf(24 * NLEDSCH);
        break;

      case NOTE_OFF: //NOTE_OFF Channel 1
        for (int i = 0; i < ARRAY_LEN(usart_rx_message); i++)
        {
          sprintf(&str[i * 4], "%03x ", usart_rx_message[i]);
          temp = strtol(&str[i * 4], NULL, 16);
          if (i != 0)
            sprintf(&str[i * 4], "%03ld ", temp);
          if (i == 1)
            // setNote(temp);
            if (i == 2)
              setVelocity(temp);
        }

        break;

      case CTRL_CHANGE: //Velocity Channel 1
        for (int i = 0; i < ARRAY_LEN(usart_rx_message); i++)
        {
          sprintf(&str[i * 4], "%03x ", usart_rx_message[i]);
          temp = strtol(&str[i * 4], NULL, 16);
          if (i != 0)
            sprintf(&str[i * 4], "%03ld ", temp);
          if (i == 2)
            setVelocity(temp);
        }
        if (WS2812_TC)
        {
          //calculateLeds();
          //fillFramebufferColor(arr);
          //WS2812_sendbuf(24 * NLEDSCHCH);
        }
        break;
      }
      waitForMessage = 0;
      cntMessage = 0;
    }
  }
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
