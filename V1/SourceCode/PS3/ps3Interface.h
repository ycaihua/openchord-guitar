/*  interface.h
   part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/


#ifndef PS3INTERFACE
#include "../configAndConstants.h"
#include "../pindef.h"
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbconfig.h"
#include "usbdrv/usbdrv.h"

#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#ifndef PS3_USB
  #error "You're trying to compile for PS3/USB, but PS3_USB isn't defined in configAndConstants.h"
#endif

// This function does all the communication initialization
void startCommunication(void);

// This one goes ahead and changes the data
void sendData(dataForController);


#define PS3INTERFACE
#endif
