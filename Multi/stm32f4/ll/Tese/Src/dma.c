/**
  ******************************************************************************
  * File Name          : dma.c
  * Description        : This file provides code for the configuration
  *                      of all the requested memory to memory DMA transfers.
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
#include "peripheral/dma.h"
#include "definitions.h"
#include "ws2812.h"

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{
  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  LL_DMA_InitTypeDef DMA_InitStruct = {0};

  /* TIM1 DMA Init */
  /* TIM1_UP Init */
  LL_DMA_DeInit(DMA2, LL_DMA_STREAM_5);
  DMA_InitStruct.Channel = LL_DMA_CHANNEL_6;
  DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_High;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.NbData = 0;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
  DMA_InitStruct.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
  LL_DMA_Init(DMA2, LL_DMA_STREAM_5, &DMA_InitStruct);

  /* TIM1_CH1 Init */
  LL_DMA_DeInit(DMA2, LL_DMA_STREAM_1);
  DMA_InitStruct.Channel = LL_DMA_CHANNEL_6;
  DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_framedata;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.NbData = 0;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
  DMA_InitStruct.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
  LL_DMA_Init(DMA2, LL_DMA_STREAM_1, &DMA_InitStruct);

  /* TIM1_CH2 Init */
  LL_DMA_DeInit(DMA2, LL_DMA_STREAM_2);
  DMA_InitStruct.Channel = LL_DMA_CHANNEL_6;
  DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_Low;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.NbData = 0;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
  DMA_InitStruct.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
  LL_DMA_Init(DMA2, LL_DMA_STREAM_2, &DMA_InitStruct);

  NVIC_SetPriority(DMA2_Stream2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 1));
  NVIC_EnableIRQ(DMA2_Stream2_IRQn);

  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
