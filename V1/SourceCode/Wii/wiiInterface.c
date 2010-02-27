
#include "wiiinterface.h"

// controller id
//const unsigned char drum_id[6] = {0x01, 0x00, 0xA4, 0x20, 0x01, 0x03};
const unsigned char guitar_id[6] = {0x00, 0x00, 0xA4, 0x20, 0x01, 0x03};
//const unsigned char nunchuk_id[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned char but_dat[6]; // struct containing button data

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
void wm_timer_inc()  // This function is unused by me.
{
        return;
}

void startCommunication(void)
{
  	// Start the wiimote communication and make wiimote think this is a guitar
    wm_init(guitar_id, but_dat, cal_data, wm_timer_inc);
}


// This function takes the 'color'On values set by readF and sets up the but_dat array accordingly
void sendData(dataForController data)
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

	// check for plus and minus
	char plusOn = !(PLUS_PIN & (1<<PLUS_PIN_NUM)); //Plus and minus are held high normally, but pressing sets it low
	char minusOn = !(MINUS_PIN & (1<<MINUS_PIN_NUM));

	but_dat[4] &= ~(plusOn << PLUS_BIT);
	but_dat[4] &= ~(minusOn << MINUS_BIT);
}

