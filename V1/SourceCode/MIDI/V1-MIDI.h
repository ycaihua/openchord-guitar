/*  OpenChord V1 AVR code for USB/PS3 *
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	For important information about compiling, see AVR

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

#ifndef V1_h
#define V1_h

#include <avr/eeprom.h>
#include <string.h>
#include <avr/io.h>
#include "configAndConstants.h" // This needs to be here in order to overwrite the 
								// configAndConstants.h that guitarInitFunctions will try and call
								// Sloppy, I know.
#include "../guitarInitFunctions.h"
#include "../pindef.h"

#ifdef MIDI_ENABLED
	#include "MIDIInterface.h"
#endif

#ifdef PS3_USB
	 #include "../PS3/ps3interface.h"
#endif

#ifdef WII
	#include "../Wii/wiiinterface.h"
#endif

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega88P__) ||	defined(__AVR_ATmega44P__)
#error Right now, V-USB does not work right for the P series chips - compile for the ATmega168 and then use that binary.
#endif



#endif
