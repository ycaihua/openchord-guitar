/* guitarInitFunctions.c
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#include "guitarInitFunctions.h"

void setPins(void)
{
	// Set the fret pins as input pins, with internal resistor
	fret_ddr &= ~(1<<green_pin);        // input
	fret_port |= (1<<green_pin);		//pull-up enabled	
	fret_ddr &= ~(1<<red_pin);
	fret_port |= (1<<red_pin);
	fret_ddr &= ~(1<<yellow_pin);
	fret_port |= (1<<yellow_pin);
	fret_ddr &= ~(1<<blue_pin);
	fret_port |= (1<<blue_pin);
	fret_ddr &= ~(1<<orange_pin);
	fret_port |= (1<<orange_pin);

	// Set the pins on strumming and plus as inputs, with internal resistor
	misc_ddr &= ~(1<<plus_pin);
	misc_port |= (1<<plus_pin);
	misc_ddr &= ~(1<<minus_pin);
	misc_port |= (1<<minus_pin);

	pick_ddr &= ~(1<<strum_pin);
	pick_port |= (1<<strum_pin);

	// Set the string pins as output pins, outputting HIGH
	/* MAKE SURE YOU HAVE DIODES OPPOSING THIS CURRENT OUTFLOW! */
	string_1_ddr |= (1<<first_string);  //output
	string_1_port |= (1<<first_string); // set high
	string_2_ddr |= (1<<second_string);  
	string_2_port |= (1<<second_string);
	string_3_ddr |= (1<<third_string);  
	string_3_port |= (1<<third_string);
	string_4_ddr |= (1<<fourth_string);  
	string_4_port |= (1<<fourth_string);
//	string_5_ddr |= (1<<fifth_string);  
//	string_5_port |= (1<<fifth_string);
//	string_6_ddr |= (1<<sixth_string);  
//	string_6_port |= (1<<sixth_string);
}

void setTimer(void) //This sets up a timer to handle the green button thing
{
	PRR &= ~(1<<PRTIM1);	// ensure power to Timer/Counter1

	TCCR1A &= ~(1<<COM1A1);  // This prevents OC2A pin from doing anything
	TCCR1A &= ~(1<<COM1A0);  //  to the pins on the chip. Otherwise, we might
	TCCR1A &= ~(1<<COM1B1);  //  get signals coming out of some of the pins we might use
	TCCR1A &= ~(1<<COM1B0);

	TCCR1A &= ~(1<<WGM11);  // These bits set up normal mode on the Waveform Generation
	TCCR1A &= ~(1<<WGM10);  // This means that the counter acts like a normal timer,
	TCCR1B &= ~(1<<WGM12);  // not some sort of fancy digital-analog converter or anything
	TCCR1B &= ~(1<<WGM13);  // It counts up to 65536 and overflows to zero

	TCCR1B |= (1<<CS12);   // These set the clock to CPU (8MHz) / 1024 
	TCCR1B &= ~(1<<CS11);  // or roughly 8 kHz (7.8125 kHz)
	TCCR1B |= (1<<CS10);   // When counting from zero to 0xFFFF, it takes about 8 seconds

	// The counter's value is stored in TCNT1
}
