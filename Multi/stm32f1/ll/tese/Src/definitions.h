#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#define USART_MIDI 1

#define SMALL_BOARD 1
#define MEDIUM_BOARD 2
#define BIG_BOARD 3

#define BOARD MEDIUM_BOARD

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))

#define MAX_DIV 5
#define MAX_BRIGHT 0.1 // 0 - 1

#if BOARD == SMALL_BOARD
#define NCHANNELS 4
#define NCOLS 7
#define NLINESCH 2
#define MIRROR_X 0
#define MIRROR_Y 0
#define REVERSE_ORIENTATION 0
#elif BOARD == MEDIUM_BOARD
#define NCHANNELS 8
#define NCOLS 16
#define NLINESCH 2
#define MIRROR_X 1
#define MIRROR_Y 0
#define REVERSE_ORIENTATION 1
#elif BOARD == BIG_BOARD
#define NCHANNELS 12
#define NCOLS 45
#define NLINESCH 4
#define MIRROR_X 0
#define MIRROR_Y 0
#define REVERSE_ORIENTATION 1
#endif

#define NLEDSCH NCOLS *NLINESCH
#define NCOLSL NCOLS - 1
#define NLEDS NCOLS *NCOLS
#define NLEDSL NLEDS - 1

#define DIV_8(a) ((a) / 8)
#define REST_8(a) ((a) % 8)
#define NLEDSMOD REST_8(NLEDS)
#if NLEDSMOD
#define NLEDSBIT DIV_8(NLEDS) + 1
#else
#define NLEDSBIT DIV_8(NLEDS)
#endif

uint8_t backLayer[NLEDS][3];
uint8_t textLayer[NLEDSBIT];

typedef struct Notes
{
  char symbol[3];
  char octave;
  char note[4];
} Note;
Note note;
extern char noteBuffer[10][5];

extern const uint8_t arrColor[13][3];

extern volatile uint8_t backColor[3];
extern uint8_t textColor[3];

void setBackColor(uint8_t *color);

extern const uint8_t font4x6[96][2];

#endif