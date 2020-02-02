#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "definitions.h"
#include "notes.h"

volatile uint8_t cntMessage = 0;
volatile uint8_t waitForMessage = 0;
volatile uint8_t resto = 12;

uint8_t velocity = 0;

void calculateLeds(void)
{
  for (uint8_t z = 0; z < 3; z++)
  {
    backColor[z] = ((arrColor[resto][z] * (velocity + 1)) / 255) * MAX_BRIGHT;
  }
}

void setVelocity(uint8_t vel)
{
  velocity = vel * 2;
}

void setNote(int nt)
{
  resto = nt % MIDI_OCTAVE;
  note.octave = (nt / MIDI_OCTAVE) + '0';
  for (uint8_t i = 0; i < 3; i++)
  {
    textColor[i] = arrColor[resto][i];
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
  setBackColor(arrColor[resto]);
}