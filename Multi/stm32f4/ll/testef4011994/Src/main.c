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

#define WS2812_DEADPERIOD 23
#define NCHANNELS 4
#define NCOLS 7
#define NLINESCH 2
#define NLEDSCH NCOLS *NLINESCH
#define NCOLSL NCOLS - 1
#define NLEDS NCOLS *NCOLS
#define MAX_DIV 5

uint16_t WS2812_IO_High = 0xFFFF;
uint16_t WS2812_IO_Low = 0x0000;

volatile uint8_t WS2812_TC = 1;
volatile uint8_t TIM2_overflows = 0;

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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

void DMA1_Stream6_IRQHandler(void)
{
  LL_DMA_ClearFlag_TC6(DMA1);
  // enable TIM2 Update interrupt to append 50us dead period
  LL_TIM_EnableIT_UPDATE(TIM2);
  // disable the DMA channels
  LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
  LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
  LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);
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

void move(int8_t newX, int8_t newY)
{
  uint8_t tempVector[NLEDS][3];
  uint8_t tempX, tempY;
  for (int y = 0; y < NCOLS; y++)
  {
    for (int x = 0; x < NCOLS; x++)
    {
      for (int z = 0; z < 3; z++)
      {
        tempVector[x + (y * NCOLS)][z] = effect[x + (y * NCOLS)][z];
      }
    }
  }

  for (int y = 0; y < NCOLS; y++)
  {
    if ((y - newY) < 0)
    {
      tempY = NCOLS - (-y + newY);
    }
    else if ((y - newY) >= NCOLS)
    {
      tempY = (y - newY) - NCOLS;
    }
    else
      tempY = y - newY;

    for (int x = 0; x < NCOLS; x++)
    {
      if ((x - newX) < 0)
      {
        tempX = NCOLS - (-x + newX);
      }
      else if ((x - newX) >= NCOLS)
      {
        tempX = (x - newX) - NCOLS;
      }
      else
        tempX = x - newX;

      for (int z = 0; z < 3; z++)
      {
        effect[x + (y * NCOLS)][z] = tempVector[tempX + (tempY * NCOLS)][z];
      }
    }
  }
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

void fillFramebuffer()
{
  uint8_t mod, z, tempLine[NCOLS][3];
  for (uint8_t i = 0; i < NCHANNELS; i++)
  {
    for (uint8_t j = 0; j < NLINESCH; j++)
    {
      mod = j % 2;
      for (uint16_t x = 0; x < NCOLS; x++)
      {
        if (mod == 0)
        {
          for (z = 0; z < 3; z++)
          {
            tempLine[x][z] = effect[x + (j * NCOLS) + (i * NLEDSCH)][z];
          }
        }
        else
        {
          for (z = 0; z < 3; z++)
          {
            tempLine[x][z] = effect[NCOLSL - x + (j * NCOLS) + (i * NLEDSCH)][z];
          }
        }
        WS2812_framedata_setPixel(i, x + (j * NCOLS), tempLine[x][0], tempLine[x][1], tempLine[x][2]);
      }
    }
  }
}

void WS2812_sendbuf(uint32_t buffersize)
{
  // transmission complete flag, indicate that transmission is taking place
  WS2812_TC = 0;

  // clear all relevant DMA flags
  LL_DMA_ClearFlag_TE1(DMA1);
  LL_DMA_ClearFlag_TE5(DMA1);
  LL_DMA_ClearFlag_TE6(DMA1);

  // configure the number of bytes to be transferred by the DMA controller
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, buffersize);
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, buffersize);
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, buffersize);

  // clear all TIM2 flags
  TIM2->SR = 0;

  // enable the corresponding DMA channels
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);

  // IMPORTANT: enable the TIM2 DMA requests AFTER enabling the DMA channels!
  LL_TIM_EnableDMAReq_CC1(TIM2);
  LL_TIM_EnableDMAReq_CC2(TIM2);
  LL_TIM_EnableDMAReq_UPDATE(TIM2);

  // preload counter with 34 so TIM2 generates UEV directly to start DMA transfer
  LL_TIM_SetCounter(TIM2, 34);

  // start TIM2
  LL_TIM_EnableCounter(TIM2);
}

void drawCircle(uint8_t *color_steps)
{
  float z = ((float)NCOLS / 2);
  uint16_t coords = 0;
  uint8_t boarder;
  int16_t temp;
  for (int y = -(NCOLS / 2); y <= (NCOLS / 2); y++)
  {
    for (int x = -(NCOLS / 2); x <= (NCOLS / 2); x++)
    {
      coords = (x + (NCOLS / 2)) + ((y + (NCOLS / 2)) * NCOLS);
      for (int i = 0; i < MAX_DIV; i++)
      {
        boarder = 1;
        float perc = 1 - ((float)i / MAX_DIV);
        // printf("%0.1f\n", perc);
        if ((x * x) + (y * y) <= (z * z) * (perc))
        {
          for (int z = 0; z < 3; z++)
          {
            temp = currColor[z] + i * color_steps[z];
            if (temp < 0)
              temp = 0;
            if (temp > 255)
              temp = 255;
            effect[coords][z] = temp;
          }
          boarder = 0;
        }
      }
      if (boarder)
      {
        if ((x * x) + (y * y) >= (z * z))
        {
          for (int z = 0; z < 3; z++)
          {
            temp = currColor[z] - color_steps[z];
            if (temp < 0)
              temp = 0;
            if (temp > 255)
              temp = 255;
            effect[coords][z] = temp;
          }
        }
      }
    }
  }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();

  uint8_t i = 0;
  uint8_t color[] = {5, 10, 30};
  drawCircle(color);
  //move(0, 1);
  fillFramebuffer();
  WS2812_sendbuf(24 * NLEDSCH);

  while (1)
  {
    for (i = 0; i < 6; i++)
    {
      WS2812_framedata_setPixel(0, 0, 0xFF, 0, 0);
      WS2812_framedata_setPixel(0, 1, 0xFF, 0, 0);
      WS2812_framedata_setPixel(0, 2, 0xFF, 0, 0);
      WS2812_framedata_setPixel(0, 3, 0xFF, 0, 0);
      WS2812_framedata_setPixel(0, 4, 0, 0xFF, 0);
      WS2812_framedata_setPixel(0, 5, 0, 0xFF, 0);
      WS2812_framedata_setPixel(0, 6, 0, 0xFF, 0);
      WS2812_framedata_setPixel(0, 7, 0, 0, 0xFF);
      WS2812_framedata_setPixel(0, 8, 0, 0, 0xFF);
      WS2812_framedata_setPixel(0, 9, 0, 0, 0xFF);
      // WS2812_framedata_setPixel(0, 1, 0b11111111, 0, 0);
      WS2812_sendbuf(24 * NLEDSCH);
      //   // wait some amount of time
      LL_mDelay(3000);
    }
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
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  LL_RCC_HSE_Enable();

  /* Wait till HSE is ready */
  while (LL_RCC_HSE_IsReady() != 1)
  {
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 336, LL_RCC_PLLP_DIV_4);
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

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */
  LL_DMA_InitTypeDef DMA_InitStructure = {0};
  LL_TIM_InitTypeDef TIM_TimeBaseStructure = {0};
  LL_TIM_OC_InitTypeDef TIM_OCInitStructure = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_DMA_DeInit(DMA1, LL_DMA_STREAM_1);
  DMA_InitStructure.Channel = LL_DMA_CHANNEL_3;
  DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->BSRR;
  DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_High;
  DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStructure.NbData = 0;
  DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStructure.Priority = LL_DMA_PRIORITY_HIGH;
  DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
  LL_DMA_Init(DMA1, LL_DMA_STREAM_1, &DMA_InitStructure);

  LL_DMA_DeInit(DMA1, LL_DMA_STREAM_5);
  DMA_InitStructure.Channel = LL_DMA_CHANNEL_3;
  DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_framedata;
  DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStructure.NbData = 0;
  DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStructure.Priority = LL_DMA_PRIORITY_HIGH;
  DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
  LL_DMA_Init(DMA1, LL_DMA_STREAM_5, &DMA_InitStructure);

  LL_DMA_DeInit(DMA1, LL_DMA_STREAM_6);
  DMA_InitStructure.Channel = LL_DMA_CHANNEL_3;
  DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)&GPIOA->ODR;
  DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)&WS2812_IO_Low;
  DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStructure.NbData = 0;
  DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
  DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStructure.Priority = LL_DMA_PRIORITY_HIGH;
  DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
  LL_DMA_Init(DMA1, LL_DMA_STREAM_6, &DMA_InitStructure);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_6);

  uint16_t PrescalerValue;

  PrescalerValue = (uint16_t)(SystemCoreClock / 24000000) - 1;

  TIM_TimeBaseStructure.Autoreload = 34; // 800kHz 11 20
  TIM_TimeBaseStructure.Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.ClockDivision = 0;
  TIM_TimeBaseStructure.CounterMode = LL_TIM_COUNTERMODE_UP;
  LL_TIM_Init(TIM2, &TIM_TimeBaseStructure);
  LL_TIM_DisableARRPreload(TIM2);

  TIM_OCInitStructure.OCMode = LL_TIM_OCMODE_FROZEN;
  TIM_OCInitStructure.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OCInitStructure.CompareValue = 11;
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OCInitStructure);
  LL_TIM_OC_DisablePreload(TIM2, LL_TIM_CHANNEL_CH1);

  /* Timing Mode configuration: Channel 2 */
  TIM_OCInitStructure.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OCInitStructure.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OCInitStructure.CompareValue = 20;
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH2, &TIM_OCInitStructure);
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
  /* DMA1_Stream1_IRQn interrupt configuration */
  // NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  // /* DMA1_Stream5_IRQn interrupt configuration */
  // NVIC_SetPriority(DMA1_Stream5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  // GPIOA pins WS2812 data outputs
  GPIO_InitStruct.Pin = 0xFFFF;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
