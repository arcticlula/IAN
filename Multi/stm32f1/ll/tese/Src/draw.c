#include "stdint.h"
#include "string.h"
#include "stm32f1xx_ll_utils.h"
#include "definitions.h"
#include "ws2812.h"
#include "draw.h"

uint8_t currColor[3] = {60, 25, 0};
uint8_t letterColor[3] = {0, 10, 0};

volatile uint8_t arr[3] = {0, 0, 0};
const uint8_t arrCol[13][3] = {
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

void fillFramebuffer(void)
{
  uint8_t mod, z, tempLine[NCOLS][3], invI, invJ, invX;
  uint16_t coords;
#if REVERSE_ORIENTATION
  for (uint8_t x = 0; x < NCOLS; x++)
  {
    for (uint8_t i = 0; i < NCHANNELS; i++)
    {
      for (uint8_t j = 0; j < NLINESCH; j++)
      {
        mod = j % 2;
        if ((j + (i * NLINESCH)) > NCOLSL)
          break;
        coords = j + (i * NLINESCH) + (x * NCOLS);

#if MIRROR_X
        invX = mod ? NCOLSL - x : x;
#if MIRROR_Y
        invI = i;
        invJ = j;
#else
        invI = NCHANNELS - 1 - i;
        invJ = NLINESCH - 1 - j;
#endif
#else
        invX = mod ? x : NCOLSL - x;
#if MIRROR_Y
        invI = NCHANNELS - 1 - i;
        invJ = NLINESCH - 1 - j;
#else
        invI = i;
        invJ = j;
#endif
#endif
#else
  for (uint8_t i = 0; i < NCHANNELS; i++)
  {
    for (uint8_t j = 0; j < NLINESCH; j++)
    {
      mod = j % 2;
      for (uint8_t x = 0; x < NCOLS; x++)
      {
        if ((x + (j * NCOLS) + (i * NLEDSCH)) > NLEDSL)
          break;
        coords = x + (j * NCOLS) + (i * NLEDSCH);

#if MIRROR_X
        invX = mod ? x : NCOLSL - x;
#else
        invX = mod ? NCOLSL - x : x;
#endif
#if MIRROR_Y
        invI = NCHANNELS - 1 - i;
        invJ = NLINESCH - 1 - j;
#else
        invI = i;
        invJ = j;
#endif
#endif
        for (z = 0; z < 3; z++)
        {
          tempLine[x][z] = BIT_CHECK(textLayer[DIV_8(coords)], REST_8(coords)) ? letterColor[z] : backLayer[coords][z];
          // tempLine[x][z] = backLayer[coords][z];
        }
        WS2812_framedata_setPixel(invI, invX + (invJ * NCOLS), tempLine[x][0], tempLine[x][1], tempLine[x][2]);
      }
    }
  }
}

void clearText(void)
{
  for (uint8_t i = 0; i < NLEDSBIT; i++)
  {
    textLayer[i] = 0;
  }
}

void clearFramebuffer(void)
{
  for (uint8_t i = 0; i < NCHANNELS; i++)
  {
    for (uint8_t j = 0; j < NLINESCH; j++)
    {
      for (uint16_t x = 0; x < NCOLS; x++)
      {
        if ((x + (j * NCOLS) + (i * NLEDSCH)) > NLEDSL)
          break;
        WS2812_framedata_setPixel(i, x + (j * NCOLS), arrCol[12][0], arrCol[12][1], arrCol[12][2]);
      }
    }
  }
}

/* void move(int8_t newX, int8_t newY)
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

void rotate(void)
{
  uint8_t tempVector[NLEDS];
  uint8_t x, y;
  for (y = 0; y < NCOLS; y++)
  {
    for (x = 0; x < NCOLS; x++)
    {
      tempVector[x + (y * NCOLS)] = backlayer[x + (y * NCOLS)];
    }
  }
  printf("\n");

  for (y = 0; y < NCOLS; y++)
  {
    for (x = 0; x < NCOLS; x++)
    {
      textlayer[-y - 1 + (x + 1) * NCOLS] = tempVector[x + (y * NCOLS)];
    }
  }
} */

void drawColor(uint8_t *color)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      for (int z = 0; z < 3; z++)
      {
        backLayer[x + (y * NCOLS)][z] = color[z];
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

void drawLine(uint8_t px, uint8_t py)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      if (x == px || y == py)
      {
        for (int z = 0; z < 3; z++)
        {
          backLayer[x + (y * NCOLS)][z] = currColor[z];
        }
      }
    }
  }
}

void drawPixel(uint8_t px, uint8_t py)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      if (x == px && y == py)
      {
        for (int z = 0; z < 3; z++)
        {
          backLayer[x + (y * NCOLS)][z] = currColor[z];
        }
      }
    }
  }
}

/* Draw Strings */

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
    drawLetter(frase[i], posX, posY);
    // posX = (posI - (nLine * (NCOLS - 3))) + 1;
    incX++;
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