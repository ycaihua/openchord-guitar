/*  MIDIInterface.h
   part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/
#ifndef MIDI_INTERFACE
#define MIDI_INTERFACE

#include "configAndConstants.h"
#include "pindef.h"
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbconfig.h"
#include "usbdrv/usbdrv.h"

#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */


// setupMIDI sets up the MIDI interface
void startMIDICommunication(void);

// sendMIDINote outputs a single note via the USB MIDI interface
void sendMIDINote(int note, int velocity, int channel);

// silenceAllMIDINotes sends a signal to turn off all notes
void silenceAllMIDINotes(void);

// Test function that outputs a MIDI beeping
void test(int note, int velocity);


#endif
