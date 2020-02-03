#include "stdint.h"
#include "definitions.h"

void setBackColor(uint8_t *color)
{
  for (uint8_t z = 0; z < 3; z++)
  {
    backColor[z] = color[z];
  }
}

void setTextColor(uint8_t *color)
{
  for (uint8_t z = 0; z < 3; z++)
  {
    textColor[z] = color[z];
  }
}

void setColor(uint8_t *dest, uint8_t *color)
{
  for (uint8_t z = 0; z < 3; z++)
  {
    dest[z] = color[z];
  }
}

void setColorFB(uint8_t *dest, uint8_t *color)
{
  for (uint8_t z = 0; z < 3; z++)
  {
    dest[z] = color[z] * MAX_BRIGHT;
  }
}