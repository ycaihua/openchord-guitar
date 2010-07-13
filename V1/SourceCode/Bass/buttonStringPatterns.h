/* buttonStringPatterns.h
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef buttonStringPatterns_h
#define buttonStringPatterns_h

#include "configAndConstants.h"
#include <avr/eeprom.h>

static inline void setEepromToDefault( void* notesAddress, void* chordsAddress)
{
	// First the defaults for Note Mode.  We're declaring our temporary storage
	// array as something big enough for all 21 chord mode combinations, but we
	// only write the first 5 patterns when we write these ones to EEPROM
	int buttonStringPatterns[NUM_CHORDS_BUTTONS][NUMBER_OF_STRINGS];

    buttonStringPatterns[0][0] = 0b0000000; // [0][0] through [0][5] - green button
    buttonStringPatterns[0][1] = 0b0001100; 
	buttonStringPatterns[0][2] = 0b0000000;	
	buttonStringPatterns[0][3] = 0b0000110;

	buttonStringPatterns[1][0] = 0b0000010; // [1][0] through [1][5] - red button
	buttonStringPatterns[1][1] = 0b0000000; 
	buttonStringPatterns[1][2] = 0b0000001;
	buttonStringPatterns[1][3] = 0b0000000;

	buttonStringPatterns[2][0] = 0b0001100; // [2][0] through [2][5] - yellow button
	buttonStringPatterns[2][1] = 0b0000000; // This is an A chord.
	buttonStringPatterns[2][2] = 0b0000110;
	buttonStringPatterns[2][3] = 0b0000000;
							  
	buttonStringPatterns[3][0] = 0b0000000; // [3][0] through [3][5] - blue button
	buttonStringPatterns[3][1] = 0b0000001; 
	buttonStringPatterns[3][2] = 0b0001100;
	buttonStringPatterns[3][3] = 0b0000000;

	buttonStringPatterns[4][0] = 0b0000000; // [4][0] through [4][5] - orange button
	buttonStringPatterns[4][1] = 0b0000110; 
	buttonStringPatterns[4][2] = 0b0000000;
	buttonStringPatterns[4][3] = 0b0000001;

	eeprom_write_block( (void*)&buttonStringPatterns, notesAddress, EEPROM_NOTES_BLOCK_SIZE);


	// Now write the default chord mapping
	buttonStringPatterns[0][0] = 0b0001110; // [0][0] through [0][5] - green button
	buttonStringPatterns[0][1] = 0b0001110; // This is a G Chord
	buttonStringPatterns[0][2] = 0b0000000;	
	buttonStringPatterns[0][3] = 0b0000000;

	buttonStringPatterns[1][0] = 0b0000000; // [1][0] through [1][5] - red button
	buttonStringPatterns[1][1] = 0b0001110; // This is a C chord
	buttonStringPatterns[1][2] = 0b0001110;
	buttonStringPatterns[1][3] = 0b0000000;

	buttonStringPatterns[2][0] = 0b0000000; // [2][0] through [2][5] - yellow button
	buttonStringPatterns[2][1] = 0b0000110; // This is an E chord.
	buttonStringPatterns[2][2] = 0b0000110;
	buttonStringPatterns[2][3] = 0b0000110;
								  
	buttonStringPatterns[3][0] = 0b0000000; // [3][0] through [3][5] - blue button
	buttonStringPatterns[3][1] = 0b0000000; // This an A chord
	buttonStringPatterns[3][2] = 0b0000110;
	buttonStringPatterns[3][3] = 0b0000110;

	buttonStringPatterns[4][0] = 0b0000000; // [4][0] through [4][5] - blue button
	buttonStringPatterns[4][1] = 0b0000000;	// This is a D chord 
	buttonStringPatterns[4][2] = 0b0000000;
	buttonStringPatterns[4][3] = 0b0001110;

	eeprom_write_block( (void*)&buttonStringPatterns, chordsAddress, EEPROM_CHORDS_BLOCK_SIZE);


	return;
}

#endif
