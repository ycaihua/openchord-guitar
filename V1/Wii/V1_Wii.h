/*  OpenChord V1 AVR code for the Wii *
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

		Special thanks to Frank Zhao and his Wiimote Extension Library,
		details which can be found here:
		
		http://frank.circleofcurrent.com/content.php?page_id=wii_extension_lib

	This program is distributed under the terms of the GNU General Public License, v3,
	and includes the wiimote extension library, originally distributed under the Apache 2.0 license

	Further details of licenses and thank-yous can be found in V1_Wii.c
*/

#ifndef v1_wii_h

#if F_CPU == NULL
#error "define your clock speed"
#endif

#include <avr/io.h>

#include "config.h"
#include "pindef-wii.h"
#include "macros.h"

#include "wiimote.h"

#define v1_wii_h
#endif

