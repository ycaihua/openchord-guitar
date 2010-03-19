/*pindef-ps3.h
    part of the OpenChord V1 AVR code
		www.OpenChord.org
		
		Alan Chatham
		Copyright 2009

    This file provides the pin definitions for the microcontrollers used in the
	 USB circuit boards bulit before the November 2009 combined USB/WII circuit board.
	
	THIS FILE IS DEPRECATED, and included only for supporting legacy versions of the hardware

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef pindef_ps3
#define pindef_ps3

#include "../configAndConstants.h"

	#ifdef ATMEGA164
		// In order for the USB to work, we must leave at least one interrupt pin
		// and one other pin on the same port available for the USB library.
		// The pins used are defined in usbconfig.h, and are Port D, pins 2 and 4.

		// Strings
		#define string_pin  PINA
		#define string_port PORTA
		#define string_ddr  DDRA
		#define first_string    5
		#define second_string   4
		#define third_string    3
		#define fourth_string   2
		#define fifth_string    1
		#define sixth_string    0
	
		// Frets
		#define fret_pin    PINC
		#define fret_port   PORTC
		#define fret_ddr    DDRC
		#define green_pin   3
		#define red_pin     4
		#define yellow_pin  5
		#define blue_pin    6
		#define orange_pin  7

		// Other Buttons
		#define misc_pin    PINA	// misc_pin sets the port that other buttons (i.e. plus) are mapped to
		#define misc_port   PORTA
		#define misc_ddr    DDRA
		#define plus_pin    7
		#define minus_pin   6

		#define pick_pin   PINC		// pick_pin sets the port that strum controls are mapped to
		#define pick_port  PORTC
		#define pick_ddr   DDRC
		#define strum_pin   2 

	
		/* ---------------------------- Hardware Config ---------------------------- */
		/* This all originally comes from usbconfig.c, but I put it here to keep all
		 * the pin configuration stuff in the same place.
		 */

		#define USB_CFG_IOPORTNAME      D
		/* This is the port where the USB bus is connected. When you configure it to
		 * "B", the registers PORTB, PINB and DDRB will be used.
		 */
		#define USB_CFG_DMINUS_BIT      4
		/* This is the bit number in USB_CFG_IOPORT where the USB D- line is connected.
		 * This may be any bit in the port.
		 */
		#define USB_CFG_DPLUS_BIT       2
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
		/* #define USB_INTR_CFG            MCUCR */
		/* #define USB_INTR_CFG_SET        ((1 << ISC00) | (1 << ISC01)) */
		/* #define USB_INTR_CFG_CLR        0 */
		/* #define USB_INTR_ENABLE         GIMSK */
		/* #define USB_INTR_ENABLE_BIT     INT0 */
		/* #define USB_INTR_PENDING        GIFR */
		/* #define USB_INTR_PENDING_BIT    INTF0 */
		/* #define USB_INTR_VECTOR         SIG_INTERRUPT0 */

	#endif

	#ifdef ATMEGA168
		// In order for the USB to work, we must leave at least one interrupt pin
		// and one other pin on the same port available for the USB library.
		// The pins used are defined in usbconfig.h, and are Port D, pins 2 and 4.

		// Strings
		#define string_pin  PINC
		#define string_port PORTC
		#define string_ddr  DDRC
		#define first_string    5
		#define second_string   4
		#define third_string    3
		#define fourth_string   2
		#define fifth_string    1
		#define sixth_string    0
	
		// Frets
		#define fret_pin    PINB
		#define fret_port   PORTB
		#define fret_ddr    DDRB
		#define green_pin   1
		#define red_pin     2
		#define yellow_pin  3
		#define blue_pin    4
		#define orange_pin  5

		// Other Buttons
		#define misc_pin    PIND	// misc_pin sets the port that other buttons (i.e. plus) are mapped to
		#define misc_port   PORTD
		#define misc_ddr    DDRD
		#define plus_pin    7
		#define minus_pin   6

		#define pick_pin   PIND		// pick_pin sets the port that strum controls are mapped to
		#define pick_port  PORTD
		#define pick_ddr   DDRD
		#define strum_pin   5 

		/* ---------------------------- Hardware Config ---------------------------- */
		/* This all originally comes from usbconfig.c, but I put it here to keep all
		 * the pin configuration stuff in the same place.
		 */

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
		/* #define USB_INTR_CFG_SET        ((1 << ISC00) | (1 << ISC01)) */   //feature bits to set
		/* #define USB_INTR_CFG_CLR        0 */  // feature bits to clear
		/* #define USB_INTR_ENABLE         GIMSK */   //register where interrupt enable bit resides
		 #define USB_INTR_ENABLE_BIT     INT1   // bit number in above register
		/* #define USB_INTR_PENDING        GIFR */  // register where interrupt pending bit resides
		 #define USB_INTR_PENDING_BIT    INTF1  // bit number in above register
		 #define USB_INTR_VECTOR         SIG_INTERRUPT1  // interrupt vector

	#endif

#endif
