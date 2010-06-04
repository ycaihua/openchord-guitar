/* Wiimote Extension Library - wiimote.h
	Frank Zhao
	Copyright 2009

	http://frank.circleofcurrent.com/
	
	Distributed under the Apache 2.0 License, 
	text of which can be found at http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef wiimote_h

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "../pindef.h"

// initialize wiimote interface with id, starting data, and calibration data - I got rid of the function call option
void wm_init(const unsigned char *, unsigned char *, const unsigned char *, void (*)(void));

// set button data
void wm_newaction(unsigned char *);

#define wiimote_h
#endif

