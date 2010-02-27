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

#ifdef GENERAL_CIRCUIT_BOARD
	/* This part defines all the pins for the current (as of November 2009) combination
	    Wii-USB circuit board. If you don't have the GENERAL_CIRCUIT_BOARD option set
		in configAndConstants.h, then it will check for other options you have set there.
		Those options are for older, deprecated boards, and as such, the pindefs for those
		options are held in their own header files, which simply get included.
    */

    /* NOTE - Currently, the strings and frets are all pins on the same registers. 
	    This is not a strict requirement; this was done due to laziness. If you want
		to change the pins around to have them on different registers, though, 
		you'll need to change a fair bit of code. For the frets, it should be a simple
		replacement process, but the strings are read through a function, so you'd have
		to amend that function to pass in the info about each string pin's register as well
	*/

	// Strings - These are on Port D, broken up by the pins used for USB D+ and D-
	#define STRING_1_PIN  PIND
	#define STRING_1_PORT PORTD
	#define STRING_1_DDR  DDRD
	#define STRING_1_PIN_NUM   7
	#define STRING_2_PIN  PIND
	#define STRING_2_PORT PORTD
	#define STRING_2_DDR  DDRD
	#define STRING_2_PIN_NUM   6
	#define STRING_3_PIN  PIND
	#define STRING_3_PORT PORTD
	#define STRING_3_DDR  DDRD
	#define STRING_3_PIN_NUM   5
	#define STRING_4_PIN  PIND
	#define STRING_4_PORT PORTD
	#define STRING_4_DDR  DDRD
	#define STRING_4_PIN_NUM   2
	#define STRING_5_PIN  PIND
	#define STRING_5_PORT PORTD
	#define STRING_5_DDR  DDRD
	#define STRING_5_PIN_NUM   1
	#define STRING_6_PIN  PIND
	#define STRING_6_PORT PORTD
	#define STRING_6_DDR  DDRD
	#define STRING_6_PIN_NUM   0
	
	// Frets on PORTB
	#define FRET_1_PIN    PINB
	#define FRET_1_PORT   PORTB
	#define FRET_1_DDR    DDRB
	#define FRET_1_PIN_NUM  1
	#define FRET_2_PIN    PINB
	#define FRET_2_PORT   PORTB
	#define FRET_2_DDR    DDRB
	#define FRET_2_PIN_NUM  2
	#define FRET_3_PIN    PINB
	#define FRET_3_PORT   PORTB
	#define FRET_3_DDR    DDRB
	#define FRET_3_PIN_NUM  3
	#define FRET_4_PIN    PINB
	#define FRET_4_PORT   PORTB
	#define FRET_4_DDR    DDRB
	#define FRET_4_PIN_NUM  4
	#define FRET_5_PIN    PINB
	#define FRET_5_PORT   PORTB
	#define FRET_5_DDR    DDRB
	#define FRET_5_PIN_NUM  5

	// Other Buttons 
	#define PLUS_PIN    PINC
	#define PLUS_PORT   PORTC
	#define PLUS_DDR    DDRC
	#define PLUS_PIN_NUM    0

	#define MINUS_PIN   PINC
	#define MINUS_PORT  PORTC
	#define MINUS_DDR   DDRC
	#define MINUS_PIN_NUM   1

	#define PICK_PIN   PINC	// PICK_PIN sets the port that strum controls are mapped to
	#define PICK_PORT  PORTC
	#define PICK_DDR   DDRC
	#define PICK_PIN_NUM   2

	/* ---------------------------- Hardware Config ---------------------------- */
	/* The USB stuff originally comes from usbconfig.c, but I put it here to keep all
	 * the pin configuration stuff in the same place. It also includes the locations
	 * of the I2C pins that the Wiimote library uses.
	 */


	// I2C ports - the Wiimote communicates over I2C
	#define twi_port PORTC
	#define twi_ddr DDRC
	#define twi_scl_pin 5	// PIN C 5 is the SCL line
	#define twi_sda_pin 4	// PIN C 4 is the SDA line

	// The following is for the USB option -

	#define USB_CFG_IOPORTNAME      D
	/* This is the port where the USB bus is connected. When you configure it to
	 * "B", the registers PORTB, PINB and DDRB will be used.
	 */
	#define USB_CFG_DMINUS_BIT      4
	/* This is the bit number in USB_CFG_IOPORT where the USB D- line is connected.
	 * This may be any bit in the port.
	 */
	#define USB_CFG_DPLUS_BIT       3
	/* This is the bit number in USB_CFG_IOPORT where the USB D+ line is connected.
	 * This may be any bit in the port. Please note that D+ must also be connected
	 * to interrupt pin INT0! [You can also use other interrupts, see section
	 * "Optional MCU Description" below, or you can connect D- to the interrupt, as
	 * it is required if you use the USB_COUNT_SOF feature. If you use D- for the
	 * interrupt, the USB interrupt will also be triggered at Start-Of-Frame
	 * markers every millisecond.]
	 */

		/* ----------------------- Optional MCU Description ------------------------ */
	/* The following configurations have working defaults in usbdrv.h. You
	 * usually don't need to set them explicitly. Only if you want to run
	 * the driver on a device which is not yet supported or with a compiler
	 * which is not fully supported (such as IAR C) or if you use a differnt
	 * interrupt than INT0, you may have to define some of these.
	 */
	/* #define USB_INTR_CFG            MCUCR */  // register where interrupt features are configured
	 #define USB_INTR_CFG_SET        ((1 << ISC10) | (1 << ISC11))   //feature bits to set
	/* #define USB_INTR_CFG_CLR        0 */  // feature bits to clear
	/* #define USB_INTR_ENABLE         GIMSK */   //register where interrupt enable bit resides
	 #define USB_INTR_ENABLE_BIT     INT1   // bit number in above register
	/* #define USB_INTR_PENDING        GIFR */  // register where interrupt pending bit resides
	 #define USB_INTR_PENDING_BIT    INTF1  // bit number in above register
	 #define USB_INTR_VECTOR         SIG_INTERRUPT1  // interrupt vector

#else // This code is used for legacy boards
	#if defined PS3_USB
	  #include "PS3/pindef-ps3.h"
	  #ifdef WII
	    #error "Both WII and PS3_USB are defined in configAndConstants.h - choose only one"
	  #endif

	#elif defined WII
	  #include "Wii/pindef-wii.h"
	  #ifdef PS3_USB
	    #error "Both WII and PS3_USB are defined in configAndConstants.h - choose only one"
	  #endif

	#else
	  #error "Console type not defined for pindef.h - check configAndConstants.h"
	#endif

#endif


#endif // End of include guard
