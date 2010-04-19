/* guitarInitFunctions.h
     Copyright 2009 
	   Alan Chatham

	For more details about copyright, liscense, etc., see V1.h

	This header declares the functions that the guitar uses during initialization.
	These functions mostly set up pins and internal registers in the chip.
*/
#ifndef GUITAR_INIT_FUNCTIONS
#define GUITAR_INIT_FUNCTIONS

#include <avr/io.h>
#include "pindef.h"

// setPins sets up the pins on the microcontroller
//  for proper IO, etc.
void setPins(void);

//This sets up a timer to handle the debouncing of the green string
// when we're playing with frets. The controller needs to wait when
// a player presses just the first fret in order to make sure that
// it's not a second fret press that just happened to contact the
// first fret first.
void setTimer(void);

// This function sets the input and output pins correctly


#endif
