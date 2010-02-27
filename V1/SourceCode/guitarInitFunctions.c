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
	FRET_1_DDR &= ~(1<<FRET_1_PIN_NUM);        // input
	FRET_1_PORT |= (1<<FRET_1_PIN_NUM);		//pull-up enabled	
	FRET_2_DDR &= ~(1<<FRET_2_PIN_NUM);
	FRET_2_PORT |= (1<<FRET_2_PIN_NUM);
	FRET_3_DDR &= ~(1<<FRET_3_PIN_NUM);
	FRET_3_PORT |= (1<<FRET_3_PIN_NUM);
	FRET_4_DDR &= ~(1<<FRET_4_PIN_NUM);
	FRET_4_PORT |= (1<<FRET_4_PIN_NUM);
	FRET_5_DDR &= ~(1<<FRET_5_PIN_NUM);
	FRET_5_PORT |= (1<<FRET_5_PIN_NUM);

	// Set the pins on strumming and plus as inputs, with internal resistor
	PLUS_DDR &= ~(1<<PLUS_PIN_NUM);
	PLUS_PORT |= (1<<PLUS_PIN_NUM);
	MINUS_DDR &= ~(1<<MINUS_PIN_NUM);
	MINUS_PORT |= (1<<MINUS_PIN_NUM);

	PICK_DDR &= ~(1<<PICK_PIN_NUM);
	PICK_PIN |= (1<<PICK_PIN_NUM);

	// Set the string pins as output pins, outputting HIGH
	/* MAKE SURE YOU HAVE DIODES OPPOSING THIS CURRENT OUTFLOW! */
	STRING_1_DDR |= (1<<STRING_1_PIN_NUM);  //output
	STRING_1_PORT |= (1<<STRING_1_PIN_NUM); // set high
	STRING_2_DDR |= (1<<STRING_2_PIN_NUM);  
	STRING_2_PORT |= (1<<STRING_2_PIN_NUM);
	STRING_3_DDR |= (1<<STRING_3_PIN_NUM);  
	STRING_3_PORT |= (1<<STRING_3_PIN_NUM);
	STRING_4_DDR |= (1<<STRING_4_PIN_NUM);  
	STRING_4_PORT |= (1<<STRING_4_PIN_NUM);
	STRING_5_DDR |= (1<<STRING_5_PIN_NUM);  
	STRING_5_PORT |= (1<<STRING_5_PIN_NUM);
	STRING_6_DDR |= (1<<STRING_6_PIN_NUM);  
	STRING_6_PORT |= (1<<STRING_6_PIN_NUM);
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
