/**
  ******************************************************************************
  * File Name          : usart.h
  * Description        : This file contains all the functions prototypes for 
  *                      the usart  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

  void MX_USART1_UART_Init(void);
#if USART_MIDI
  void MX_USART3_UART_Init(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
