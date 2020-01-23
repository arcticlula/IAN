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
/* this define sets the number of TIM2 overflows
 * to append to the data frame for the LEDs to 
 * load the received data into their registers */

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define POLY_PRESS 0xA0
#define CTRL_CHANGE 0xB0
#define PROG_CHANGE 0xC0
#define CHAN_PRESS 0xD0
#define PITCH_BEND 0xE0
#define SYSTEM 0xF0

#define MIDI_C0 0
#define MIDI_D0 2
#define MIDI_E0 4
#define MIDI_F0 5
#define MIDI_G0 7
#define MIDI_A0 9
#define MIDI_B0 11
#define MIDI_C 60
#define MIDI_D 62
#define MIDI_E 64
#define MIDI_F 65
#define MIDI_G 67
#define MIDI_A 69
#define MIDI_B 71
#define MIDI_SHARP 1
#define MIDI_FLAT -1
#define MIDI_OCTAVE 12

#define MAX_BRIGHT 0.1 // 0 - 1

#define WS2812_DEADPERIOD 40
#define MAX_DIV 5
#define NCHANNELS 8
#define NCOLS 16
#define NLINESCH 2
#define NLEDSCH NCOLS *NLINESCH
#define NCOLSL NCOLS - 1
#define NLEDS NCOLS *NCOLS

#define DIV_8(a) ((a) / 8)
#define REST_8(a) ((a) % 8)
#define NLEDSMOD REST_8(NLEDS)
#if NLEDSMOD
#define NLEDSBIT DIV_8(NLEDS) + 1
#else
#define NLEDSBIT DIV_8(NLEDS)
#endif
#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))

uint16_t WS2812_IO_High = 0xFFFF;
uint16_t WS2812_IO_Low = 0x0000;

volatile uint8_t WS2812_TC = 1;
volatile uint8_t TIM2_overflows = 0;

volatile int cntMessage = 0;
volatile int waitForMessage = 0;
volatile int resto = 12;
volatile uint8_t arr[3] = {0, 0, 0};
volatile uint8_t arrCol[13][3] = {
    {50, 0, 0},   //red ~~
    {50, 25, 0},  //orange ~~
    {50, 50, 0},  //amarelo ~~
    {50, 38, 0},  //castanho ~~
    {0, 50, 36},  //azul turquesa ~~
    {25, 0, 50},  //cor de rosa ~~
    {50, 0, 25},  //rosa choque ~~
    {50, 0, 50},  //violeta escuro ~~
    {0, 0, 40},   //azul escuro
    {30, 30, 30}, //cinzento
    {60, 60, 20}, //branca
    {60, 30, 20}, //red magenta
    {0, 0, 0}};
const uint8_t font4x6[96][2] = {
    {0x00, 0x00}, /*SPACE*/
    {0x49, 0x08}, /*'!'*/
    {0xb4, 0x00}, /*'"'*/
    {0xbe, 0xf6}, /*'#'*/
    {0x7b, 0x7a}, /*'$'*/
    {0xa5, 0x94}, /*'%'*/
    {0x55, 0xb8}, /*'&'*/
    {0x48, 0x00}, /*'''*/
    {0x29, 0x44}, /*'('*/
    {0x44, 0x2a}, /*')'*/
    {0x15, 0xa0}, /*'*'*/
    {0x0b, 0x42}, /*'+'*/
    {0x00, 0x50}, /*','*/
    {0x03, 0x02}, /*'-'*/
    {0x00, 0x08}, /*'.'*/
    {0x25, 0x90}, /*'/'*/
    {0x76, 0xba}, /*'0'*/
    {0x59, 0x5c}, /*'1'*/
    {0xc5, 0x9e}, /*'2'*/
    {0xc5, 0x38}, /*'3'*/
    {0x92, 0xe6}, /*'4'*/
    {0xf3, 0x3a}, /*'5'*/
    {0x73, 0xba}, /*'6'*/
    {0xe5, 0x90}, /*'7'*/
    {0x77, 0xba}, /*'8'*/
    {0x77, 0x3a}, /*'9'*/
    {0x08, 0x40}, /*':'*/
    {0x08, 0x50}, /*';'*/
    {0x2a, 0x44}, /*'<'*/
    {0x1c, 0xe0}, /*'='*/
    {0x88, 0x52}, /*'>'*/
    {0xe5, 0x08}, /*'?'*/
    {0x56, 0x8e}, /*'@'*/
    {0x77, 0xb6}, /*'A'*/
    {0x77, 0xb8}, /*'B'*/
    {0x72, 0x8c}, /*'C'*/
    {0xd6, 0xba}, /*'D'*/
    {0x73, 0x9e}, /*'E'*/
    {0x73, 0x92}, /*'F'*/
    {0x72, 0xae}, /*'G'*/
    {0xb7, 0xb6}, /*'H'*/
    {0xe9, 0x5c}, /*'I'*/
    {0x64, 0xaa}, /*'J'*/
    {0xb7, 0xb4}, /*'K'*/
    {0x92, 0x9c}, /*'L'*/
    {0xbe, 0xb6}, /*'M'*/
    {0xd6, 0xb6}, /*'N'*/
    {0x56, 0xaa}, /*'O'*/
    {0xd7, 0x92}, /*'P'*/
    {0x76, 0xee}, /*'Q'*/
    {0x77, 0xb4}, /*'R'*/
    {0x71, 0x38}, /*'S'*/
    {0xe9, 0x48}, /*'T'*/
    {0xb6, 0xae}, /*'U'*/
    {0xb6, 0xaa}, /*'V'*/
    {0xb6, 0xf6}, /*'W'*/
    {0xb5, 0xb4}, /*'X'*/
    {0xb5, 0x48}, /*'Y'*/
    {0xe5, 0x9c}, /*'Z'*/
    {0x69, 0x4c}, /*'['*/
    {0x91, 0x24}, /*'\'*/
    {0x64, 0x2e}, /*']'*/
    {0x54, 0x00}, /*'^'*/
    {0x00, 0x1c}, /*'_'*/
    {0x44, 0x00}, /*'`'*/
    {0x0e, 0xae}, /*'a'*/
    {0x9a, 0xba}, /*'b'*/
    {0x0e, 0x8c}, /*'c'*/
    {0x2e, 0xae}, /*'d'*/
    {0x0e, 0xce}, /*'e'*/
    {0x56, 0xd0}, /*'f'*/
    {0x55, 0x3B}, /*'g'*/
    {0x93, 0xb4}, /*'h'*/
    {0x41, 0x44}, /*'i'*/
    {0x41, 0x51}, /*'j'*/
    {0x97, 0xb4}, /*'k'*/
    {0x49, 0x44}, /*'l'*/
    {0x17, 0xb6}, /*'m'*/
    {0x1a, 0xb6}, /*'n'*/
    {0x0a, 0xaa}, /*'o'*/
    {0xd6, 0xd3}, /*'p'*/
    {0x76, 0x67}, /*'q'*/
    {0x17, 0x90}, /*'r'*/
    {0x0f, 0x38}, /*'s'*/
    {0x9a, 0x8c}, /*'t'*/
    {0x16, 0xae}, /*'u'*/
    {0x16, 0xba}, /*'v'*/
    {0x16, 0xf6}, /*'w'*/
    {0x15, 0xb4}, /*'x'*/
    {0xb5, 0x2b}, /*'y'*/
    {0x1c, 0x5e}, /*'z'*/
    {0x6b, 0x4c}, /*'{'*/
    {0x49, 0x48}, /*'|'*/
    {0xc9, 0x5a}, /*'}'*/
    {0x54, 0x00}, /*'~'*/
    {0x56, 0xe2}  /*''*/
};
uint8_t velocity = 0;
typedef struct Notes
{
  char symbol[3];
  char octave;
  char note[4];
} Note;
Note volatile note;
char noteBuffer[10][5] = {"", "", "", "", "", "", "", "", "", ""};

/* WS2812 framebuffer
 * buffersize = (#LEDs / 16) * 24 */
uint16_t WS2812_IO_framedata[24 * NLEDSCH];

uint8_t currColor[3] = {60, 25, 0};
uint8_t backLayer[NLEDS][3];
uint8_t textLayer[NLEDSBIT];
uint8_t letterColor[3] = {0, 10, 0};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);

unsigned char getFontLine(unsigned char data, int line_num)
{
  const uint8_t index = (data - 32);
  unsigned char pixel = 0;
  if ((font4x6[index][1] & 1) == 1)
    line_num -= 1;
  if (line_num == 0)
  {
    pixel = (font4x6[index][0]) >> 4;
  }
  else if (line_num == 1)
  {
    pixel = (font4x6[index][0]) >> 1;
  }
  else if (line_num == 2)
  {
    // Split over 2 bytes
    return (((font4x6[index][0]) & 0x03) << 2) | (((font4x6[index][1]) & 0x02));
  }
  else if (line_num == 3)
  {
    pixel = (font4x6[index][1]) >> 4;
  }
  else if (line_num == 4)
  {
    pixel = (font4x6[index][1]) >> 1;
  }
  return pixel & 0xE;
}

void calculateLeds()
{
  arr[0] = ((arrCol[resto][0] * (velocity + 1)) / 255) * MAX_BRIGHT;
  arr[1] = ((arrCol[resto][1] * (velocity + 1)) / 255) * MAX_BRIGHT;
  arr[2] = ((arrCol[resto][2] * (velocity + 1)) / 255) * MAX_BRIGHT;
}

void setVelocity(uint8_t vel)
{
  velocity = vel * 2;
}

void setNote(int nt)
{
  resto = nt % MIDI_OCTAVE;
  note.octave = (nt / MIDI_OCTAVE) + '0';
  for (size_t i = 0; i < 3; i++)
  {
    letterColor[i] = arrCol[resto][i];
  }
  switch (resto)
  {
  case MIDI_C0:
    strcpy(note.symbol, "C");
    break;
  case MIDI_C0 + MIDI_SHARP:
    strcpy(note.symbol, "C#");
    break;
  case MIDI_D0:
    strcpy(note.symbol, "D");
    break;
  case MIDI_E0 + MIDI_FLAT:
    strcpy(note.symbol, "Eb");
    break;
  case MIDI_E0:
    strcpy(note.symbol, "E");
    break;
  case MIDI_F0:
    strcpy(note.symbol, "F");
    break;
  case MIDI_F0 + MIDI_SHARP:
    strcpy(note.symbol, "F#");
    break;
  case MIDI_G0:
    strcpy(note.symbol, "G");
    break;
  case MIDI_A0 + MIDI_FLAT:
    strcpy(note.symbol, "Ab");
    break;
  case MIDI_A0:
    strcpy(note.symbol, "A");
    break;
  case MIDI_B0 + MIDI_FLAT:
    strcpy(note.symbol, "Bb");
    break;
  case MIDI_B0:
    strcpy(note.symbol, "B");
    break;
  }
  sprintf(note.note, "%s%c", note.symbol, note.octave);
  for (size_t k = ARRAY_LEN(noteBuffer); k > 0; k--)
  {
    strcpy(noteBuffer[k], noteBuffer[k - 1]);
  }
  sprintf(noteBuffer[0], "%s%c", note.note, ' ');
  arr[0] = arrCol[resto][0];
  arr[1] = arrCol[resto][1];
  arr[2] = arrCol[resto][2];
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

/* void WS2812_framedata_setPixel(uint8_t row, uint16_t column, uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    // write new data for pixel
    WS2812_IO_framedata[((column * 24) + i)] = 0;
    WS2812_IO_framedata[((column * 24) + 8 + i)] = 0;
    WS2812_IO_framedata[((column * 24) + 16 + i)] = 0b1111111;
  }
} */

void WS2812_framedata_setRow(uint8_t row, uint16_t columns, uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  for (i = 0; i < columns; i++)
  {
    WS2812_framedata_setPixel(row, i, red, green, blue);
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
        tempVector[x + (y * NCOLS)][z] = backLayer[x + (y * NCOLS)][z];
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
        backLayer[x + (y * NCOLS)][z] = tempVector[tempX + (tempY * NCOLS)][z];
      }
    }
  }
}

void clearFramebuffer()
{
  for (uint8_t i = 0; i < NCHANNELS; i++)
  {
    for (uint8_t j = 0; j < NLINESCH; j++)
    {
      for (uint16_t x = 0; x < NCOLS; x++)
      {
        if ((x + (j * NCOLS) + (i * NLEDSCH)) > NLEDS - 1)
          break;
        WS2812_framedata_setPixel(i, x + (j * NCOLS), arrCol[12][0], arrCol[12][1], arrCol[12][2]);
      }
    }
  }
}

void clearText()
{
  for (uint8_t i = 0; i < NLEDSBIT; i++)
  {
    textLayer[i] = 0;
  }
}

void fillFramebuffer()
{
  uint8_t mod, z, tempLine[NCOLS][3];
  uint16_t coords;
  for (uint8_t i = 0; i < NCHANNELS; i++)
  {
    for (uint8_t j = 0; j < NLINESCH; j++)
    {
      mod = j % 2;
      for (uint8_t x = 0; x < NCOLS; x++)
      {
        if ((x + (j * NCOLS) + (i * NLEDSCH)) > NLEDS - 1)
          break;
        // coords = mod ? NCOLSL - x + (j * NCOLS) + (i * NLEDSCH) : x + (j * NCOLS) + (i * NLEDSCH);
        coords = mod ? x + (j * NCOLS) + (i * NLEDSCH) : NCOLSL - x + (j * NCOLS) + (i * NLEDSCH);
        for (z = 0; z < 3; z++)
        {
          tempLine[x][z] = BIT_CHECK(textLayer[DIV_8(coords)], REST_8(coords)) ? letterColor[z] : backLayer[coords][z];
          // tempLine[x][z] = backLayer[coords][z];
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

void drawColor(volatile uint8_t *color)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      for (int z = 0; z < 3; z++)
      {
        backLayer[x + (y * NCOLS)][z] = color[z];
        // backLayer[x + (y * NCOLS)][z] = arrCol16[y][z];
      }
    }
  }
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
            backLayer[coords][z] = temp;
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
            backLayer[coords][z] = temp;
          }
        }
      }
    }
  }
}

void drawLine(uint8_t xx, uint8_t yy)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      if (x == xx || y == yy)
      {
        for (int z = 0; z < 3; z++)
        {
          backLayer[x + (y * NCOLS)][z] = currColor[z];
        }
      }
    }
  }
}

void drawScrollingString(char *frase, uint16_t interval_ms, uint8_t space, uint8_t offX, uint8_t offY)
{
  int8_t posX, posY;
  uint8_t fill, width = 3 + space;
  for (uint8_t letter = 0; letter < strlen(frase); letter++)
  {
    for (uint8_t pos = 0; pos < 3; pos++)
    {
      clearText();
      for (uint8_t i = letter, idx = 0; i < strlen(frase); i++, idx++)
      {
        posX = (idx * width) + offX;
        posY = offY;

        for (int8_t y = posY, l = 0; y < (posY + 5); y++, l++)
        {
          if (y >= (NCOLS - offY))
            break;
          fill = getFontLine(frase[i], l);
          for (int8_t x = posX - pos, aX = 3; x < (posX + width) - pos; x++, aX--)
          {
            if (x >= (NCOLS - offX))
              break;
            if (x >= offX)
            {
              if (fill & (1u << aX))
                BIT_SET(textLayer[DIV_8(x + (y * NCOLS))], REST_8(x + (y * NCOLS)));
            }
          }
        }
      }
      fillFramebuffer();
      WS2812_sendbuf(24 * NLEDSCH);
      LL_mDelay(interval_ms);
    }
  }
}

void drawLetter(char letter, uint8_t px, uint8_t py)
{
  uint8_t fill;
  uint8_t val = 0;
  for (uint8_t y = py, l = 0; y <= (py + 5); y++, l++)
  {
    if (y > NCOLS - 1)
      break;
    fill = getFontLine(letter, l);
    for (uint8_t x = px, aX = 3; x <= (px + 3); x++, aX--)
    {
      val = fill & (1u << aX) ? 1 : 0;
      if (val)
      {
        for (uint8_t z = 0; z < 3; z++)
        {
          BIT_SET(textLayer[DIV_8(x + (y * NCOLS))], REST_8(x + (y * NCOLS)));
        }
      }
    }
  }
}

void drawStringArray(uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey)
{
  uint8_t posX, incX = 0, posY, lastY = py, nLine = 0;
  for (size_t j = 0; j < ARRAY_LEN(noteBuffer); j++)
  {
    for (size_t i = 0; i < strlen(noteBuffer[j]); i++)
    {
      posX = incX * (3 + spacex) + px;
      if (posX + ((3 + spacex)) > NCOLS - 2)
      {
        posX = px;
        nLine++;
        incX = 0;
      }
      posY = nLine * (5 + spacey) + py;
      drawLetter(noteBuffer[j][i], posX, posY);
      // posX = (posI - (nLine * (NCOLS - 3))) + 1;
      incX++;
    }
  }
}

void drawString(char *frase, uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey)
{
  uint8_t posX, incX = 0, posY, lastY = py, nLine = 0;
  for (size_t i = 0; i < strlen(frase); i++)
  {
    // posX = (char)frase[i] == (char)32 ? (i * 3) + px - 1 : (i * (3 + spacex)) + px;
    posX = incX * (3 + spacex) + px;
    if (posX + ((3 + spacex)) > NCOLS - 2)
    {
      posX = px;
      nLine++;
      incX = 0;
    }
    posY = nLine * (5 + spacey) + py;
    if (posY + 5 > NCOLS - 1)
    {
      letterColor[0] = 10;
      letterColor[1] = 0;
      letterColor[2] = 0;
      break;
    }
    drawLetter(frase[i], posX, posY);
    // posX = (posI - (nLine * (NCOLS - 3))) + 1;
    incX++;
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
  MX_USART3_UART_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // uint8_t i = 0;
  uint8_t color[] = {5, 10, 30};
  // drawLetter('C', 0, 1);
  // drawLetter('5', 4, 1);

  clearFramebuffer();
  clearText();
  uint8_t blue[3] = {1, 1, 5};
  uint8_t red[3] = {5, 1, 1};
  uint8_t green[3] = {1, 5, 1};

  // drawString("Porque e que", 1, 0, 1, 0);
  // fillFramebuffer();
  // drawColor(green);
  // fillFramebuffer();
  // WS2812_sendbuf(24 * NLEDSCH);
  // LL_mDelay(1000);

  // drawCircle(red);
  // fillFramebuffer();
  // WS2812_sendbuf(24 * NLEDSCH);
  // WS2812_framedata_setPixel(0, 0, blue[0], blue[1], blue[2]);

  // drawScrollingString("O lula", 500, 1, 1, 0);
  // WS2812_sendbuf(24 * NLEDSCH);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // set two pixels (columns) in the defined row (channel 0) to the
    // color values defined in the colors array
    /* for (i = 0; i < 6; i++)
    {
      // wait until the last frame was transmitted
      while (!WS2812_TC)
        ;
      // move(1, 1);
      // fillFramebuffer();
      // WS2812_sendbuf(24 * NLEDSCH);
      // LL_mDelay(500);
      // }
      drawColor(arrCol[i]);
      fillFramebuffer();
      WS2812_sendbuf(24 * NLEDSCH);
      LL_mDelay(1000);
    } */
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
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
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

static void MX_USART3_UART_Init(void)
{

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
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

void drawNote()
{
  drawStringArray(1, 0, 1, 1);
}

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

        calculateLeds();
        clearText();
        drawNote();
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
            setNote(temp);
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
