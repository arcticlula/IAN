#ifndef __NOTES_H
#define __NOTES_H

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

extern volatile uint8_t cntMessage;
extern volatile uint8_t waitForMessage;
extern volatile uint8_t resto;

extern uint8_t velocity;

typedef struct Notes
{
    char symbol;
    char pitch;
    char octave;
} Note;

typedef struct notesBuffer
{
    Note note;
    uint8_t color[3];
} NoteArray;

typedef struct notesWords
{
    Note note[1];
    // Note note[3];
    char word[10];
} NoteWord;

Note note;
NoteWord noteSequenceWTECS[5];
uint8_t noteSequenceSize;
NoteWord noteSequenceOET[6];
NoteWord *noteSequence;
NoteArray noteBuffer[10];

void calculateLeds(void);
void setVelocity(uint8_t vel);
void setNote(int nt);
char getNoteBuffer(uint8_t index, uint8_t field);
void clearNoteBuffer(void);

#endif