/*  OpenChord V1 AVR code for the Wii *
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

		Special thanks to Frank Zhao and his Wiimote Extension Library,
		details which can be found here:
		
		http://frank.circleofcurrent.com/content.php?page_id=wii_extension_lib

		If you read this Frank, thank you so much.  After trying to figure out
		the PS2's protocol for like 3 days straight (still no luck), your work
		made this so, so much easier.

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

/*  In its current form, this is set up for the Atmel ATMega 164p chip. In order to work,
    the followin settings must be applied to the chip

Fuses - both of these are default enabled, so must be disabled

JTAGEN = disabled   - JTAG works over the TWI pins, so it must be disabled
CKDIV8 = disabled   - This increases the clock speed to 8mHz, otherwise it's 
					  too slow for the Wiimote's 400kHz TWI communication
*/

#include "V1_Wii.h"

// most of this data is found on
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

#define GREEN_TIMER_WAIT 100 // This controls the delay for the green button.
				// There is a delay in order to avoid triggering green when red is played
				// but the green fret is hit first, so you don't press red but see the green
				// button flash for an instant before red activates



// controller id
//const unsigned char drum_id[6] = {0x01, 0x00, 0xA4, 0x20, 0x01, 0x03};
const unsigned char guitar_id[6] = {0x00, 0x00, 0xA4, 0x20, 0x01, 0x03};
//const unsigned char nunchuk_id[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


unsigned char but_dat[6]; // struct containing button data

int debounceTimer; // This is used as a global timer for debouncing buttons
char previousPlusOn;
//char controllerMode;

#define DEBOUNCE_DELAY 1000

#define NUMBER_OF_FRETS 5// This is how many frets are wired up to the controller

// These are labels to tell us which mode we're in
#define FRETS			0
#define FRETS_TO_NOTES  1
#define NOTES 			2
#define NOTES_TO_CHORDS 3
#define CHORDS 			4
#define CHORDS_TO_FRETS 5
#define FRETS_HIGH      6

// These are which bits (from left to right) in the 6th byte of the packet
// correspond to which buttons
#define GREEN_BIT  4
#define RED_BIT    6
#define YELLOW_BIT 3
#define BLUE_BIT   5
#define ORANGE_BIT 7
#define UP_BIT     0

// These buttons are in the 5th byte
#define DOWN_BIT   6
#define PLUS_BIT   2
#define MINUS_BIT  4

// calibration data
const unsigned char cal_data[32] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
};

typedef struct dataForController
{
	char greenOn;  // variables to abstractly tell us which buttons are pressed		
	char redOn;
	char yellowOn;
	char blueOn;
	char orangeOn;
	char upOn;
	char downOn;
} dataForController;

int greenTimers[6]; //These give timing to properly read green buttons on each string.
int triedGreen[6];  // Also used for the green button timing thing

//Just a function declaration to stop a compiler warning. Actually defined below 'main'
inline void processStringState(dataForController* data, int stringState[], 
												int buttonStringPatterns[][6], char *controllerMode);

void wm_timer_inc()  // This function is unused by me.
{
        return;
}

inline void init(void) // This just initialized variables, mostly to zero
{
	int zero6array[6] = {0,0,0,0,0,0};  // These 3 lines set greenTimers and triedGreen to zeros
	memcpy(greenTimers, zero6array, 6);  
	memcpy(triedGreen, zero6array, 6);

    // initialize button data array
    but_dat[0] = 0b00011111;
    but_dat[1] = 0b00011111;
    but_dat[2] = 0b00001111; // last 5 bits are touchbar data, should be 0F for no touching
    but_dat[3] = 0b00001111; // last 5 bits are whammy bar data, should be 0F probably
    but_dat[4] = 0b11111111; // no buttons pressed (with the Wiimote, 0 is pressed)
    but_dat[5] = 0b11111111; // no buttons pressed

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
	string_ddr |= (1<<first_string);  //output
	string_port |= (1<<first_string); // set high
	string_ddr |= (1<<second_string);  
	string_port |= (1<<second_string);
	string_ddr |= (1<<third_string);  
	string_port |= (1<<third_string);
	string_ddr |= (1<<fourth_string);  
	string_port |= (1<<fourth_string);
	string_ddr |= (1<<fifth_string);  
	string_port |= (1<<fifth_string);
	string_ddr |= (1<<sixth_string);  
	string_port |= (1<<sixth_string);


} // end of init



void setTimer(void) //This sets up a timer to handle the green button thing
{
	PRR &= ~(1<<PRTIM1);	// ensure power to Timer/Counter1

	TCCR1A &= ~(1<<COM1A1);  // This prevents OC2A pin from doing anything
	TCCR1A &= ~(1<<COM1A0);  ///  to the pins
	TCCR1A &= ~(1<<COM1B1);  
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


inline void setButtons(dataForController data, char* controllerMode)
// This function takes the 'color'On values set by readF and sets up the but_dat array accordingly
{
	//set the buttons in the button data array to all 1's
	but_dat[4] = 0b11111111;
	but_dat[5] = 0b11111111;

	// Since 'colorOn' is 1 when a fret is pressed, we invert, shift, invert, and & it to set the
	// corresponding button bit to zero, which corresponds to a button press.
	but_dat[5] &= ~(data.orangeOn << ORANGE_BIT);
	but_dat[5] &= ~(data.blueOn << BLUE_BIT);
	but_dat[5] &= ~(data.yellowOn << YELLOW_BIT);
	but_dat[5] &= ~(data.redOn << RED_BIT);
	but_dat[5] &= ~(data.greenOn << GREEN_BIT);

	but_dat[5] &= ~(data.upOn << UP_BIT);
	but_dat[4] &= ~(data.downOn << DOWN_BIT);

	int i = misc_pin; // check for plus and minus
	char plusOn = !(i & (1<<plus_pin)); //Plus and minus are held high normally, but pressing sets it low
	char minusOn = !(i & (1<<minus_pin));

	but_dat[4] &= ~(plusOn << PLUS_BIT);
	but_dat[4] &= ~(minusOn << MINUS_BIT);
}


inline void configSetButtons(int* buttonToAssign, char* plusLock, int buttonStringPatterns[][6], 
						int stringState[], char* controllerMode)
{
	//set the buttons in the button data array to all 1's
	but_dat[4] = 0b11111111;
	but_dat[5] = 0b11111111;

	// Since 'colorOn' is 1 when a fret is pressed, we invert, shift, invert, and & it to set the
	// corresponding button bit to zero, which corresponds to a button press.
	if (*buttonToAssign == 0)
		but_dat[5] &= ~(1 << GREEN_BIT);
	else if(*buttonToAssign == 1)
		but_dat[5] &= ~(1 << RED_BIT);
	else if(*buttonToAssign == 2)
		but_dat[5] &= ~(1 << YELLOW_BIT);
	else if(*buttonToAssign == 3)
		but_dat[5] &= ~(1 << BLUE_BIT);
	else if(*buttonToAssign == 4)
		but_dat[5] &= ~(1 << ORANGE_BIT);

	int i = misc_pin; // check for plus and minus
	char plusOn = !(i & (1<<plus_pin)); //Plus and minus are held high normally, but pressing sets it low

	// Code to debounce our plus button
	if (plusOn != 0)
	{
		// Setup a timer if 
		if (previousPlusOn == 0)
		{
			debounceTimer = TCNT1;
			previousPlusOn = 1;
			plusOn = 0;
		}
		else if ( (TCNT1 - debounceTimer < DEBOUNCE_DELAY) && (TCNT1 - debounceTimer >= 0))
		{
			plusOn = 0;
		}		
	}
	else
	{
		previousPlusOn = 0;
	}

	// Now, if you press plus, it will lock in whatever string pattern
	// and advance to working on the next pattern.
	if((plusOn) && (*plusLock == 0))
	{

		if (*controllerMode == CHORDS) 
		{   // Set the pattern to nothing pressed
			buttonStringPatterns[*buttonToAssign][0] = 0;
			buttonStringPatterns[*buttonToAssign][1] = 0;
			buttonStringPatterns[*buttonToAssign][2] = 0;
			buttonStringPatterns[*buttonToAssign][3] = 0;
			buttonStringPatterns[*buttonToAssign][4] = 0;
			buttonStringPatterns[*buttonToAssign][5] = 0;
		}
			// Assign what's being pressed on the frets to what matches the current button press
		buttonStringPatterns[*buttonToAssign][0] |= stringState[0];
		buttonStringPatterns[*buttonToAssign][1] |= stringState[1];
		buttonStringPatterns[*buttonToAssign][2] |= stringState[2];
		buttonStringPatterns[*buttonToAssign][3] |= stringState[3];
		buttonStringPatterns[*buttonToAssign][4] |= stringState[4];
		buttonStringPatterns[*buttonToAssign][5] |= stringState[5];

		*plusLock = 1;
		*buttonToAssign += 1;

	}
	else if(plusOn == 0)
	{
		*plusLock = 0;
	}

	return;
}

inline void doXnops(int x)
{
	while (x >= 0)
	{
		nop();
		x--;
	}
}


//readF reads the frets for a single string, and returns an int acting as a boolean array
//  telling which frets are being touched by the string.
void readFrets(int guitarString, int* stringState)
{
  	string_port &= ~(1<<guitarString);  // Set the string LOW

	// Then we have to delay for a while, since right after the switch, the pin values
	// jump around a bit
	doXnops(20);	
	// Read the values on the frets
  	int i = fret_pin;               
	i = fret_pin;
	i = fret_pin;		// More delay funcitonality... without these delays, there is some bouncing
	i = fret_pin;		// on the pins, so the read screws up.
	  
	//Reset everything on the string
	*stringState = 0;	                
	// Next, since the fret pin is zero if the a fret is grounded, we do a mask to isolate each pin from 
	//  the port, which will return a zero if pressed and non-zero if unpressed. So we ! that to get a 1 if
	//  the string touches that fret and a zero if it doesn't touch, which we then load into our storage 
	//  integer.

 	*stringState |= ( (!(i & (1<<green_pin))) << 1); //green fret
 	*stringState |= ( (!(i & (1<<red_pin))) << 2); //red fret
 	*stringState |= ( (!(i & (1<<yellow_pin))) << 3); //yellow fret
 	*stringState |= ( (!(i & (1<<blue_pin))) << 4); //blue fret
 	*stringState |= ( (!(i & (1<<orange_pin))) << 5); //orange fret

	// Also, if we strummed a string, hold that in the 'zeroth' fret
	i = pick_pin;
 	*stringState |= ( (!(i & (1<<strum_pin))) << 0); //pick touching the string

	//Turn the string back HIGH and return
	string_port |= (1<<guitarString); 
	return;
}

// This function tests to see if you're pressing the top 4 strings on the orange fret,
//  thereby activating config mode. Works with both chords and scales.

inline char testForConfigMode(int stringState[], int buttonStringPatterns[][6], char *controllerMode)
{
	if ((stringState[0] == 0b00110000) &&
		(stringState[1] == 0b00110000) &&
		(stringState[2] == 0b00110000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
	{
		// If we're playing with notes, when we enter config mode, we need to clear the 
		//  button string patterns once so we can rebuild them.
		if (*controllerMode == NOTES)
		{
			buttonStringPatterns[0][0] = 0;	buttonStringPatterns[0][1] = 0;
			buttonStringPatterns[0][2] = 0;	buttonStringPatterns[0][3] = 0;
			buttonStringPatterns[0][4] = 0;	buttonStringPatterns[0][5] = 0;
			buttonStringPatterns[1][0] = 0;	buttonStringPatterns[1][1] = 0;
			buttonStringPatterns[1][2] = 0;	buttonStringPatterns[1][3] = 0;
			buttonStringPatterns[1][4] = 0;	buttonStringPatterns[1][5] = 0;
			buttonStringPatterns[2][0] = 0;	buttonStringPatterns[2][1] = 0;
			buttonStringPatterns[2][2] = 0;	buttonStringPatterns[2][3] = 0;
			buttonStringPatterns[2][4] = 0;	buttonStringPatterns[2][5] = 0;
			buttonStringPatterns[3][0] = 0;	buttonStringPatterns[3][1] = 0;
			buttonStringPatterns[3][2] = 0;	buttonStringPatterns[3][3] = 0;
			buttonStringPatterns[3][4] = 0;	buttonStringPatterns[3][5] = 0;
			buttonStringPatterns[4][0] = 0;	buttonStringPatterns[4][1] = 0;
			buttonStringPatterns[4][2] = 0;	buttonStringPatterns[4][3] = 0;
			buttonStringPatterns[4][4] = 0;	buttonStringPatterns[4][5] = 0;\
		}
		return 1;
	}
	else
		return 0;
}

inline char testForEndConfigMode(int stringState[])
{
	if ((stringState[0] == 0b00000000) &&
		(stringState[1] == 0b00110000) &&
		(stringState[2] == 0b00110000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
		return 1;
	else
		return 0;
}

inline void resetStringPatterns(int stringState[], int buttonStringPatterns[][6], char* controllerMode)
{
	// Test for the right keypress to start the mode
	if ((stringState[0] == 0b00111000) &&
		(stringState[1] == 0b00111000) &&
		(stringState[2] == 0b00111000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
	{
		if (*controllerMode % 2 == 0) // This means that the mode is a full mode
			{++*controllerMode;}  //This puts it into a transitional mode
	}

	// And if we're not pressing that anymore, finish the transition
	else if((stringState[0] == 0) &&
			(stringState[1] == 0) &&
			(stringState[2] == 0)) 
	{
		if (*controllerMode == CHORDS_TO_FRETS) // since these modes are just numbers, we need to wrap around
		{	
			*controllerMode = FRETS;
		}
		
		// Now, reset the buttonStringPattern array to be appropriate for the mode
		if (*controllerMode  == FRETS_TO_NOTES)
		{
		    buttonStringPatterns[0][0] = 0b0000000; // [0][0] through [0][5] - green button
			buttonStringPatterns[0][1] = 0b0001100; 
			buttonStringPatterns[0][2] = 0b0000000;	
			buttonStringPatterns[0][3] = 0b0000110;
			buttonStringPatterns[0][4] = 0b0000000;
			buttonStringPatterns[0][5] = 0b0000010;

			buttonStringPatterns[1][0] = 0b0000010; // [1][0] through [1][5] - red button
			buttonStringPatterns[1][1] = 0b0000000; 
			buttonStringPatterns[1][2] = 0b0000001;
			buttonStringPatterns[1][3] = 0b0000000;
			buttonStringPatterns[1][4] = 0b0000001;
			buttonStringPatterns[1][5] = 0b0001100;

			buttonStringPatterns[2][0] = 0b0001100; // [2][0] through [2][5] - yellow button
			buttonStringPatterns[2][1] = 0b0000000; // This is an A chord.
			buttonStringPatterns[2][2] = 0b0000110;
			buttonStringPatterns[2][3] = 0b0000000;
			buttonStringPatterns[2][4] = 0b0000010;
			buttonStringPatterns[2][5] = 0b0000000;
									  
			buttonStringPatterns[3][0] = 0b0000000; // [3][0] through [3][5] - blue button
			buttonStringPatterns[3][1] = 0b0000001; 
			buttonStringPatterns[3][2] = 0b0001100;
			buttonStringPatterns[3][3] = 0b0000000;
			buttonStringPatterns[3][4] = 0b0001100;
			buttonStringPatterns[3][5] = 0b0000000;

			buttonStringPatterns[4][0] = 0b0000000; // [4][0] through [4][5] - blue button
			buttonStringPatterns[4][1] = 0b0000110; 
			buttonStringPatterns[4][2] = 0b0000000;
			buttonStringPatterns[4][3] = 0b0000001;
			buttonStringPatterns[4][4] = 0b0000000;
			buttonStringPatterns[4][5] = 0b0000001;

			*controllerMode = NOTES;
		}
		else if (*controllerMode  == NOTES_TO_CHORDS)
		{
		    buttonStringPatterns[0][0] = 0b0001110; // [0][0] through [0][5] - green button
			buttonStringPatterns[0][1] = 0b0001110; // This is a G Chord
			buttonStringPatterns[0][2] = 0b0000000;	
			buttonStringPatterns[0][3] = 0b0000000;
			buttonStringPatterns[0][4] = 0b0000000;
			buttonStringPatterns[0][5] = 0b0001110;

			buttonStringPatterns[1][0] = 0b0000000; // [1][0] through [1][5] - red button
			buttonStringPatterns[1][1] = 0b0001110; // This is a C chord
			buttonStringPatterns[1][2] = 0b0001110;
			buttonStringPatterns[1][3] = 0b0000000;
			buttonStringPatterns[1][4] = 0b0001110;
			buttonStringPatterns[1][5] = 0b0000000;

			buttonStringPatterns[2][0] = 0b0000000; // [2][0] through [2][5] - yellow button
			buttonStringPatterns[2][1] = 0b0000110; // This is an E chord.
			buttonStringPatterns[2][2] = 0b0000110;
			buttonStringPatterns[2][3] = 0b0000110;
			buttonStringPatterns[2][4] = 0b0000000;
			buttonStringPatterns[2][5] = 0b0000000;
									  
			buttonStringPatterns[3][0] = 0b0000000; // [3][0] through [3][5] - blue button
			buttonStringPatterns[3][1] = 0b0000000; // This an A chord
			buttonStringPatterns[3][2] = 0b0000110;
			buttonStringPatterns[3][3] = 0b0000110;
			buttonStringPatterns[3][4] = 0b0000110;
			buttonStringPatterns[3][5] = 0b0000000;

			buttonStringPatterns[4][0] = 0b0000000; // [4][0] through [4][5] - blue button
			buttonStringPatterns[4][1] = 0b0000000;	// This is a D chord 
			buttonStringPatterns[4][2] = 0b0000000;
			buttonStringPatterns[4][3] = 0b0001110;
			buttonStringPatterns[4][4] = 0b0001110;
			buttonStringPatterns[4][5] = 0b0001110;

			*controllerMode = CHORDS;
		}	
	}
}


int main()
/* This controls the whole program.
	 It works under normal mode by first initializing things then looping through the code.
	 -Under normal operation, the loop reads the values on the strings and frets, processes these
	  values against a set patterns to determine what buttons on the guitar hero controller should 
	  be set, sets those buttons in a formatted data structure, then sends that structure to 
	  the actual controller.
	 -Under config mode, the loop reads in the values of the strings and frets, assigns that current
	  string-fret combination to correspond to a button press, then locks that combination in when 
	  'Plus' is pressed and moves on to changing the next button.  It also sets the button data structure
	  to display the current controller button being programmed, and then sends that data to the controller.
*/ 
{

	init(); // Initialize the pins and stuff

	int stringState[6] = {0,0,0,0,0,0}; // This stores an int for each string,
										// with each int acting as a boolean array
										// telling which frets a string is connected to,
										// with the 0th bit being if a string is being strummed

	int buttonStringPatterns[NUMBER_OF_FRETS][6];


  	// Start the wiimote communication and make wiimote think this is a guitar
    wm_init(guitar_id, but_dat, cal_data, wm_timer_inc);

	setTimer(); // Start the timer going

	char configMode = 0; // Start in normal mode
	int buttonToAssign = 0; // declare some variables
	char plusLock = 0;

	char controllerMode = FRETS;
	dataForController data;

    while(1)
    {
        // We first read all the strings for button hits and store them into
		//  our stringState array.  Strum processing is also handled here for now.
        readFrets(first_string,  &stringState[0]);
		readFrets(second_string, &stringState[1]);
		readFrets(third_string,  &stringState[2]);
		readFrets(fourth_string, &stringState[3]);
		readFrets(fifth_string,  &stringState[4]);
		readFrets(sixth_string,  &stringState[5]);

		if ((controllerMode == NOTES) || (controllerMode == CHORDS))
		{
			// Check to see if we're trying to enter Config Mode; disabled if we're playing with frets
			if (configMode == 0)
			{
				configMode = testForConfigMode(stringState, buttonStringPatterns, &controllerMode);
				buttonToAssign = 0; // This variable keeps track of which button we're assigning
				plusLock = 0; // This is a tool to let us test that in the prior cycle, plus was or wasn't on
			}

			// Config Mode operation
			if (configMode == 1) 
			{
				// Allow us to escape config mode by a string combination
				if (testForEndConfigMode(stringState))
				{	
					configMode = 0;
				}
				else
				{
					// Now set up the data packet to the Wii to say we're pressing whatever button we're currently
					// assigning, and if we're pressing 'plus', go ahead and lock that string combination in
					configSetButtons(&buttonToAssign, &plusLock, buttonStringPatterns, stringState, &controllerMode);

					// If we're done assigning buttons  return to normal mode or restat 
					if (buttonToAssign >= 5 && plusLock == 0)
					{
						if (controllerMode == NOTES)
							buttonToAssign = 0;
						else 				// CHORDS mode
							configMode = 0;
		}	}	}	} // End of Config Mode code


		// Normal operation
		if (configMode == 0)
		{
			// First, reset the data struct
			data.orangeOn = 0;
			data.blueOn   = 0;
			data.yellowOn = 0;
			data.redOn    = 0;
			data.greenOn  = 0;
			data.upOn     = 0;
			data.downOn   = 0;
			
			// Then update controllerMode to see if we're trying to change controller modes
			resetStringPatterns(stringState, buttonStringPatterns, &controllerMode);
			

			// Next, using our strumState array, we process that data to figure out what sort
			//  of buttons on the controller we want to emulate pressing.
			if (!(controllerMode % 2))
				processStringState(&data, stringState, buttonStringPatterns, &controllerMode);

			// Now, if we're in a transitional controller mode, we'll forget that data and substitue 
			// visual cue data to show that we're changing to a specific mode
			if (controllerMode == FRETS_TO_NOTES)
			{
				data.greenOn = 1;
				data.yellowOn = 1;
			}
			else if (controllerMode == NOTES_TO_CHORDS)
			{
				data.greenOn = 1;
				data.blueOn = 1;
			}
			else if (controllerMode == CHORDS_TO_FRETS)
			{
				data.greenOn = 1;
				data.orangeOn = 1;
			}
	    

			// Then using those button presses, we set up the 
			// button data packet
		    setButtons(data, &controllerMode);
		}
		
    	// Finally, regardless of mode, we send that packet to the 
		// TWI transfer register, handled by the wiimote.c library
	    wm_newaction(but_dat);

	} // End of while loop

    return 0;
}

inline void processStringState(dataForController* data, int stringState[],
												int buttonStringPatterns[][6], char* controllerMode)
{
	// Give us a place to store our data
//dataForController processedData = newDataForController();

	if (*controllerMode == CHORDS) // If we're playing with Chords - default is FRETS
	{
		for (int guitarString = 0; guitarString < 6; guitarString++)
		 {

			if (stringState[guitarString] & 1)
			{
				stringState[guitarString] &= ~1; //Set the last bit to zero
				if (guitarString < 3)
				{
					data->downOn = 1;
					data->upOn   = 0;
				}
				else
				{
					data->upOn   = 1;
					data->downOn = 0;
				}
			}
		}
	
		// green
		if(stringState[0] == buttonStringPatterns[0][0] &&
		   stringState[1] == buttonStringPatterns[0][1] &&
		   stringState[2] == buttonStringPatterns[0][2] &&
		   stringState[3] == buttonStringPatterns[0][3] &&
		   stringState[4] == buttonStringPatterns[0][4] &&
		   stringState[5] == buttonStringPatterns[0][5]  ) 
			{
				data->greenOn = 1;
	  		}

		// red
		if(stringState[0] == buttonStringPatterns[1][0] &&
		   stringState[1] == buttonStringPatterns[1][1] &&
		   stringState[2] == buttonStringPatterns[1][2] &&
		   stringState[3] == buttonStringPatterns[1][3] &&
		   stringState[4] == buttonStringPatterns[1][4] &&
		   stringState[5] == buttonStringPatterns[1][5]  ) 
			{
				data->redOn = 1;
	  		}
	
		// yellow
		if(stringState[0] == buttonStringPatterns[2][0] &&
		   stringState[1] == buttonStringPatterns[2][1] &&
		   stringState[2] == buttonStringPatterns[2][2] &&
		   stringState[3] == buttonStringPatterns[2][3] &&
		   stringState[4] == buttonStringPatterns[2][4] &&
		   stringState[5] == buttonStringPatterns[2][5]  ) 
			{
				 data->yellowOn = 1;
	  		}

		// blue
		if(stringState[0] == buttonStringPatterns[3][0] &&
		   stringState[1] == buttonStringPatterns[3][1] &&
		   stringState[2] == buttonStringPatterns[3][2] &&
		   stringState[3] == buttonStringPatterns[3][3] &&
		   stringState[4] == buttonStringPatterns[3][4] &&
		   stringState[5] == buttonStringPatterns[3][5]  ) 
			{
				 data->blueOn = 1;
	  		}

		// orange
		if(stringState[0] == buttonStringPatterns[4][0] &&
		   stringState[1] == buttonStringPatterns[4][1] &&
		   stringState[2] == buttonStringPatterns[4][2] &&
		   stringState[3] == buttonStringPatterns[4][3] &&
		   stringState[4] == buttonStringPatterns[4][4] &&
		   stringState[5] == buttonStringPatterns[4][5]  ) 
			{
				 data->orangeOn = 1;
	  		}
	}

	else if (*controllerMode == NOTES) // We're playing with notes
	{
		/* If we're playing with notes on a scale, then we have to figure out if the proper note is being
		   played, and also if we strummed the correct string.
		 */

		// First thing, we need to reset the strumming code, since we need to check and see if we 
		//  strummed on the correct string or not
		data->upOn = 0;
		data->downOn = 0;
		char strumOn = -1; // This holds which string the pick is strumming
		char buttonPressed = 0;
		char strumPressed  = 0;

		for (int i = 0; i < 6; i++)
		{
			if (stringState[i] != 0) // Only look at strings that are being touched or strummed
			{
				// Check for open strings being strummed first
				if (stringState[i] == 1) // This means we're strumming an open string
				{
					if (stringState[i] == buttonStringPatterns[0][i])
					{
						strumOn = i;
						data->greenOn =  1;	
					}
					if (stringState[i] == buttonStringPatterns[1][i])
					{
						strumOn = i;
						data->redOn =  1;	
					}
					if (stringState[i] == buttonStringPatterns[2][i])
					{
						strumOn = i;
						data->yellowOn =  1;
					}				
					if (stringState[i] == buttonStringPatterns[3][i])
					{
						strumOn = i;
						data->blueOn =  1;
					}				
					if (stringState[i] == buttonStringPatterns[4][i])
					{
						strumOn = i;
						data->orangeOn =  1;				
					}

				}
				 // Now we have checked for strummed notes, we need to check for pressed notes
				 //  so that we can display them for reference as well as use them when strummed
				else
					// First see if we're strumming or not
					strumPressed = stringState[i] & 1;

					// Now ensure the strum bit is zero
					stringState[i] &= (~1);

					// Take care of displaying closed notes
					if (stringState[i] == buttonStringPatterns[0][i]&& 
											buttonStringPatterns[0][i] != 0)
					{
						data->greenOn =  1;	
						buttonPressed = 1;
					}
					if (stringState[i] == buttonStringPatterns[1][i] && 
											buttonStringPatterns[1][i] != 0)
					{
						data->redOn =  1;	
						buttonPressed = 1;
					}
					if (stringState[i] == buttonStringPatterns[2][i] && 
											buttonStringPatterns[2][i] != 0)
						{
						data->yellowOn =  1;
						buttonPressed = 1;
					}
					if (stringState[i] == buttonStringPatterns[3][i] && 
											buttonStringPatterns[3][i] != 0)
					{
						data->blueOn =  1;
						buttonPressed = 1;
						}				
					if (stringState[i] == buttonStringPatterns[4][i] && 
											buttonStringPatterns[4][i] != 0)
					{
						data->orangeOn =  1;
						buttonPressed = 1;									
					}
						// Now deal with strumming
					if ((buttonPressed = 1) && (strumPressed == 1))
					{
						strumOn = i;
					}
				}
		}

		//Now set the strum buttons properly

		if ((strumOn >= 0) && (strumOn < 3))
		{
			data->downOn = 1;
		}
		else if ((strumOn >= 3) && (strumOn < 6 ) && (data->downOn != 1))
		{
			data->upOn = 1;
		}
	}
	else // We're playing with frets
	{

		/* Then we use that information to see what buttons we need to press.
			In order for a button to be pressed, the frets on either side must
			be pressed.  This is complicated by the green fret, since there's only
			one fret there, so when you press red, if the green fret contacts first,
			you don't want that to register as a green button press.  So we do some
			timing on that one, so when you press just green, it starts counting
			instead of turing greenOn on, so if the red fret is turned on before it
			counts up to the GREEN_TIMER_WAIT, red gets activated and green doesn't.
		 */

		for(int guitarString = 0; guitarString < 6; guitarString++)
		{
			// find the proper value by masking the stringState variable for each fret
			char orange = (stringState[guitarString] & (1<<5));
			char blue   = (stringState[guitarString] & (1<<4));
			char yellow = (stringState[guitarString] & (1<<3));
			char red    = (stringState[guitarString] & (1<<2));
			char green  = (stringState[guitarString] & (1<<1));

			if ((orange != 0) && (blue != 0) && (data->orangeOn == 0))
			{    
			    data->orangeOn = 1;
			    triedGreen[guitarString] = 0;
			}
			else if ((blue != 0) && (yellow != 0) && (data->blueOn == 0))
			{
			    data->blueOn = 1;
			    triedGreen[guitarString] = 0;
			}
			else if ((yellow != 0) && (red != 0) && (data->yellowOn == 0))
			{
			    data->yellowOn = 1;
			    triedGreen[guitarString] = 0;
			}
			else if ((red != 0) && (green != 0) && (data->redOn == 0))
			{
			    data->redOn = 1;
			    triedGreen[guitarString] = 0;
			}
			else if (green != 0)
			{
			    if (triedGreen[guitarString] == 0) // we didn't touch green last cycle
			    {        
					// so we start the counter and say we did touch it.
			      greenTimers[guitarString] = TCNT1;
			      triedGreen[guitarString] = 1;
			    }
			    else //triedGreen == true
			    {
					// now we test to see if the counter has counted high enough, and
					// if it has, then we do record a green hit.
					int test = TCNT1 - greenTimers[guitarString];
			    	if (  (data->greenOn == 0) && (( test > GREEN_TIMER_WAIT) || (test <= 0)) )
			    	{
			        	data->greenOn = 1;
			    	}
			    }
			}
 
		    else  //no buttons pressed
		    {
		        triedGreen[guitarString] = 0;
			}

		/*  Strum Code - for now, we're handling it right here...
			This is just like the frets, except that
			the strum has a direction, so if you strum the
			top 3 strings, you strum down, and if you strum
			the bottom 3 strings, you're strumming up.
		*/
			// Check for strumming
			if (stringState[guitarString] & 1)
			{
				if (guitarString < 3)
				{
					data->downOn = 1;
					data->upOn   = 0;
				}
				else
				{
					data->upOn   = 1;
					data->downOn = 0;
				}
			}
		}	// end of for loop
	}

	return;	
}

