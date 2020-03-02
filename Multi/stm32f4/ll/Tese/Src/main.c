/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
// #include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
// #include "string.h"
#include "peripheral.h"
#include "definitions.h"
#include "ws2812.h"
#include "notes.h"
#include "draw.h"
#include "bluetooth.h"
// #include "minihdlc-master/minihdlc.h"
/* this define sets the number of TIM2 overflows
 * to append to the data frame for the LEDs to 
 * load the received data into their registers */

volatile uint8_t TIM1_overflows = 0;

void SystemClock_Config(void);

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
#if USART_MIDI
  MX_USART6_UART_Init();
#endif
  MX_DMA_Init();
  MX_TIM1_Init();

  arrColor = &colorOriginal[0];
  drawshape_function = drawShapeNote;
  callback_function = circle;
  startReception();

  clearNoteBuffer();
  clearBack();
  clearText();
  clearFramebuffer();
  WS2812_sendbuf(24 * NLEDSCH);
  uint8_t blue[3] = {70, 30, 80};

  while (1)
  {
    for (uint8_t i = 0; i < 12; i++)
    {
      setNote(i);
      drawShape();
      // drawColor(blue);
      drawCenter("Ola", 0, 0);
      fillFramebuffer();
      WS2812_sendbuf(24 * NLEDSCH);
      LL_mDelay(1000);
      while (!WS2812_TC)
        ;
    }
  }

  // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  // LL_mDelay(2000);
  // drawColor(blue);
  // fillFramebuffer();
  // WS2812_sendbuf(24 * NLEDSCH);
  // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  // LL_mDelay(2000);
  // drawColor(red);
  // fillFramebuffer();
  // WS2812_sendbuf(24 * NLEDSCH);
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
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  LL_RCC_HSE_Enable();

  /* Wait till HSE is ready */
  while (LL_RCC_HSE_IsReady() != 1)
  {
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 168, LL_RCC_PLLP_DIV_2);
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
  LL_Init1msTick(84000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(84000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

void DMA2_Stream2_IRQHandler(void)
{
  // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  LL_DMA_ClearFlag_TC2(DMA2);
  // enable TIM1 Update interrupt to append 50us dead period
  LL_TIM_EnableIT_UPDATE(TIM1);
  // disable the DMA streams
  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_5);
  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
  // IMPORTANT: disable the DMA requests, too!
  LL_TIM_DisableDMAReq_CC1(TIM1);
  LL_TIM_DisableDMAReq_CC2(TIM1);
  LL_TIM_DisableDMAReq_UPDATE(TIM1);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
  // Clear TIM1 Interrupt Flag
  // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  LL_TIM_ClearFlag_UPDATE(TIM1);

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
    LL_TIM_DisableCounter(TIM1);
    /* disable the TIM1 Update interrupt again
		 * so it doesn't occur while transmitting data */
    LL_TIM_DisableIT_UPDATE(TIM1);
    // finally indicate that the data frame has been transmitted
    WS2812_TC = 1;
  }
}

#if USART_MIDI
void USART6_IRQHandler(void)
{
  static uint8_t usart_rx_buffer;
  static uint8_t usart_rx_message[3];
  char str[100];
  long temp;
  static uint8_t sizeMsg;
  // volatile uint8_t colorLetter[] = {0, 0, 0};
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  if (LL_USART_IsActiveFlag_RXNE(USART6) && LL_USART_IsEnabledIT_RXNE(USART6))
  {
    usart_rx_buffer = LL_USART_ReceiveData8(USART6);
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
