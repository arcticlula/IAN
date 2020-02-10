#ifndef __DRAW_H
#define __DRAW_H

void fillFramebuffer(void);
void clearFramebuffer(void);
void clearBack(void);
void clearText(void);

// void move(int8_t newX, int8_t newY);
// void rotate(void);

void drawColor(uint8_t *color);
void drawPixel(uint8_t px, uint8_t py, uint8_t *color);
void drawLine(uint8_t px, uint8_t py, uint8_t *color);
void drawCircleNote(uint8_t orientation);
void drawCircleNoteGrad(uint8_t orientation);
void drawNote(void);
void drawCenter(char *frase, uint8_t spacex, uint8_t spacey);

// unsigned char getFontLine(unsigned char data, int line_num);
void drawLetter(char letter, uint8_t px, uint8_t py);
void drawString(char *frase, uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey);
void drawStringArray(uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey);
void drawScrollingString(char *frase, uint16_t interval_ms, uint8_t space, uint8_t offX, uint8_t offY);

#endif