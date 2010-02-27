/*  OpenChord V1 AVR code for USB/PS3 *
		www.OpenChord.org
		
		Alan Chatham
		Copyright 2009

    This file provides the pin definitions for the microcontrollers used in the
	 Wii circuit boards bulit before the November 2009 combined USB/WII circuit board.
	
	THIS FILE IS DEPRECATED, and included only for supporting legacy versions of the hardware

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef pindef_wii

	#ifdef ATMEGA164OLD
		// I2C ports
		#define twi_port PORTC
		#define twi_ddr DDRC
		#define twi_scl_pin 0
		#define twi_sda_pin 1

		// Strings on PORTB
		#define STRING_1_PIN  PINB
		#define STRING_1_PORT PORTB
		#define STRING_1_DDR  DDRB
		#define STRING_1_PIN_NUM    5
		#define STRING_2_PIN  PINB
		#define STRING_2_PORT PORTB
		#define STRING_2_DDR  DDRB
		#define STRING_2_PIN_NUM   4
		#define STRING_3_PIN  PINB
		#define STRING_3_PORT PORTB
		#define STRING_3_DDR  DDRB
		#define STRING_3_PIN_NUM    3
		#define STRING_4_PIN  PINB
		#define STRING_4_PORT PORTB
		#define STRING_4_DDR  DDRB
		#define STRING_4_PIN_NUM   2
		#define STRING_5_PIN  PINB
		#define STRING_5_PORT PORTB
		#define STRING_5_DDR  DDRB
		#define STRING_5_PIN_NUM    1
		#define STRING_6_PIN  PINB
		#define STRING_6_PORT PORTB
		#define STRING_6_DDR  DDRB
		#define STRING_6_PIN_NUM    0
	
		// Frets on PORTD
		#define FRET_1_PIN    PIND
		#define FRET_1_PORT   PORTD
		#define FRET_1_DDR    DDRD
		#define FRET_1_PIN_NUM  6
		#define FRET_2_PIN    PIND
		#define FRET_2_PORT   PORTD
		#define FRET_2_DDR    DDRD
		#define FRET_2_PIN_NUM  3
		#define FRET_3_PIN    PIND
		#define FRET_3_PORT   PORTD
		#define FRET_3_DDR    DDRD
		#define FRET_3_PIN_NUM  2
		#define FRET_4_PIN    PIND
		#define FRET_4_PORT   PORTD
		#define FRET_4_DDR    DDRD
		#define FRET_4_PIN_NUM  1
		#define FRET_5_PIN    PIND
		#define FRET_5_PORT   PORTD
		#define FRET_5_DDR    DDRD
		#define FRET_5_PIN_NUM  0

		// Other Buttons
		#define PLUS_PIN    PINB
		#define PLUS_PORT   PORTB
		#define PLUS_DDR    DDRB
		#define PLUS_PIN_NUM    7

		#define PICK_PIN   PINB		// PICK_PIN sets the port that strum controls are mapped to
		#define PICK_PORT  PORTB
		#define PICK_DDR   DDRB
		#define PICK_PIN_NUM   6 

	#endif

	#ifdef ATMEGA164

		// I2C ports
		#define twi_port PORTC
		#define twi_ddr DDRC
		#define twi_scl_pin 0
		#define twi_sda_pin 1

		// Strings on PORTA
		#define STRING_1_PIN  PINA
		#define STRING_1_PORT PORTA
		#define STRING_1_DDR  DDRA
		#define STRING_1_PIN_NUM   0
		#define STRING_2_PIN  PINA
		#define STRING_2_PORT PORTA
		#define STRING_2_DDR  DDRA
		#define STRING_2_PIN_NUM   1
		#define STRING_3_PIN  PINA
		#define STRING_3_PORT PORTA
		#define STRING_3_DDR  DDRA
		#define STRING_3_PIN_NUM   2
		#define STRING_4_PIN  PINA
		#define STRING_4_PORT PORTA
		#define STRING_4_DDR  DDRA
		#define STRING_4_PIN_NUM   3
		#define STRING_5_PIN  PINA
		#define STRING_5_PORT PORTA
		#define STRING_5_DDR  DDRA
		#define STRING_5_PIN_NUM   4
		#define STRING_6_PIN  PINA
		#define STRING_6_PORT PORTA
		#define STRING_6_DDR  DDRA
		#define STRING_6_PIN_NUM   5
	
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
		#define FRET_4_PIN_NUM    6
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

	#endif


	#ifdef ATMEGA168			// Pin Definitions for the ATMega 168

		// I2C ports
		#define twi_port PORTC
		#define twi_ddr DDRC
		#define twi_scl_pin 5	// PIN C 5 is the SCL
		#define twi_sda_pin 4	// PIN C 4 is the SDA

		// Strings on PORTB				// All strings are mapped on PORTB
		#define STRING_1_PIN  PINB
		#define STRING_1_PORT PORTB
		#define STRING_1_DDR  DDRB
		#define STRING_1_PIN_NUM    0
		#define STRING_2_PIN  PINB
		#define STRING_2_PORT PORTB
		#define STRING_2_DDR  DDRB
		#define STRING_2_PIN_NUM   1
		#define STRING_3_PIN  PINB
		#define STRING_3_PORT PORTB
		#define STRING_3_DDR  DDRB
		#define STRING_3_PIN_NUM    2
		#define STRING_4_PIN  PINB
		#define STRING_4_PORT PORTB
		#define STRING_4_DDR  DDRB
		#define STRING_4_PIN_NUM   3
		#define STRING_5_PIN  PINB
		#define STRING_5_PORT PORTB
		#define STRING_5_DDR  DDRB
		#define STRING_5_PIN_NUM    4
		#define STRING_6_PIN  PINB
		#define STRING_6_PORT PORTB
		#define STRING_6_DDR  DDRB
		#define STRING_6_PIN_NUM    5
	
		// Frets on PORTD				// Frets are mapped to PORTD
		#define FRET_1_PIN    PIND
		#define FRET_1_PORT   PORTD
		#define FRET_1_DDR    DDRD
		#define FRET_1_PIN_NUM  0
		#define FRET_2_PIN    PIND
		#define FRET_2_PORT   PORTD
		#define FRET_2_DDR    DDRD
		#define FRET_2_PIN_NUM  1
		#define FRET_3_PIN    PIND
		#define FRET_3_PORT   PORTD
		#define FRET_3_DDR    DDRD
		#define FRET_3_PIN_NUM  2
		#define FRET_4_PIN    PIND
		#define FRET_4_PORT   PORTD
		#define FRET_4_DDR    DDRD
		#define FRET_4_PIN_NUM  3
		#define FRET_5_PIN    PIND
		#define FRET_5_PORT   PORTD
		#define FRET_5_DDR    DDRD
		#define FRET_5_PIN_NUM  4

		// Other Buttons
		#define PICK_PIN   PIND 	// pick_* sets the port that strum controls are mapped to
		#define PICK_PORT  PORTD
		#define PICK_DDR   DDRD
		#define PICK_PIN_NUM   5 // pin D5

		#define PLUS_PIN    PINB	// misc_* sets the port that other buttons (i.e. plus) are mapped to
		#define PLUS_PORT   PORTB
		#define PLUS_DDR    DDRB
		#define PLUS_PIN_NUM    6	// pin 
		
		#define MINUS_PIN    PINB	// misc_* sets the port that other buttons (i.e. plus) are mapped to
		#define MINUS_PORT   PORTB
		#define MINUS_DDR    DDRB
		#define MINUS_PIN_NUM    7
	



	#endif

/*
	#define dev_detect_port PORTD
	#define dev_detect_ddr DDRD
	#define dev_detect_pin 4

	#define uart_port PORTD
	#define uart_rx_pin 0

	#define power_detect_pin 0
	#define power_detect_port PORTD
	#define power_detect_ddr DDRD
	#define power_detect_input PIND

	#define adc_port PORTC
	#define adc_ddr DDRC*/

	// for the bit numbers, go to
	// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

#define pindef_wii
#endif
