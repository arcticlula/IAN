#include "stdint.h"
#include "string.h"
#include "stm32f1xx_ll_utils.h"
#include "definitions.h"
#include "notes.h"
#include "ws2812.h"
#include "draw.h"

uint8_t currColor[3] = {60, 25, 0};
uint8_t letterColor[3] = {0, 10, 0};

volatile uint8_t arr[3] = {0, 0, 0};
/* uint8_t arrColor[13][3] = {
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
    {0, 0, 0}}; */

/* uint8_t arrColor[13][3] = {
    {123, 0, 0},    //red ~~
    {123, 90, 0},   //orange ~~
    {123, 123, 0},  //amarelo ~~
    {123, 109, 0},  //castanho ~~
    {0, 123, 106},  //azul turquesa ~~
    {90, 0, 123},   //cor de rosa ~~
    {123, 0, 90},   //rosa choque ~~
    {123, 0, 123},  //violeta escuro ~~
    {0, 0, 112},    //azul escuro
    {98, 98, 98},   //cinzento
    {133, 133, 82}, //branca
    {133, 98, 82},  //red magenta
    {0, 0, 0}}; */

/* uint8_t arrColor[13][3] = {
    {51, 51, 51},    //red ~~
    {62, 62, 62},    //orange ~~
    {74, 74, 74},    //amarelo ~~
    {85, 85, 85},    //castanho ~~
    {96, 96, 96},    //azul turquesa ~~
    {108, 108, 108}, //cor de rosa ~~
    {119, 119, 119}, //rosa choque ~~
    {130, 130, 130}, //violeta escuro ~~
    {142, 142, 142}, //azul escuro
    {153, 153, 153}, //cinzento
    {164, 164, 164}, //branca
    {176, 176, 176}, //red magenta
    {0, 0, 0}}; */

uint8_t arrColor[13][3] = {
    {78, 78, 78},    //red ~~
    {116, 99, 94},   //orange ~~
    {101, 105, 105}, //amarelo ~~
    {72, 72, 36},    //castanho ~~
    {39, 39, 39},    //azul turquesa ~~
    {58, 49, 47},    //cor de rosa ~~
    {50, 53, 53},    //rosa choque ~~
    {36, 36, 18},    //violeta escuro ~~
    {117, 117, 117}, //azul escuro
    {174, 148, 141}, //cinzento
    {151, 158, 158}, //branca
    {108, 108, 54},  //red magenta
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

uint8_t orientation = 0;
uint8_t MAX_DIV = 3;
float MAX_BRIGHT = 0.5;
uint8_t MODE_BACK = 1;
uint8_t MODE_TEXT = 1;
callback_type callback_function;

static inline uint8_t callback(int8_t x, int8_t y, float layer)
{
  if (callback_function)
  {
    return (*callback_function)(x, y, layer);
  }
  return 0;
}

void fillFramebuffer(void)
{
  uint8_t mod, tempPoint[3], invI, invJ, invX, tmp;
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
#if COLOR_1BIT
        tmp = BIT_CHECK(textLayer[DIV_8(coords)], REST_8(coords));
        if (tmp)
          setColorFB(tempPoint, textColor);
#else
        tmp = textLayer[coords][0] + textLayer[coords][1] + textLayer[coords][2];
        if (tmp)
          setColorFB(tempPoint, textLayer[coords]);
#endif
        else
          setColorFB(tempPoint, backLayer[coords]);

        WS2812_framedata_setPixel(invI, invX + (invJ * NCOLS), tempPoint[0], tempPoint[1], tempPoint[2]);
      }
    }
  }
}

void clearBack(void)
{
  for (uint8_t y = 0; y < NCOLS; y++)
  {
    for (uint8_t x = 0; x < NCOLS; x++)
    {
      setColor(backLayer[x + (y * NCOLS)], arrColor[12]);
    }
  }
}

void clearText(void)
{
#if COLOR_1BIT
  for (uint8_t i = 0; i < NLEDSBIT; i++)
  {
    textLayer[i] = 0;
  }
#else
  for (uint8_t y = 0; y < NCOLS; y++)
  {
    for (uint8_t x = 0; x < NCOLS; x++)
    {
      setColor(textLayer[x + (y * NCOLS)], arrColor[12]);
    }
  }
#endif
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
        WS2812_framedata_setPixel(i, x + (j * NCOLS), arrColor[12][0], arrColor[12][1], arrColor[12][2]);
      }
    }
  }
}

// void move(int8_t newX, int8_t newY)
// {
//   uint8_t tempVector[NLEDS][3];
//   uint8_t tempX, tempY;
//   for (int y = 0; y < NCOLS; y++)
//   {
//     for (int x = 0; x < NCOLS; x++)
//     {
//       for (int z = 0; z < 3; z++)
//       {
//         tempVector[x + (y * NCOLS)][z] = backLayer[x + (y * NCOLS)][z];
//       }
//     }
//   }

//   for (int y = 0; y < NCOLS; y++)
//   {
//     if ((y - newY) < 0)
//     {
//       tempY = NCOLS - (-y + newY);
//     }
//     else if ((y - newY) >= NCOLS)
//     {
//       tempY = (y - newY) - NCOLS;
//     }
//     else
//       tempY = y - newY;

//     for (int x = 0; x < NCOLS; x++)
//     {
//       if ((x - newX) < 0)
//       {
//         tempX = NCOLS - (-x + newX);
//       }
//       else if ((x - newX) >= NCOLS)
//       {
//         tempX = (x - newX) - NCOLS;
//       }
//       else
//         tempX = x - newX;

//       for (int z = 0; z < 3; z++)
//       {
//         backLayer[x + (y * NCOLS)][z] = tempVector[tempX + (tempY * NCOLS)][z];
//       }
//     }
//   }
// }

void drawShapeNote()
{
  uint8_t rz = NCOLS / 2;
  uint16_t coords = 0;
  uint8_t invI;
  for (int8_t y = -rz; y <= rz; y++)
  {
    for (int8_t x = -rz; x <= rz; x++)
    {
      coords = (x + rz) + ((y + rz) * NCOLS);
      invI = orientation ? MAX_DIV - 1 : 0;
      for (int8_t i = 0; i < MAX_DIV - 1; i++)
      {
        float layer = ((float)(i + 1) / MAX_DIV);
        if (callback(x, y, layer))
        {
          invI = orientation ? i : MAX_DIV - 1 - i;
          break;
        }
      }
      setColor(backLayer[coords], noteBuffer[invI].color);
      for (uint8_t z = 0, invz = 2; z < 3; z++, invz--)
      {
        textColor[z] = 150;
      }
    }
  }
}

void drawShapeNoteGrad()
{
  uint8_t rz = NCOLS / 2;
  uint16_t coords = 0;
  uint8_t invO = orientation ? 0 : 1;
  int8_t diff[3];
  for (uint8_t j = 0; j < 3; j++)
  {
    diff[j] = (noteBuffer[orientation].color[j] - noteBuffer[invO].color[j]) / MAX_DIV;
  }

  for (int8_t y = -rz; y <= rz; y++)
  {
    for (int8_t x = -rz; x <= rz; x++)
    {
      coords = (x + rz) + ((y + rz) * NCOLS);
      setColor(backLayer[coords], noteBuffer[orientation].color);
      for (int8_t i = 0; i < MAX_DIV - 1; i++)
      {
        float layer = ((float)(i + 1) / MAX_DIV);
        if (callback(x, y, layer))
        {
          for (uint8_t j = 0; j < 3; j++)
          {
            backLayer[coords][j] = noteBuffer[invO].color[j] + ((i + 1) * diff[j]);
          }
          break;
        }
      }
    }
  }
}
uint8_t circle(int8_t x, int8_t y, float layer)
{
  float z = ((float)NCOLS / 2);
  return ((x * x) + (y * y) <= (z * z) * layer);
}

uint8_t square(int8_t x, int8_t y, float layer)
{
  float z = ((float)NCOLS / 2);
  float perc = layer * z;
  return ((x >= -perc && x <= perc) && (y >= -perc && y <= perc));
}

uint8_t triangle(int8_t x, int8_t y, float layer)
{
  float z = ((float)NCOLS / 2);
  float perc = layer * z;
  float offset = z / 7;
  float m = y * 0.7 + perc;
  return ((x > -m) && (x < m)) && (y < perc * 0.9 + offset);
}

uint8_t cross(int8_t x, int8_t y, float layer)
{
  float z = ((float)NCOLS / 2);
  float perc = z - layer * z;
  return ((x <= -perc || x >= perc) && (y <= -perc || y >= perc));
}

uint8_t horizontal(int8_t x, int8_t y, float layer)
{
  float z = ((float)NCOLS / 2);
  float perc = z - layer * z;
  return ((x >= -perc || x <= perc) && (y <= -perc || y >= perc));
}

uint8_t vertical(int8_t x, int8_t y, float layer)
{
  float z = ((float)NCOLS / 2);
  float perc = z - layer * z;
  return ((x <= -perc || x >= perc) && (y >= -perc || y <= perc));
}

void drawLine(uint8_t px, uint8_t py, uint8_t *color)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      if (x == px || y == py)
      {
        setColor(backLayer[x + (y * NCOLS)], color);
      }
    }
  }
}

void drawPixel(uint8_t px, uint8_t py, uint8_t *color)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      if (x == px && y == py)
      {
        setColor(backLayer[x + (y * NCOLS)], color);
      }
    }
  }
}

void drawColor(uint8_t *color)
{
  for (int y = 0; y <= NCOLS; y++)
  {
    for (int x = 0; x <= NCOLS; x++)
    {
      setColor(backLayer[x + (y * NCOLS)], color);
    }
  }
}

void checkSequence(void)
{
  uint8_t arrayLen = ARRAY_LEN(noteSequence);
  uint8_t cnt = 0;
  // uint8_t blue[3] = {1, 1, 20};
  for (uint8_t j = 0; j < arrayLen; j++)
  {
    uint8_t len = ARRAY_LEN(noteSequence[j].note);
    for (uint8_t i = 0; i < len; i++)
    {
      if (noteBuffer[i].note.symbol == noteSequence[j].note[len - 1 - i].symbol)
      {
        cnt++;
        if (cnt == len)
        {
          clearText();
          // drawString(noteSequence[j].word, 1, 24, 1, 0);
          drawCenter(noteSequence[j].word, 1, 0);
        }
      }
      else
        break;
    }
  }
}

void drawNote(void)
{
  // uint8_t red[3] = {20, 1, 1};
  // uint8_t green[3] = {1, 20, 1};
  // drawStringArray(1, 1, 1, 1);

  checkSequence();

  // drawString(noteBuffer[0], 1, 1, 1, 0);
  // setTextColor(noteBuffer[0].color);
  // drawColor(noteBuffer[0].color);
  // drawLetter(note.symbol, 1, 10);
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
  for (uint8_t y = py, l = 0; y <= (py + 5); y++, l++)
  {
    if (y > NCOLSL)
      break;
    fill = getFontLine(letter, l);
    for (uint8_t x = px, aX = 3; x <= (px + 3); x++, aX--)
    {
      if (fill & (1u << aX))
      {
#if COLOR_1BIT
        BIT_SET(textLayer[DIV_8(x + (y * NCOLS))], REST_8(x + (y * NCOLS)));
#else
        setColor(textLayer[x + (y * NCOLS)], textColor);
#endif
      }
    }
  }
}

void drawCenter(char *frase, uint8_t spacex, uint8_t spacey)
{
  uint8_t len = strlen(frase) * (3 + spacex);
  int8_t px = (NCOLS - len) / 2;
  len = (5 + spacey);
  int8_t py = (NCOLS - len) / 2;
  px = px < 0 ? 0 : px;
  drawString(frase, px, py, spacex, spacey);
}

void drawString(char *frase, uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey)
{
  uint8_t posX, incX = 0, posY, nLine = 0;
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
  uint8_t posX, incX = 0, posY, nLine = 0;
  char temp;
  for (uint8_t j = 0; j < ARRAY_LEN(noteBuffer); j++)
  {
    setTextColor(noteBuffer[j].color);
    for (uint8_t i = 0; i < 3; i++)
    {
      posX = incX * (3 + spacex) + px;
      if (posX + ((3 + spacex)) > NCOLS - 2)
      {
        posX = px;
        nLine++;
        incX = 0;
      }
      posY = nLine * (5 + spacey) + py;
      temp = getNoteBuffer(j, i);
      if (temp)
      {
        drawLetter(temp, posX, posY);
        incX++;
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
              {
#if COLOR_1BIT
                BIT_SET(textLayer[DIV_8(x + (y * NCOLS))], REST_8(x + (y * NCOLS)));
#else
                setColor(textLayer[x + (y * NCOLS)], textColor);
#endif
              }
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