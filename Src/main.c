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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  typedef struct Notes {
	char symbol[2];
	char octave[1];
	char note[3];
  } Note;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))

#define NOTE_OFF  	0x80
#define NOTE_ON  	0x90
#define POLY_PRESS  0xA0
#define CTRL_CHANGE 0xB0
#define PROG_CHANGE 0xC0
#define CHAN_PRESS  0xD0
#define PITCH_BEND  0xE0
#define SYSTEM  	0xF0

#define MIDI_C0            0
#define MIDI_D0            2
#define MIDI_E0            4
#define MIDI_F0            5
#define MIDI_G0            7
#define MIDI_A0            9
#define MIDI_B0           11
#define MIDI_C            60
#define MIDI_D            62
#define MIDI_E            64
#define MIDI_F            65
#define MIDI_G            67
#define MIDI_A            69
#define MIDI_B            71
#define MIDI_SHARP         1
#define MIDI_FLAT         -1
#define MIDI_OCTAVE       12

#define TIM3_CCR1_Address 0x40000434	// physical memory address of Timer 3 CCR1 register
#define WS0 			   9
#define WS1 			  17
#define LED_NUMBER 		   4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  volatile int cntMessage = 0;
  volatile int waitForMessage = 0;
  volatile uint8_t arr[3] = {0,0,0};
  volatile uint8_t arrCol[12][3] = {
		{255, 0, 0}, 	//red
		{255, 127, 0},  //orange
		{255, 255, 0},  //yellow
		{127, 255, 0},  //green yellow
		{0, 255, 0},    //green
		{0, 255, 127},	//green cyan
		{0, 255, 255},	//cyan
		{0, 127, 255},	//blue cyan
		{0,0,255},		//blue
		{127,0,255},	//blue magenta
		{255,0,255},	//magenta
		{255,0,127}		//red magenta
  };
  uint8_t velocity = 0;
  Note note;

  uint16_t LED_BYTE_Buffer[100];

  uint8_t GammaE[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
		2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
		6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
		11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
		19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
		29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
		40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
		55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
		71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
		90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
		111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
		135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
		162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
		191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
		222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255
	};  //Gamma Correction
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_DMA_Init(void);
/* USER CODE BEGIN PFP */
void usart_process_data(const void* data, size_t len);
void usart_send_string(const char* str);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

void WS2812_send(volatile uint8_t (*color)[3], volatile uint16_t len)
{
	volatile uint8_t j;
	volatile uint8_t led;
	volatile uint16_t memaddr;
	volatile uint16_t buffersize;

	buffersize = (len*24)+42;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;				// reset buffer memory index
	led = 0;					// reset led index

	// fill transmit buffer with correct compare values to achieve
	// correct pulse widths according to color values
	while (len)
	{
		for (j = 0; j < 8; j++)					// GREEN data
		{
			if ( (color[led][1]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = WS1; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = WS0;	// compare value for logical 0
			}
			memaddr++;
		}

		for (j = 0; j < 8; j++)					// RED data
		{
			if ( (color[led][0]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
				LED_BYTE_Buffer[memaddr] = WS1; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = WS0;	// compare value for logical 0
			}
			memaddr++;
		}

		for (j = 0; j < 8; j++)					// BLUE data
		{
			if ( (color[led][2]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = WS1; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = WS0;	// compare value for logical 0
			}
			memaddr++;
		}

		led++;
		len--;
	}

	// add needed delay at end of byte cycle, pulsewidth = 0
	while(memaddr < buffersize)
	{
		LED_BYTE_Buffer[memaddr] = 0;
		memaddr++;
	}

	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, buffersize);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6); // enable DMA channel 6
	LL_TIM_EnableCounter(TIM3);						// enable Timer 3
	while(!LL_DMA_IsActiveFlag_TC6(DMA1)); 	// wait until transfer complete
	LL_TIM_DisableCounter(TIM3);					// disable Timer 3
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);	// disable DMA channel 6
	LL_DMA_ClearFlag_TC6(DMA1);			// clear DMA1 Channel 6 transfer complete flag
}

void WS2812_sendv2(volatile uint8_t *color, volatile uint16_t len)
{
	volatile uint8_t j;
	volatile uint16_t memaddr;
	volatile uint16_t buffersize;
	char str[100];

	buffersize = (len*24)+42;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;				// reset buffer memory index

	// fill transmit buffer with correct compare values to achieve
	// correct pulse widths according to color values
	while (len)
	{
		sprintf(str, "R: %d G: %d B: %d \n", color[0], color[1], color[2]);
		usart_send_string(str);
		for (j = 0; j < 8; j++)					// GREEN data
		{
			if ( (color[1]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = WS1; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = WS0;	// compare value for logical 0
			}
			memaddr++;
		}

		for (j = 0; j < 8; j++)					// RED data
		{
			if ( (color[0]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = WS1; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = WS0;	// compare value for logical 0
			}
			memaddr++;
		}

		for (j = 0; j < 8; j++)					// BLUE data
		{
			if ( (color[2]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = WS1; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = WS0;	// compare value for logical 0
			}
			memaddr++;
		}

		len--;
	}

	// add needed delay at end of byte cycle, pulsewidth = 0
	while(memaddr < buffersize)
	{
		LED_BYTE_Buffer[memaddr] = 0;
		memaddr++;
	}

	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, buffersize);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6); // enable DMA channel 6
	LL_TIM_EnableCounter(TIM3);						// enable Timer 3
	while(!LL_DMA_IsActiveFlag_TC6(DMA1)); 	// wait until transfer complete
	LL_TIM_DisableCounter(TIM3);					// disable Timer 3
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);	// disable DMA channel 6
	LL_DMA_ClearFlag_TC6(DMA1);			// clear DMA1 Channel 6 transfer complete flag
}

void setVelocity(uint8_t vel) {
	velocity = vel*2;
}

void setNote(int nt) {
	int resto = nt % MIDI_OCTAVE;
	uint8_t res = nt / MIDI_OCTAVE;
	char resStr[1];
	sprintf(resStr, "%1d", res);
	//strcpy(note.octave,resStr);
	switch (resto){
		case MIDI_C0:
			strcpy(note.symbol,"C");
			break;
		case MIDI_C0 + MIDI_SHARP:
			strcpy(note.symbol,"C#");
			break;
		case MIDI_D0:
			strcpy(note.symbol,"D");
			break;
		case MIDI_D0 + MIDI_SHARP:
			strcpy(note.symbol,"D#");
			break;
		case MIDI_E0:
			strcpy(note.symbol,"E");
			break;
		case MIDI_F0:
			strcpy(note.symbol,"F");
			break;	
		case MIDI_F0 + MIDI_SHARP:
			strcpy(note.symbol,"F#");
			break;	
		case MIDI_G0:
			strcpy(note.symbol,"G");
			break;
		case MIDI_G0 + MIDI_SHARP:
			strcpy(note.symbol,"G#");
			break;
		case MIDI_A0:
			strcpy(note.symbol,"A");
			break;
		case MIDI_A0 + MIDI_SHARP:
			strcpy(note.symbol,"A#");
			break;
		case MIDI_B0:
			strcpy(note.symbol,"B");
			break;
	}	
	arr[0] = arrCol[resto][0];
	arr[1] = arrCol[resto][1];
	arr[2] = arrCol[resto][2];
	usart_send_string(note.symbol);
	usart_send_string(resStr);
	usart_send_string("\n");
	// strcpy(note.octave,resStr);
	// strcpy(note.note,note.symbol);
	// strcat(note.note, note.octave);
	// usart_send_string(note.note);
}

void calculateLeds() {
	arr[0]=(arr[0]*GammaE[velocity+1])/255;
	arr[1]=(arr[1]*GammaE[velocity+1])/255;
	arr[2]=(arr[2]*GammaE[velocity+1])/255;
}
/* USER CODE END 0 */

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
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled 
  */
  LL_GPIO_AF_DisableRemap_SWJ();


  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_DMA_Init();

  usart_send_string("Init Done. \n");
  WS2812_sendv2(arr, 4);

//  int16_t i;
  while (1)
  {
    // for (i = 0; i < 766; i += 1)
	  // 	{
    //     WS2812_send(&eightbit[i], 4);
    //     LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    //     LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    //     Delay(50000); // 1 = 0.127 us -> 8 MHz 		50000 -> 6.25ms
    //     LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    //     Delay(1);
    //     LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
	  // 	}
  }
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
    Error_Handler();  
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_3);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(24000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(24000000);
}

LL_TIM_InitTypeDef TIM_InitStruct = {0};
LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
LL_DMA_InitTypeDef DMA_InitStruct = {0};
LL_USART_InitTypeDef USART_InitStruct = {0};


static void MX_TIM3_Init(void)
{
  uint16_t PrescalerValue;

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**TIM3 GPIO Configuration  
  PA6   ------> TIM3_CH1 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;

  /* Time base configuration */
  TIM_InitStruct.Autoreload = 29; //~805 khz (72/2/29 -> 1.25 us)
  TIM_InitStruct.Prescaler = PrescalerValue;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  LL_TIM_Init(TIM3, &TIM_InitStruct);

  //LL_TIM_DisableARRPreload(TIM3);
  //LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);

  //   LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH1);
  //   LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  //   LL_TIM_DisableMasterSlaveMode(TIM3);


    /* Init with LL driver */
  /* DMA controller clock enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* TIM3 DMA Init */
  	LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_6);

  	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)TIM3_CCR1_Address;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)LED_BYTE_Buffer;
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.NbData = 42;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;

	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_6,&DMA_InitStruct);

	/* TIM3 CC1 DMA Request enable */
	//TIM_DMACmd(TIM3, TIM_DMA_CC1, ENABLE);
	LL_TIM_EnableDMAReq_CC1(TIM3);
}

static void MX_USART1_UART_Init(void)
{
  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration  
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration  
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 31270;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE BEGIN USART2_Init 2 */
  LL_USART_EnableIT_RXNE(USART2);
  /* USER CODE END USART2_Init 2 */

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
  /* DMA1_Channel6_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_5 
                          |LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_12 
                          |LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_10 
                          |LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14 
                          |LL_GPIO_PIN_15|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5 
                          |LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void usart_process_data(const void* data, size_t len) {
    const uint8_t* d = data;
    while (len--) {
        LL_USART_TransmitData8(USART1, *d++);
        while (!LL_USART_IsActiveFlag_TXE(USART1));
    }
    while (!LL_USART_IsActiveFlag_TC(USART1));
}

void usart_send_string(const char* str) {
    usart_process_data(str, strlen(str));
}

void USART2_IRQHandler(void) {
	static uint8_t usart_rx_buffer;
	static uint8_t usart_rx_message[3];
	char str[100];
	long temp;
	if(LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2)) {
		usart_rx_buffer = LL_USART_ReceiveData8(USART2);
		switch(usart_rx_buffer) {
		   case NOTE_ON  : //NOTE_ON Channel 1
			  cntMessage = 3;
			  waitForMessage=1;
		      break;

		   case NOTE_OFF  : //NOTE_OFF Channel 1
			  cntMessage = 3;
			  waitForMessage=1;
		      break;
		}
		if (cntMessage>0) {
//			char temp[1];
//			itoa (usart_rx_buffer, temp, ARRAY_LEN(temp));
			usart_rx_message[3-cntMessage]=usart_rx_buffer;
			cntMessage--;
		}
		else if(waitForMessage) {
			// usart_process_data(&usart_rx_message[0],ARRAY_LEN(usart_rx_message));
			for (int i = 0 ; i < ARRAY_LEN(usart_rx_message); i++) {
				sprintf(&str[i*4],"%03x ", usart_rx_message[i]);
				temp = strtol(&str[i*4],NULL,16);
				if(i!=0) sprintf(&str[i*4],"%03ld ", temp);
				if(i==1) setNote(temp);
				if(i==2) setVelocity(temp);
			}
			usart_send_string(str);
			//usart_send_string(note.note);
			usart_send_string("\n");
			switch(usart_rx_message[0]) {
				case NOTE_ON  : //NOTE_ON Channel 1
					calculateLeds();
					WS2812_sendv2(arr,4);
					break;

				case NOTE_OFF  : //NOTE_OFF Channel 1
					// arr[1] = 0;
					// WS2812_sendv2(arr,4);
					break;
			}
			waitForMessage=0;
		}
	}
}
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

#ifdef  USE_FULL_ASSERT
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
