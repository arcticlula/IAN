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
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "tim.h"
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

void SystemClock_Config(void);

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

#define MODE_TEXT_STATE 0x60

static inline void drawShape(void)
{
  if (drawshape_function && MODE_BACK)
  {
    (*drawshape_function)();
  }
}

static inline void drawText(void)
{
  if (drawtext_function && MODE_TEXT)
  {
    (*drawtext_function)();
  }
}

void frame_handler_function(const uint8_t *frame_buffer, uint16_t frame_length)
{
  uint8_t data;
  data = *frame_buffer++;
  drawtext_function = drawNote;
  switch (data)
  {
  case SETTINGS_BRIGHT:
    data = *frame_buffer++;
    MAX_BRIGHT = (float)data / 20;
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  case MODE_BACK_STATE:
    data = *frame_buffer++;
    MODE_BACK = data;
    clearBack();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  case MODE_BACK_CIRCLE:
    drawshape_function = drawShapeNote;
    callback_function = circle;
    break;
  case MODE_BACK_CIRCLE_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = circle;
    break;
  case MODE_BACK_CROSS:
    drawshape_function = drawShapeNote;
    callback_function = cross;
    break;
  case MODE_BACK_CROSS_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = cross;
    break;
  case MODE_BACK_SQUARE:
    drawshape_function = drawShapeNote;
    callback_function = square;
    break;
  case MODE_BACK_SQUARE_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = square;
    break;
  case MODE_BACK_TRIANGLE:
    drawshape_function = drawShapeNote;
    callback_function = triangle;
    break;
  case MODE_BACK_TRIANGLE_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = triangle;
    break;
  case MODE_BACK_LINES_H:
    drawshape_function = drawShapeNote;
    callback_function = horizontal;
    break;
  case MODE_BACK_LINES_H_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = horizontal;
    break;
  case MODE_BACK_LINES_V:
    drawshape_function = drawShapeNote;
    callback_function = vertical;
    break;
  case MODE_BACK_LINES_V_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = vertical;
    break;
  case MODE_TEXT_STATE:
    data = *frame_buffer++;
    MODE_TEXT = data;
    clearText();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  default:
    break;
  }

  if (data > 0x20 && data < 0x60)
  {
    data = *frame_buffer++;
    MAX_DIV = data;
    data = *frame_buffer++;
    orientation = data;
    minihdlc_send_frame(frame_buffer, frame_length);
  }
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

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
#if USART_MIDI
  MX_USART3_UART_Init();
#endif
  MX_DMA_Init();
  MX_TIM2_Init();

  startReception();

  clearNoteBuffer();
  clearBack();
  clearText();
  clearFramebuffer();
  WS2812_sendbuf(24 * NLEDSCH);

  uint8_t blue[3] = {1, 1, 4};
  uint8_t red[3] = {4, 1, 1};

  drawColor(blue);
  fillFramebuffer();
  WS2812_sendbuf(24 * NLEDSCH);

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
    // for (uint8_t i = 0; i < 12; i++)
    // {
    //   setNote(i);
    //   drawShape();
    //   fillFramebuffer();
    //   WS2812_sendbuf(24 * NLEDSCH);
    //   LL_mDelay(1000);
    //   while (!WS2812_TC)
    //     ;
    // }
    // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    while (!WS2812_TC)
      ;
    // LL_mDelay(800);
    drawColor(blue);
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    LL_mDelay(800);
    drawColor(red);
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);

    // move(1, 1);
    // fillFramebuffer();
    // WS2812_sendbuf(24 * NLEDSCH);
    // LL_mDelay(500);
    // }
    // drawString("Ordem dos Engenheiros Tecnicos", 1, 1, 1, 0);
    // rotate()
    // drawColor(arrCol[i]);
    // fillFramebuffer();
    // WS2812_sendbuf(24 * NLEDSCH);
  }
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

void DMA1_Channel7_IRQHandler(void)
{
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
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
  // if (LL_TIM_IsActiveFlag_UPDATE(TIM2))
  // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
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
    // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
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
  // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
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
        drawText();
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
