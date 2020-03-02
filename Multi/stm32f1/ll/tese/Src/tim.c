/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "definitions.h"
#include "ws2812.h"

uint32_t tim_period;
uint32_t timer_reset_pulse_period;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_DMA_InitTypeDef DMA_InitStruct = {0};
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
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
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
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
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
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
