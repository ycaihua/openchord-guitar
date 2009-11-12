/*configAndConstants.h
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This file contains 2 things -
	 -Most importantly, the options for proper compilation. It allows you to choose
	   whether you're compiling the code for the PS3/USB or the Wii. Change these depending
	   on what system you're building your guitar for.

     -Not as important, it holds certain constants used throughout the program - sizes,
	   names for modes, delay times, etc. These probably shouldn't be messed with.


	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* ---------------- The following definitions control what files get 
                     included and what system and chip the program gets built for.  
					 These should be changed to reflect what you're doing. ----------*/

	#define PS3_USB
	//#define WII

	
	/* You should be using GENERAL_CIRCUIT_BOARD unless you have one of the first prototypes.
	    The other options are for when we were using different chips and different boards for
	    the Wii and USB options. */

	//#define GENERAL_CIRCUIT_BOARD
	//#define ATMEGA164
	#define ATMEGA168


/* ---------------- Lesser constants, generally ignorable -----------------*/

	#define GREEN_TIMER_WAIT 100 // This controls the delay for the green button.
					// There is a delay in order to avoid triggering green when red is played
					// but the green fret is hit first, so you don't press red but see the green
					// button flash for an instant before red activates

	#define DEBOUNCE_DELAY 1000 // Just a number to control our debouncing of the plus button

	#define NUM_NOTES_BUTTONS 5   // There are 5 buttons programmable in Note Mode, 1 for each color
	#define NUM_CHORDS_BUTTONS 10   // There are 9 button combinations programmable in Chord Mode
									// covers all single and double button combinations
									// that can show up in Medium Difficulty
	#define NUMBER_OF_STRINGS 6 // and how many strings on the guitar are wired to the chip
	#define EEPROM_NOTES_BLOCK_SIZE (NUM_NOTES_BUTTONS * NUMBER_OF_STRINGS) * sizeof(int)
	#define EEPROM_CHORDS_BLOCK_SIZE (NUM_CHORDS_BUTTONS * NUMBER_OF_STRINGS) * sizeof(int)

	// These are labels to tell us which mode we're in
	#define FRETS			0
	#define FRETS_TO_NOTES  1
	#define NOTES 			2
	#define NOTES_TO_CHORDS 3
	#define CHORDS 			4
	#define CHORDS_TO_FRETS 5
	#define FRETS_HIGH      6




#endif

