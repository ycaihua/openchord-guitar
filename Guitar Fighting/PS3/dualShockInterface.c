/* ps3interface.c
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

#include "dualShockInterface.h"

//code for 12.8 mhz option
#if F_CPU == 12800000
static void calibrateOscillator(void)
{
uchar       step = 128;
uchar       trialValue = 0, optimumValue;
int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
 
    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    // proportional to current real frequency
        if(x < targetValue)             // frequency still too low
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; // this is certainly far away from optimum
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
 
void    usbEventResetReady(void)
{
    cli();  // usbMeasureFrameLength() counts CPU cycles, so disable interrupts.
    calibrateOscillator();
    sei();
    eeprom_write_byte(0, OSCCAL);   // store the calibrated value in EEPROM
}

#endif
//end 12.8 mhz code

// These are which bits (from left to right) in the byte 0 of the packet
// correspond to which buttons
#define TRIANGLE_BIT 0
#define CIRCLE_BIT   1
#define CROSS_BIT    2
#define SQUARE_BIT   3
#define L1_BIT       4
#define R1_BIT       5
#define L2_BIT       6
#define R2_BIT       7


// These buttons are in the byte 1
#define START_BIT   1
#define SELECT_BIT  2
#define HOME_BIT    4

/* The data being transferred is in the following format:
	byte0 - 8 buttons ( bit0...bit7 =
				[Triangle/Jab][Circle/Short][Cross/Forward][Square/Strong][L1/4th punch][R1/Fierce][L2/4th kick][R2/Roundhouse]
	byte1 - 5 buttons, 3bits padding bit0...bit7 = 
				[Select][Start][Left Analog Click?][Right Analog Click?],[PS3 Home Button],[??][??][??]
	byte2 - hat switch - D pad
			0000 N, 0001 N/E, 0010 E, 0011 SE, 0100 S, 0101 SW, 0110 W, 0111, NW, 1000, nothing pressed
	byte3 - left stick x axis (left is 0x00, right is 0xFF)
	byte4 - left stick y axis (up is 0x00, down is 0xFF)
	byte5 - z axis (??)
	byte6 - rzaxis (??)

	Note: These mappings are incomplete, and possibly incorrect.
*/

PROGMEM char usbHidReportDescriptor[84] = { // PC HID Report Descriptor
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //   PHYSICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x0d,                    //   REPORT_COUNT (13)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x0d,                    //   USAGE_MAXIMUM (Button 13)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
/* report bits: 13x1=13 */
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
/* report bits: + 3x1=3 */
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,              //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,                    //   REPORT_SIZE (4)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x65, 0x14,                    //   UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,                    //   USAGE (Hat switch)
    0x81, 0x42,                    //   INPUT (Data,Var,Abs,Null)
/* report bits: + 1x4=4 */
    0x65, 0x00,                    //   UNIT (None)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
/* report bits: + 1x4=4 */
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x09, 0x30,                    //   USAGE (X)
    0x09, 0x31,                    //   USAGE (Y)
    0x09, 0x32,                    //   USAGE (Z)
    0x09, 0x35,                    //   USAGE (Rz)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
/* report bits: + 4x8=32 */
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
    0x0a, 0x21, 0x26,              //   UNKNOWN
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)

    0xc0                           // END_COLLECTION  
};

uchar reportBuffer[7];


/* ------------------------------------------------------------------------- */
static uchar buffer[8];

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    /* class request */
		/* wValue: ReportType (highbyte), ReportID (lowbyte) */
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			 // set buffer data
			buffer[0] = 33;
			buffer[1] = 38;
			buffer[2] =
			buffer[3] =
			buffer[4] =
			buffer[5] = 
			buffer[6] =
			buffer[7] = 0;
			usbMsgPtr = buffer;

			return 8; // send 8 bytes	
        }
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

void startDualShockCommunication(void)
{
    // First, set up all the USB communication stuff
    wdt_enable(WDTO_1S);
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!

     * RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */

    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    uchar i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    sei();
	usbPoll();
}


void sendDualShockData(dataForController data)
{
        //set the buttons to all at default positions
        reportBuffer[0] = 0b00000000;
        reportBuffer[1] = 0b00000000;
        reportBuffer[2] = 0b00001000;
        reportBuffer[3] = 0b10000000;
        reportBuffer[4] = 0b10000000;
        reportBuffer[5] = 0b10000000;
        reportBuffer[6] = 0b10000000;

/* The data being transferred is in the following format:
	byte0 - 8 buttons ( bit0...bit7 =
				[Triangle/Jab][Circle/Short][Cross/Forward][Square/Strong][L1/4th punch][R1/Fierce][L2/4th kick][R2/Roundhouse]
	byte1 - 5 buttons, 3bits padding bit0...bit7 = 
				[Select][Start][Left Analog Click?][Right Analog Click?],[PS3 Home Button],[??][??][??]
	byte2 - bits 0-3 are the hat switch/D pad, bits 4-7 are unknown
			0000 N, 0001 N/E, 0010 E, 0011 SE, 0100 S, 0101 SW, 0110 W, 0111 NW, 1000, nothing pressed
	byte3 - left stick x axis (left is 0x00, right is 0xFF)
	byte4 - left stick y axis (up is 0x00, down is 0xFF)
	byte5 - z axis (??)
	byte6 - rzaxis (??)

	Note: These mappings are incomplete, and possibly incorrect.
*/

		// First, attack buttons are going to be done with strumming - 
		//  Punch - Strum Down
		//  Kick  - Strum Up

		if (data.upOn == 1)
			reportBuffer[0] = 0b00000010;
		if (data.downOn == 1)
			reportBuffer[0] = 0b00000001;


        //We're going to map the colors as such:
		// Green - Left
		// Red -   Down-Left if less than 3 strings are pressed, Up-Left if 3+ strings are pressed
		// Yellow - Down or Up, using the same conditions as above
		// Blue -  Down-Right or Up-Right, again
		// Orange - Right

		if (data.greenOn == 1)
		{
			reportBuffer[2] |= 0b0110;
			reportBuffer[3] = 0x00;
		}

		if (data.redOn == 1)
		{
			if (data.numberOfStringsPressed >=3)
			{
				reportBuffer[2] = 0b0111; // D-Pad Up-Left
				reportBuffer[4] = 0x00; //Analog Up
			}
			else
			{
				reportBuffer[2] = 0b0101; //D-Pad Down-Left
				reportBuffer[4] = 0xFF; //Analog Down
			}
			reportBuffer[3] = 0x00; //Analog Left
		}

		if (data.yellowOn == 1)
		{
			if (data.numberOfStringsPressed >=3)
			{
				reportBuffer[2] = 0b0000; // D-Pad Up
				reportBuffer[4] = 0x00; //Analog Up
			}
			else
			{
				reportBuffer[2] = 0b0100; //D-Pad Down
				reportBuffer[4] = 0xFF; //Analog Down
			}
		}

		if (data.blueOn == 1)
		{
			if (data.numberOfStringsPressed >=3)
			{
				reportBuffer[2] = 0b0001; // D-Pad Up-Right
				reportBuffer[4] = 0x00; //Analog Up
			}
			else
			{
				reportBuffer[2] = 0b0011; //D-Pad Down-Right
				reportBuffer[4] = 0xFF; //Analog Down
			}
			reportBuffer[3] = 0xFF; //Analog Right
		}

		if (data.orangeOn == 1)
		{
			reportBuffer[2] = 0b0010; //D-Pad Right
			reportBuffer[3] = 0xFF; //Analog Right
		}

		//Finally, Start, Star Power, and the Home button (not working yet)
		reportBuffer[1] |= (data.plusOn << START_BIT);
		reportBuffer[1] |= (data.homeOn << HOME_BIT);

        //Then we finish off some USB stuff.
        wdt_reset();  //Reset the watchdog timer
        usbPoll();    //USB poll - must be called at least once per 10ms

        // Now, regardless of mode, we have to send the data to the 
        // USB controlling library from the buffer we've been building
        if(usbInterruptIsReady())
        {
            /* called after every poll of the interrupt endpoint */
            usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
        }
                

}
