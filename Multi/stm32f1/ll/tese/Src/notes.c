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

/* void setNote(int nt)
{
  resto = nt % MIDI_OCTAVE;
  note.octave = (nt / MIDI_OCTAVE) + '0';

  setTextColor(arrColor[resto]);
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
    strcpy(noteBuffer[k].note, noteBuffer[k - 1].note);
  }
  sprintf(noteBuffer[0].note, "%s%c", note.note, ' ');
  // setBackColor(arrColor[resto]);
} */

void addNoteBuffer(uint8_t *color)
{
  for (uint8_t k = ARRAY_LEN(noteBuffer); k > 0; k--)
  {
    noteBuffer[k] = noteBuffer[k - 1];
  }
  noteBuffer[0].note = note;
  setColor(noteBuffer[0].color, color);
}

void setNote(int nt)
{
  resto = nt % MIDI_OCTAVE;
  note.octave = (nt / MIDI_OCTAVE) + '0';
  note.pitch = '\0';
  // setTextColor(arrColor[resto]);
  switch (resto)
  {
  case MIDI_C0:
    note.symbol = 'C';
    break;
  case MIDI_C0 + MIDI_SHARP:
    note.symbol = 'C';
    note.pitch = '#';
    break;
  case MIDI_D0:
    note.symbol = 'D';
    break;
  case MIDI_E0 + MIDI_FLAT:
    note.symbol = 'E';
    note.pitch = 'b';
    break;
  case MIDI_E0:
    note.symbol = 'E';
    break;
  case MIDI_F0:
    note.symbol = 'F';
    break;
  case MIDI_F0 + MIDI_SHARP:
    note.symbol = 'F';
    note.pitch = '#';
    break;
  case MIDI_G0:
    note.symbol = 'G';
    break;
  case MIDI_A0 + MIDI_FLAT:
    note.symbol = 'A';
    note.pitch = 'b';
    break;
  case MIDI_A0:
    note.symbol = 'A';
    break;
  case MIDI_B0 + MIDI_FLAT:
    note.symbol = 'B';
    note.pitch = 'b';
    break;
  case MIDI_B0:
    note.symbol = 'B';
    break;
  }
  addNoteBuffer(arrColor[resto]);
}

char getNoteBuffer(uint8_t index, uint8_t field)
{
  switch (field)
  {
  case 0:
    return noteBuffer[index].note.symbol;
  case 1:
    return noteBuffer[index].note.pitch;
  case 2:
    return noteBuffer[index].note.octave;
  default:
    return '\0';
  }
}

void clearNoteBuffer(void)
{
  uint8_t i;
  for (uint8_t z = 0; z < ARRAY_LEN(noteBuffer); z++)
  {
    noteBuffer[z].note.symbol = '\0';
    noteBuffer[z].note.pitch = '\0';
    noteBuffer[z].note.octave = '\0';
    for (i = 0; i < 3; i++)
    {
      noteBuffer[z].color[i] = 0;
    }
  }
}