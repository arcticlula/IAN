#ifndef __DRAW_H
#define __DRAW_H

void fillFramebuffer(void);
void clearFramebuffer(void);
void clearBack(void);
void clearText(void);

// void move(int8_t newX, int8_t newY);
// void rotate(void);
typedef uint8_t (*callback_type)(int8_t x, int8_t y, float layer);

extern uint8_t orientation;
extern uint8_t MAX_DIV;
extern callback_type callback_function;

void drawColor(uint8_t *color);
void drawPixel(uint8_t px, uint8_t py, uint8_t *color);
void drawLine(uint8_t px, uint8_t py, uint8_t *color);
void drawShapeNote(void);
void drawShapeNoteGrad(void);

uint8_t circle(int8_t x, int8_t y, float layer);
uint8_t square(int8_t x, int8_t y, float layer);
uint8_t cross(int8_t x, int8_t y, float layer);
uint8_t horizontal(int8_t x, int8_t y, float layer);
uint8_t vertical(int8_t x, int8_t y, float layer);

void drawNote(void);
void drawCenter(char *frase, uint8_t spacex, uint8_t spacey);

// unsigned char getFontLine(unsigned char data, int line_num);
void drawLetter(char letter, uint8_t px, uint8_t py);
void drawString(char *frase, uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey);
void drawStringArray(uint8_t px, uint8_t py, uint8_t spacex, uint8_t spacey);
void drawScrollingString(char *frase, uint16_t interval_ms, uint8_t space, uint8_t offX, uint8_t offY);

#endif