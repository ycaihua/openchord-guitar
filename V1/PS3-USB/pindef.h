/* pindef.h
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef PINDEF_H
#define PINDEF_H

#include "configAndConstants.h"

#if defined PS3_USB
	#include "PS3/ps3interface.h"
#elif defined WII
	#include "Wii/pindef-wii.h"
#else
	#error "Console type not defined in pindef.h"
#endif


#endif
