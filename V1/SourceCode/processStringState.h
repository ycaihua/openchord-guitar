/*processStringState.h
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef PROCESS_STRING_STATE_h
#define PROCESS_STRING_STATE_h

#include "configAndConstants.h"
#include <avr/io.h>

/* processStringState is used to take in the data from the strings
    and output the abstract state of the controller - i.e. what buttons are being pressed
  It takes in:
  		stringState - data from the strings
		buttonStringPatterns - an array that holds patterns
								 of notes and chords to match 
								 the string data against
        controllerMode - a number that tells us what mode (frets, notes, or chords)
		                       the controller is in, so we know how to process the data
		greenTimers - an array used by fret mode to debounce the green button
		triedGreen -  another array to help with the same problem
	
	It changes the data struct by reference to reflect the abstract state
	of the controller
		data - a struct definec in V1Typedefs.h that holds variables about 
		        what buttons are being pressed
*/					                                          
void processStringState(dataForController* data, int stringState[NUMBER_OF_STRINGS],
									int buttonStringPatterns[][NUMBER_OF_STRINGS], char controllerMode, 
									int greenTimers[NUMBER_OF_STRINGS], int triedGreen[NUMBER_OF_STRINGS]);

/* displayTransitionState gets called when the controller is switching between
     modes.  It takes in the controller mode and sets the
     data struct to certain patterns to give the user feedback
     as to what state is being pressed.
*/
void displayTransitionState(dataForController* data, int controllerMode);

/* configSetButtonData is used when the controller is in button programming mode.
      It provides the user feedback as to what color button they are currently programming
    Like processStringState, it similarly sets up the data struct, but it does so based on what is 
	  being programmed in the chip.
    It takes in:
		buttonToAssign - a number telling us what button we're programming right now
		stringState - same as above, used to set up the string patterns
        controllerMode - same as above
		plusLock - used to prevent programming all buttons at once
    and it changes
	    data - same as above
		buttonStringPatterns - same as above, only this time it's an output that's
		                        getting set by the function
		plusLock - also gets written to
*/
void configSetButtonData(dataForController* data, int* buttonToAssign, int buttonStringPatterns[][6], 
						int stringState[], char controllerMode, char* plusLock, char* previousPlusOn, int* debounceTimer);


#endif
