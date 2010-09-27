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

	// This determines how long each note plays, if not interrupted.
	#define NOTE_COUNTDOWN 2000
	// What MIDI channel we're sending data on
	#define MIDI_CHANNEL 0
	// Standard velocity with which to send the notes at
	#define DEFAULT_VELOCITY 200

/* ---------------- The following definitions control what files get 
                     included and what system and chip the program gets built for.  
					 These should be changed to reflect what you're doing. ----------*/

	#define MIDI_ENABLED
	//#define PS3_USB
	//#define WII

	
	/* You should be using GENERAL_CIRCUIT_BOARD unless you have one of the first prototypes.
	    The other options are for when we were using different chips and different boards for
	    the Wii and USB options. */

	#define GENERAL_CIRCUIT_BOARD
	//#define ATMEGA164
	//#define ATMEGA168


/* ---------------- Lesser constants, generally ignorable -----------------*/

	#define GREEN_TIMER_WAIT 100 // This controls the delay for the green button.
					// There is a delay in order to avoid triggering green when red is played
					// but the green fret is hit first, so you don't press red but see the green
	
	#define STRING_DEBOUNCE_DELAY 10 // How many cycles to ignore a changed string state before
                                     // updating the string state array. This hopefully helps with
                                     // the pick and grounding it with your fingers - 
                                     // Caution - too high of a value, and spurious hits will start to
                                     // cancel out acutal hits.

	#define DEBOUNCE_DELAY 1000 // Just a number to control our debouncing of the plus button

	#define NUM_NOTES_BUTTONS 5   // There are 5 buttons programmable in Note Mode, 1 for each color
	#define NUM_CHORDS_BUTTONS 10   // There are 9 button combinations programmable in Chord Mode
									// covers all single and double button combinations
									// that can show up in Medium Difficulty

	#define NUMBER_OF_STRINGS 6 // How many strings on the instrument are wired to the chip.
								// If you change this, you'll have to also make some changes in
								// other files to make the string tests make sense.  Find
								// these instances by setting this to one, and you'll see all the
								// places where there are hard-coded things that need changing -
								// You'll get a bunch of warnings about out-of-bounds array indexes -
								// These are the places where you'll need to add or delete code.
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


#ifndef __ASSEMBLER__  // For some reason, the assembler freaks out when it hits these typedefs.
						// Therefore, we make the assembler ignore them.

	// This struct stores abstract data about what note is currently being held down, how long
	// it's been held down for, and other things.
	typedef struct noteInfo
	{
		char note;  // Note number
		char velocity; // Note velocity
		int16_t countdown; // A countdown timer to turn the notes off after a certain time 
		char newNoteFlag; // Is this a newly active note info packed
		char pickFlag;   // A flag for if the guitar is being picked
	} noteInfo;

	inline void stopNote(noteInfo* note)
	{
		note->countdown = 0;
	};

	// This struct is used to store abstract data about what the controller's state is.
	// It gets filled by the functions in main.c and then gets passed to the ___interface.c files
	typedef struct dataForController
	{
		char greenOn;  // variables to abstractly tell us which buttons are pressed		
		char redOn;
		char yellowOn;
		char blueOn;
		char orangeOn;
		char upOn;
		char downOn;
		char plusOn;
		char minusOn;
		char numberOfStringsPressed;
		char homeOn;
		char whammy;
	} dataForController;

	inline void clearData(dataForController* data)
	{
		data->orangeOn = 0;
		data->blueOn   = 0;
		data->yellowOn = 0;
		data->redOn    = 0;
		data->greenOn  = 0;
		data->upOn     = 0;
		data->downOn   = 0;
		data->plusOn   = 0;
		data->minusOn  = 0;
		data->numberOfStringsPressed = 0;
		data->homeOn   = 0;
		data->whammy   = 0;
	};

	// Macro for No Operation assembly instructions
	#define nop() asm volatile("nop")

	// Macro to as many NOPs as we want
	inline void doXnops(int x)
	{
		while (x >= 0)
		{
			nop();
			x--;
		}
	}
#endif

#endif

