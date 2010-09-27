/*  OpenChord V1 AVR code for USB/PS3 *
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	Look at V1.h for important info about compiling this

	Thanks to the people at Objective Development for their open-source V-USB library
	for the AVR microcontroller. http://www.obdev.at/avrusb/

	Special thanks to Frank Zhao and his Wiimote Extension Library,
	details which can be found here:
		
	http://frank.circleofcurrent.com/content.php?page_id=wii_extension_lib
	Also, thanks to Hector Martin's work on encryption and decryption

	This program is distributed under the terms of the GNU General Public License, v3,
	and includes the wiimote extension library, originally distributed under the Apache 2.0 license

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "V1.h"

// Space in the EEPROM memory for the button pattern data for each
// mode of playing, as well as the address of the check string
int EEMEM notesModeButtonPatterns[EEPROM_NOTES_BLOCK_SIZE] = 
{
    // [0][0] through [0][5] - green button
    0b0000000, 
	0b0001100,
	0b0000000,	
	0b0000110,

	0b0000010, // [1][0] through [1][5] - red button
	0b0000000, 
	0b0000001,
	0b0000000,

	0b0001100, // [2][0] through [2][5] - yellow button
	0b0000000, // This is an A chord.
	0b0000110,
	0b0000000,
							  
	0b0000000, // [3][0] through [3][5] - blue button
	0b0000001, 
	0b0001100,
	0b0000000,

	0b0000000, // [4][0] through [4][5] - blue button
	0b0000110, 
	0b0000000,
	0b0000001,
};

int EEMEM chordModeButtonPatterns[EEPROM_CHORDS_BLOCK_SIZE] = 
{
	// Now write the default chord mapping
	0b0001110, // [0][0] through [0][5] - green button
	0b0001110, // This is a G Chord
	0b0000000,	
	0b0000000,

	0b0000000, // [1][0] through [1][5] - red button
	0b0001110, // This is a C chord
	0b0001110,
	0b0000000,

	0b0000000, // [2][0] through [2][5] - yellow button
	0b0000110, // This is an E chord.
	0b0000110,
	0b0000110,
									  
	0b0000000, // [3][0] through [3][5] - blue button
	0b0000000, // This an A chord
	0b0000110,
	0b0000110,

	0b0000000, // [4][0] through [4][5] - blue button
	0b0000000,	// This is a D chord 
	0b0000000,
	0b0001110,
};


//readF reads the frets for a single string, and returns an int acting as a boolean array
//  telling which frets are being touched by the string.
int readFrets(int guitarString, volatile uint8_t *port)
{
  	*port &= ~(1<<guitarString);  // Set the string LOW

	// Then we have to delay for a while, since right after the switch, the pin values
	// jump around a bit
	doXnops(20);	
	// Read the values on the frets
  	int i = fret_pin;               
	i = fret_pin;
	i = fret_pin;		// More delay funcitonality... without these delays, there is some bouncing
	i = fret_pin;		// on the pins, so the read screws up.
	  
	//Reset everything on the string
	int stringState = 0;	                
	// Next, since the fret pin is zero if the a fret is grounded, we do a mask to isolate each pin from 
	//  the port, which will return a zero if pressed and non-zero if unpressed. So we ! that to get a 1 if
	//  the string touches that fret and a zero if it doesn't touch, which we then load into our storage 
	//  integer.

 	stringState |= ( (!(i & (1<<green_pin))) << 1); //green fret
 	stringState |= ( (!(i & (1<<red_pin))) << 2); //red fret
 	stringState |= ( (!(i & (1<<yellow_pin))) << 3); //yellow fret
 	stringState |= ( (!(i & (1<<blue_pin))) << 4); //blue fret
 	stringState |= ( (!(i & (1<<orange_pin))) << 5); //orange fret

	// Also, if we strummed a string, hold that in the 'zeroth' fret
	i = pick_pin;
 	stringState |= ( (!(i & (1<<strum_pin))) << 0); //pick touching the string

	//Turn the string back HIGH and return
	*port |= (1<<guitarString); 
	return stringState;
}

inline void clearButtonStringPatterns(int buttonStringPatterns[][NUMBER_OF_STRINGS])
{
	buttonStringPatterns[0][0] = 0;	buttonStringPatterns[0][1] = 0;
	buttonStringPatterns[0][2] = 0;	buttonStringPatterns[0][3] = 0;
	buttonStringPatterns[1][0] = 0;	buttonStringPatterns[1][1] = 0;
	buttonStringPatterns[1][2] = 0;	buttonStringPatterns[1][3] = 0;
	buttonStringPatterns[2][0] = 0;	buttonStringPatterns[2][1] = 0;
	buttonStringPatterns[2][2] = 0;	buttonStringPatterns[2][3] = 0;
	buttonStringPatterns[3][0] = 0;	buttonStringPatterns[3][1] = 0;
	buttonStringPatterns[3][2] = 0;	buttonStringPatterns[3][3] = 0;
	buttonStringPatterns[4][0] = 0;	buttonStringPatterns[4][1] = 0;
	buttonStringPatterns[4][2] = 0;	buttonStringPatterns[4][3] = 0;
}



// This function just reads our plus and minus buttons and sets the data struct accordingly
inline void readOtherButtons(dataForController* data)
{
	// read our plus and minus buttons
	int i = misc_pin; // check for plus and minus
	data->plusOn = !(i & (1<<plus_pin)); //Plus and minus are held high normally, but pressing it drops it low
	data->minusOn = !(i & (1<<minus_pin));
}

// figureOutNote takes the data from the stringState
int figureOutNote(int stringState, int emptyStringNote)
{
	stringState &= ~0b0001; // This clears out the strum data that we don't care about.
	
	// Now, we check each fret in the order from 5 to 1.  They're chromatic, so we just add 1
	// for each number
	if (stringState & 0b100000)
		return emptyStringNote + 5;

	if (stringState & 0b010000)
		return emptyStringNote + 4;

	if (stringState & 0b001000)
		return emptyStringNote + 3;

	if (stringState & 0b000100)
		return emptyStringNote + 2;

	if (stringState & 0b000010)
		return emptyStringNote + 1;

	// If we're not pressing anything, just return the note value of the open string	
	return emptyStringNote;
}
			
int main(void)
/* This controls the whole program.
	 It works under normal mode by first initializing things then looping through the code.
	 -Under normal operation, the loop reads the values on the strings and frets, processes these
	  values against a set patterns to determine what buttons on the guitar hero controller should 
	  be set, sets those buttons in a formatted data structure, then sends that structure to 
	  the actual controller.
	 -Under config mode, the loop reads in the values of the strings and frets, assigns that current
	  string-fret combination to correspond to a button press, then locks that combination in when 
	  'Plus' is pressed and moves on to changing the next button.  It also sets the button data structure
	  to display the current controller button being programmed, and then sends that data to the controller.
*/ 
{	
	// First, set up the guitar stuff - These functions are stored in guitarInitFunctions.h/.c
	setPins();
	setTimer();	
 	
	// Now set up all the communication stuff - initialization routines
 	//  set specially in the interface functions - see ps3interface.h, wiiinterface.h, etc.
 	#ifdef MIDI_ENABLED
		startMIDICommunication();
	#endif

	#ifdef PS3_USB
		startPS3Communication();
	#endif
    
	// Declaration of variables used in Main()
	int stringState[NUMBER_OF_STRINGS] = {0,0,0,0,0,0}; // This stores an int for each string,
										// with each int acting as a boolean array
										// telling which frets a string is connected to,
										// with the 0th bit being if a string is being strummed
    int tempStringState[NUMBER_OF_STRINGS]; // Temporary place to record how the strings are
                                            // being pressed, for debouncing purposes


    int stringStateTimers[NUMBER_OF_STRINGS] = {0,0,0,0,0,0};
	
	dataForController data;
	clearData(&data); //This function is contained in configAndConstants.h

	//emptyFretNotes are the MIDI notes that the guitar's open strings are tuned to.
	int openStringNotes[NUMBER_OF_STRINGS] = {40, 45, 50, 55, 59, 64};

	noteInfo currentNotes[NUMBER_OF_STRINGS];

	for (int i = 0; i <= NUMBER_OF_STRINGS; i++)
	{
		stopNote(&currentNotes[i]);
	}

	uint16_t timer = 0; // Timer for counting loops
 					 /* main event loop */
    while(1)
	{   
		// We first read all the strings for button hits and store them into
		//  our stringState array.  Strum processing is also handled here for now.
        tempStringState[0] = readFrets(first_string, &string_1_port);
		tempStringState[1] = readFrets(second_string, &string_2_port);
		tempStringState[2] = readFrets(third_string, &string_3_port);
		tempStringState[3] = readFrets(fourth_string, &string_4_port);
		tempStringState[4] = readFrets(fifth_string, &string_5_port);
		tempStringState[5] = readFrets(sixth_string, &string_6_port);

        // Debounce the strings by making sure what is being fretted
        // remains for a certain number of cycles through the main loop
        for (int i = 0; i < NUMBER_OF_STRINGS; i++)
        {
            if (tempStringState[i] == stringState[i])
                stringStateTimers[i] = 0;
            else
                stringStateTimers[i] += 1;
            // If we've seen enough cycles of a new string state, update stringState 
            if (stringStateTimers[i] >= STRING_DEBOUNCE_DELAY)
            {
                stringState[i] = tempStringState[i];
                stringStateTimers[i] = 0;
            }
        }
		
		// Next, we reset the data struct so we have a fresh place to store things
		clearData(&data);  // This function is in V1Typedefs.h

		// Now read our other buttons, plus and minus.
		readOtherButtons(&data);

		// Next, using our strumState array, we process that data to figure out what sort
		//  of notes we want to be playing, or not playing.

		for (int string = NUMBER_OF_STRINGS - 1; string >= 0; string--)// NUMBER_OF_STRINGS; string++) // Each string can only be playing one note
		{
			noteInfo stringNote = currentNotes[string];

			// First, we decrement the countdown of all notes, so each note lasts only so long
			// and turn off the note if the countdown is over
			if (stringNote.countdown > 0)
			{
				stringNote.countdown--;
				if (stringNote.countdown == 0)
				{
					stringNote.velocity = 0;
					stringNote.newNoteFlag = 1; //set the changed note flag
				}
			}

			// Next, we cancel any note that isn't being fretted anymore
			if (stringNote.note != figureOutNote(stringState[string], openStringNotes[string]) )
			{
				stringNote.note = figureOutNote(stringState[string], openStringNotes[string]);
				stringNote.velocity = 0;
				stringNote.countdown = 0;
				stringNote.newNoteFlag = 1; //set the changed note flag
			}

			// Now, we trigger a note if it is strummed
			if (stringState[string] & 0b0001)  // This means that it's being strummed
			{
				stringNote.pickFlag = 1; // set this flag to tell us we were plucking the string
			}
			else if ( stringNote.pickFlag == 1 ) // Not currently strummed, but just released
			{
				stringNote.note = figureOutNote(stringState[string], openStringNotes[string]);
				stringNote.velocity = DEFAULT_VELOCITY;
				stringNote.countdown = NOTE_COUNTDOWN;
				stringNote.newNoteFlag = 1; //set the changed note flag since we're no longer plucking the string
				stringNote.pickFlag = 0;
			}


			// Make sure that we write our current note back into the array
			currentNotes[string] = stringNote;
		}

		// Now we send the note data to the computer
	#ifdef MIDI_ENABLED

		for (int string = NUMBER_OF_STRINGS - 1; string >= 0; string--)// NUMBER_OF_STRINGS; string++)//NUMBER_OF_STRINGS; string++) // Each string can only be playing one note
		{
			if (currentNotes[string].newNoteFlag == 1)
			{
				sendMIDINote(currentNotes[string].note, currentNotes[string].velocity, MIDI_CHANNEL);
				//Clear the note flag
				currentNotes[string].newNoteFlag = 0;
			}
			else // keep the USB connection alive
			{
				wdt_reset();
				usbPoll();
			}
		}
	#endif



		
		// Test code to see if the notes are working out well
		for (int string = 0; string < NUMBER_OF_STRINGS; string++)//NUMBER_OF_STRINGS; string++) // Each string can only be playing one note
		{	
			if (currentNotes[string].newNoteFlag == 1)
			{
				data.numberOfStringsPressed = currentNotes[string].note * 2;
				if (currentNotes[string].countdown > 1)
					data.blueOn = 1;
			}
		}

		// Big function for testing stringState visually
		//data.blueOn   = !!(stringState[0] & 0b00100000);
		data.greenOn  = !!(stringState[0] & 0b00010000);
		data.redOn    = !!(stringState[0] & 0b00001000);
		data.yellowOn = !!(stringState[0] & 0b00000100);
		data.orangeOn = !!(stringState[0] & 0b00000010);
		data.minusOn  = stringState[0] & 0b00000001;

//		processStringState(&data, stringState, buttonStringPatterns, controllerMode, greenTimers, triedGreen);
		// Now our processing is complete, so using those button presses, we set up the 
		// button data packet. This function depends on the console and is 
		// set specially in the interface functions - see ps3interface.h, wiiinterface.h, etc.

	#ifdef PS3_USB
		  sendPS3Data(data);
	#endif
		//test();

	}// End of while loop
    return 0;
}


