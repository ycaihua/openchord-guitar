/*  OpenChord V1 AVR code for USB/PS3 *
		www.OpenChord.org
		
		Alan Chatham
		Copyright 2009
*/

#ifndef pindef_wii


// pick which ATMega chip you are using (comment out the one you aren't)
// don't use both at once
//#define ATMEGA164
#define ATMEGA168

	#ifdef ATMEGA164OLD
		// I2C ports
		#define twi_port PORTC
		#define twi_ddr DDRC
		#define twi_scl_pin 0
		#define twi_sda_pin 1

		// Strings
		#define string_pin  PINB
		#define string_port PORTB
		#define string_ddr  DDRB
		#define first_string    5
		#define second_string   4
		#define third_string    3
		#define fourth_string   2
		#define fifth_string    1
		#define sixth_string    0
	
		// Frets
		#define fret_pin    PIND
		#define fret_port   PORTD
		#define fret_ddr    DDRD
		#define green_pin   6
		#define red_pin     3
		#define yellow_pin  2
		#define blue_pin    1
		#define orange_pin  0

		// Other Buttons
		#define misc_pin    PINB	// misc_pin sets the port that other buttons (i.e. plus) are mapped to
		#define misc_port   PORTB
		#define misc_ddr    DDRB
		#define plus_pin    7

		#define pick_pin   PINB		// pick_pin sets the port that strum controls are mapped to
		#define pick_port  PORTB
		#define pick_ddr   DDRB
		#define strum_pin   6 

	#endif

	#ifdef ATMEGA164

		// I2C ports
		#define twi_port PORTC
		#define twi_ddr DDRC
		#define twi_scl_pin 0
		#define twi_sda_pin 1

		// Strings
		#define string_pin  PINA
		#define string_port PORTA
		#define string_ddr  DDRA
		#define first_string    0
		#define second_string   1
		#define third_string    2
		#define fourth_string   3
		#define fifth_string    4
		#define sixth_string    5
	
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

	#endif


	#ifdef ATMEGA168			// Pin Definitions for the ATMega 168

		// I2C ports
		#define twi_port PORTC
		#define twi_ddr DDRC
		#define twi_scl_pin 5	// PIN C 5 is the SCL
		#define twi_sda_pin 4	// PIN C 4 is the SDA

		// Strings				// All strings are mapped on PORTB
		#define string_pin  PINB
		#define string_port PORTB
		#define string_ddr  DDRB
		#define first_string    0
		#define second_string   1
		#define third_string    2
		#define fourth_string   3
		#define fifth_string    4
		#define sixth_string    5
	
		// Frets				// Frets are mapped to PORTD
		#define fret_pin    PIND
		#define fret_port   PORTD
		#define fret_ddr    DDRD
		#define green_pin   0
		#define red_pin     1
		#define yellow_pin  2
		#define blue_pin    3
		#define orange_pin  4

		// Other Buttons
		#define pick_pin   PIND 	// pick_* sets the port that strum controls are mapped to
		#define pick_port  PORTD
		#define pick_ddr   DDRD
		#define strum_pin   5 // pin D5

		#define misc_pin    PINB	// misc_* sets the port that other buttons (i.e. plus) are mapped to
		#define misc_port   PORTB
		#define misc_ddr    DDRB
		//#define up_pin      5
		//#define down_pin    6
		#define plus_pin    6	// pin B6
		#define minus_pin    7
	



	#endif


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
	#define adc_ddr DDRC

	// for the bit numbers, go to
	// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

#define pindef_wii
#endif
