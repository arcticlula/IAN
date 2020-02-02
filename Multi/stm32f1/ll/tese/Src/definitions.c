#include "stdint.h"
#include "definitions.h"

void setBackColor(uint8_t *color)
{
  for (uint8_t z = 0; z < 3; z++)
  {
    backColor[z] = color[z];
  }
}