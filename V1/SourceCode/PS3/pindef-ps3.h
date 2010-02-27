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

		// Strings on PORTA
		#define STRING_1_PIN  PINA
		#define STRING_1_PORT PORTA
		#define STRING_1_DDR  DDRA
		#define STRING_1_PIN_NUM   5
		#define STRING_2_PIN  PINA
		#define STRING_2_PORT PORTA
		#define STRING_2_DDR  DDRA
		#define STRING_2_PIN_NUM   4
		#define STRING_3_PIN  PINA
		#define STRING_3_PORT PORTA
		#define STRING_3_DDR  DDRA
		#define STRING_3_PIN_NUM   3
		#define STRING_4_PIN  PINA
		#define STRING_4_PORT PORTA
		#define STRING_4_DDR  DDRA
		#define STRING_4_PIN_NUM   2
		#define STRING_5_PIN  PINA
		#define STRING_5_PORT PORTA
		#define STRING_5_DDR  DDRA
		#define STRING_5_PIN_NUM   1
		#define STRING_6_PIN  PINA
		#define STRING_6_PORT PORTA
		#define STRING_6_DDR  DDRA
		#define STRING_6_PIN_NUM   0
	
		// Frets on PORTC
		#define FRET_1_PIN    PINC
		#define FRET_1_PORT   PORTC
		#define FRET_1_DDR    DDRC
		#define FRET_1_PIN_NUM  3
		#define FRET_2_PIN    PINC
		#define FRET_2_PORT   PORTC
		#define FRET_2_DDR    DDRC
		#define FRET_2_PIN_NUM  4
		#define FRET_3_PIN    PINC
		#define FRET_3_PORT   PORTC
		#define FRET_3_DDR    DDRC
		#define FRET_3_PIN_NUM  5
		#define FRET_4_PIN    PINC
		#define FRET_4_PORT   PORTC
		#define FRET_4_DDR    DDRC
		#define FRET_4_PIN_NUM  6
		#define FRET_5_PIN    PINC
		#define FRET_5_PORT   PORTC
		#define FRET_5_DDR    DDRC
		#define FRET_5_PIN_NUM  7

		// Other Buttons
		#define PLUS_PIN    PINA
		#define PLUS_PORT   PORTA
		#define PLUS_DDR    DDRA
		#define PLUS_PIN_NUM    7

		#define MINUS_PIN    PINA
		#define MINUS_PORT   PORTA
		#define MINUS_DDR    DDRA
		#define MINUS_PIN_NUM   6

		#define PICK_PIN   PINC		// PICK_PIN sets the port that strum controls are mapped to
		#define PICK_PORT  PORTC
		#define PICK_DDR   DDRC
		#define PICK_PIN_NUM   2 

	
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

		// Strings on PORTC
		#define STRING_1_PIN  PINC
		#define STRING_1_PORT PORTC
		#define STRING_1_DDR  DDRC
		#define STRING_1_PIN_NUM   5
		#define STRING_2_PIN  PINC
		#define STRING_2_PORT PORTC
		#define STRING_2_DDR  DDRC
		#define STRING_2_PIN_NUM   4
		#define STRING_3_PIN  PINC
		#define STRING_3_PORT PORTC
		#define STRING_3_DDR  DDRC
		#define STRING_3_PIN_NUM   3
		#define STRING_4_PIN  PINC
		#define STRING_4_PORT PORTC
		#define STRING_4_DDR  DDRC
		#define STRING_4_PIN_NUM   2
		#define STRING_5_PIN  PINC
		#define STRING_5_PORT PORTC
		#define STRING_5_DDR  DDRC
		#define STRING_5_PIN_NUM   1
		#define STRING_6_PIN  PINC
		#define STRING_6_PORT PORTC
		#define STRING_6_DDR  DDRC
		#define STRING_6_PIN_NUM   0
	
		// Frets on PORT B
		#define FRET_1_PIN    PINB
		#define FRET_1_PORT   PORTB
		#define FRET_1_DDR    DDRB
		#define FRET_1_PIN_NUM   1
		#define FRET_2_PIN    PINB
		#define FRET_2_PORT   PORTB
		#define FRET_2_DDR    DDRB
		#define FRET_2_PIN_NUM     2
		#define FRET_3_PIN    PINB
		#define FRET_33_PORT   PORTB
		#define FRET_3_DDR    DDRB
		#define FRET_3_PIN_NUM  3
		#define FRET_4_PIN    PINB
		#define FRET_4_PORT   PORTB
		#define FRET_4_DDR    DDRB
		#define FRET_4_PIN_NUM    4
		#define FRET_5_PIN    PINB
		#define FRET_5_PORT   PORTB
		#define FRET_5_DDR    DDRB
		#define FRET_5_PIN_NUM  5

		// Other Buttons
		#define PLUS_PIN    PIND
		#define PLUS_PORT   PORTD
		#define PLUS_DDR    DDRD
		#define PLUS_PIN_NUM    7
		
		#define MINUS_PIN    PIND
		#define MINUS_PORT   PORTD
		#define MINUS_DDR    DDRD
		#define MINUS_PIN_NUM   6

		#define PICK_PIN   PIND		// PICK_PIN sets the port that strum controls are mapped to
		#define PICK_PORT  PORTD
		#define PICK_DDR   DDRD
		#define PICK_PIN_NUM   5 

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
