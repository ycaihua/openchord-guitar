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

void setupADC() //Sets up the ADC 
{
	//Set up the ADC
	// Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz 
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
    ADMUX |= (1 << REFS0); // Set ADC reference to AVCC 
    ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading 
    // Using only ADC3 (PORTC, pin 3)
	ADMUX |= 0b0011;

    ADCSRB &= ~0b00000111;  // Set ADC to Free-Running Mode 

    ADCSRA |= (1 << ADEN);  // Enable ADC  
    ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
} // end of setupADC

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

static inline void switchPlayModes(int stringState[], int buttonStringPatterns[][NUMBER_OF_STRINGS], char* controllerMode)
{
	// Test for the right keypress to start the mode
	if (testForSwitchModes(stringState))
	{
		if (*controllerMode % 2 == 0) // This means that the mode is a full mode
			{++*controllerMode;}  //This puts it into a transitional mode
			                      // This is a cheap way of button debouncing.
	}
	// And if we're not pressing that anymore, switch from the transitional mode to the regular mode,
	//  resetting the button patterns as necessary
	else if(testForNoStringsPressed(stringState)) 
	{
		if (*controllerMode == CHORDS_TO_FRETS) // since these modes are just numbers, we need to wrap around
			{*controllerMode = FRETS;}

		// Now, reset the buttonStringPattern array to be appropriate for the mode
		// For this, we'll read it from the EEPROM memory.
		else if (*controllerMode  == FRETS_TO_NOTES)
		{
			// Notice, we're already passing in buttonStringPatterns as an address, so the EEPROM read looks a bit strange
			eeprom_read_block( (void*)buttonStringPatterns, (void*)&notesModeButtonPatterns, EEPROM_NOTES_BLOCK_SIZE);
			*controllerMode = NOTES;
		}
		else if (*controllerMode  == NOTES_TO_CHORDS)
		{
			// Notice, we're already passing in buttonStringPatterns as an address, so the EEPROM read looks a bit strange
			eeprom_read_block( (void*)buttonStringPatterns, (void*)&chordModeButtonPatterns, EEPROM_CHORDS_BLOCK_SIZE);
			*controllerMode = CHORDS;
		}	
	}
}

// This function just reads our plus and minus buttons and sets the data struct accordingly
inline void readOtherButtons(dataForController* data)
{
	// read our plus and minus buttons
	int i = misc_pin; // check for plus and minus
	data->plusOn = !(i & (1<<plus_pin)); //Plus and minus are held high normally, but pressing it drops it low
	data->minusOn = !(i & (1<<minus_pin));
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
 	startPS3Communication();
 	startWiiCommunication();
    
	// Declaration of variables used in Main()
	int greenTimers[NUMBER_OF_STRINGS] = {0,0,0,0}; //These give timing to properly read green buttons on each string.
	int triedGreen[NUMBER_OF_STRINGS] = {0,0,0,0};  // Also used for the green button timing thing
	int stringState[6] = {0,0,0,0,0,0}; // This stores an int for each string,
										// with each int acting as a boolean array
										// telling which frets a string is connected to,
										// with the 0th bit being if a string is being strummed
    int tempStringState[NUMBER_OF_STRINGS]; // Temporary place to record how the strings are
                                            // being pressed, for debouncing purposes


	int buttonStringPatterns[NUM_CHORDS_BUTTONS][NUMBER_OF_STRINGS]; //This holds the button patterns the
																	 // controller compares the string presses
																	 // to in order to judge if a button is being
																	 // pressed. It's the size of all the chord combinations,
																	 // but in Note Mode, only the first 5 blocks get looked at. 
    int stringStateTimers[NUMBER_OF_STRINGS] = {0,0,0,0};

	char configMode = 0; // Start in normal mode
	int buttonToAssign = 0; // declare some variables
	char plusLock = 0;
	char previousPlusOn = 0; // This is used as a lock so we don't program all the buttons at once
	int debounceTimer = 0; // This is used as a global timer for debouncing buttons
	char controllerMode = FRETS;
	
	dataForController data;
	clearData(&data); //This function is contained in configAndConstants.h

 					 /* main event loop */
    while(1)
	{   
		// We first read all the strings for button hits and store them into
		//  our stringState array.  Strum processing is also handled here for now.
        tempStringState[0] = readFrets(first_string, &string_1_port);
		tempStringState[1] = readFrets(second_string, &string_2_port);
		tempStringState[2] = readFrets(third_string, &string_3_port);
		tempStringState[3] = readFrets(fourth_string, &string_4_port);

        // Debounce the strings by making sure what is being fretted
        // remains for a certain number of cycles through the main loop
        for (int i = 0; i <= NUMBER_OF_STRINGS; i++)
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

		// Try and see if we're resetting the controller
		if(testForResetButtonPatterns(stringState))
		{
			//if so, reset the button mappings to default and the play mode to chords.
			controllerMode = FRETS;
			setEepromToDefault(&notesModeButtonPatterns, &chordModeButtonPatterns); // Stored in buttonStringPatterns.h
		}
		
		// Test if we're trying to hit the Home button
		if(testForHomeChord(stringState))
		{
			data.homeOn = 1;
		}

		// Check to see if we're trying to enter Config Mode;
		// disabled if we're playing with frets
		if ((controllerMode == NOTES) || (controllerMode == CHORDS))
		{
			if (configMode == 0)
			{
				configMode = testForConfigMode(stringState);
				
				// If we're playing with notes, when we enter config mode, we need to clear the 
				//  button string patterns once so we can rebuild them.
				if (configMode == 1 && controllerMode == NOTES)
				{
					clearButtonStringPatterns(buttonStringPatterns);
				}
				buttonToAssign = 0; // This variable keeps track of which button we're assigning
				plusLock = 0; // This is a tool to let us test that in the prior cycle, plus was or wasn't on
			}

			// Config Mode operation
			if (configMode == 1) 
			{
				// Allow us to escape config mode by a string combination
				if (testForEndConfigMode(stringState))
					{
						// Now save this new button mapping to the EEPROM 
						// Since we're already passing in buttonsStringPatterns
						// as a pointer, we don't use the '&' operator
						if (controllerMode == NOTES)
							eeprom_write_block( (void*)buttonStringPatterns, (void*)&notesModeButtonPatterns, EEPROM_NOTES_BLOCK_SIZE);
						else if (controllerMode == CHORDS)
							eeprom_write_block( (void*)buttonStringPatterns, (void*)&chordModeButtonPatterns, EEPROM_CHORDS_BLOCK_SIZE);
		
						configMode = 0;
					}
				else
				{
					// Now set up the data packet to the interface layer to say we're pressing whatever button we're currently
					// assigning, and if we're pressing 'plus', go ahead and lock that string combination in
					//  The function itself can be found in processStringState.h/.c
					configSetButtonData(&data, &buttonToAssign, buttonStringPatterns, stringState, controllerMode,
											&plusLock, &previousPlusOn, &debounceTimer);
					
					data.plusOn = 0; // Make sure we're not actually sending a "plus" signal to the controller, though.
									// If we were, it would pause the game.

					// If we've assigned allcolors in Notes mode, cycle back to programming Green 
					if (controllerMode == NOTES && buttonToAssign >= NUM_NOTES_BUTTONS && plusLock == 0 )
					{
						buttonToAssign = 0;
					}
					
					// Or if we've programmed all 10 notes in Chord mode, exit programming mode
					if (controllerMode == CHORDS && buttonToAssign >= NUM_CHORDS_BUTTONS && plusLock == 0 )
					{
						// Now save this new button mapping to the EEPROM 
						// Since we're already passing in buttonsStringPatterns
						// as a pointer, we don't use the '&' operator
						eeprom_write_block( (void*)buttonStringPatterns, (void*)&chordModeButtonPatterns, EEPROM_CHORDS_BLOCK_SIZE);
						configMode = 0;
					}
		}	}	}	 // End of Config Mode code

		
		// Normal operation
		if (configMode == 0)
		{
			//Test if Minus is being played by the chord instead of the button
			// This means it won't get read in config mode, which is good.
			if(testForMinusChord(stringState))
				data.minusOn = 1;
		
			// Update controllerMode to see if we're trying to change controller modes
			//  If we are, this changes the buttonStringPatterns as well to match
			//  correctly with the modes - this function is here in main.c
			switchPlayModes(stringState, buttonStringPatterns, &controllerMode);

			// Next, using our strumState array, we process that data to figure out what sort
			//  of buttons on the controller we want to emulate pressing.
			// find out if we're in a transitional controller mode or not
			if (!(controllerMode % 2)) // Not a transitional mode
			{	//  This function can be found in processStringState.h/.c
				processStringState(&data, stringState, buttonStringPatterns, controllerMode, greenTimers, triedGreen);
			}
			else
			{   // Now, if we're in a transitional controller mode, we'll forget that data and substitue 
				// visual cue data to show that we're changing to a specific mode
				//  The function itself can be found in processStringState.h/.c
				displayTransitionState(&data, controllerMode);
			}

			//Finally, we'll measure our ADC for the whammy bar
			data.whammy = ADCH;
		}
			    
		// Now our processing is complete, so using those button presses, we set up the 
		// button data packet. This function depends on the console and is 
		// set specially in the interface functions - see ps3interface.h, wiiinterface.h, etc.
		sendPS3Data(data);
		sendWiiData(data);

	}// End of while loop
    return 0;
}

/* ---------Unused debug code - can be put in to the main function and cause an LED to blink
             off of a pin


// Debugging LED EEPROM holding location - put above the main() function
uint8_t EEMEM nonvolitileString[10];
			 

	//Debug stuff - the following goes before the while loop in main()
	int timer;
    DDRB |= 1;
  // set PORTB for output
  
  uint8_t string[1];
  uint8_t x = 10;

  eeprom_read_block( (void*)&string, (void*)&nonvolitileString, 1);
  if (string[0] == 50)
  {
  	  x = 250;
  	  eeprom_write_block( (void*)&x, (void*)&nonvolitileString, 1);
  }
  else
  {
  	  x = 50;
  	  eeprom_write_block( (void*)&x, (void*)&nonvolitileString, 1);
  }
  timer = x;
			 
		// Debug stuff again - call it in the main while loop
		if (timer == 0)
		{
			timer = x;
			PORTB |= 1;  // turn on Port B pin 0
		}
		else
		{
			--timer;
			if (timer == 50)
				PORTB &= (~1); // turn off Port B pin 0
		}			 
	End of debug code chunks---------------------------------------------------------------- */


