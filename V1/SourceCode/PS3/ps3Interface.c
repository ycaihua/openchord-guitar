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

#include "ps3interface.h"

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
#define GREEN_BIT  1
#define RED_BIT    2
#define YELLOW_BIT 3
#define BLUE_BIT   0
#define ORANGE_BIT 4
#define MINUS_BIT  5 // Star power

// These buttons are in the byte 1
#define PLUS_BIT   1
#define HOME_BIT   4

/* The data being transferred is in the following format:
	byte0 - 8 buttons ( bit0...bit7 =
				[blue][green][red][yellow][orange][star power][is higher frets][unused]
	byte1 - 5 buttons, 3bits padding bit0...bit7 = 
				[select][start][??][??],[ps3 button],[pad][pad][pad]
	byte2 - hat switch 
			0000 N, 0001 N/E, 0010 E, 0011 SE, 0100 S, 0101 SW, 0110 W, 0111, NW, 1000, nothing pressed
	byte3 - x axis (unused)
	byte4 - y axis (unused)
	byte5 - z axis (Whammy Bar)
	byte6 - rzaxis (Choose solo style)
*/

PROGMEM char usbHidReportDescriptor[74] = { // USB report descriptor, size must match usbconfig.h
  0x05,0x01, // usage page (generic desktop)
  0x09,0x05, // Usage (Joystick)
  0xa1,0x01, // Collection (Application)
  0x15,0x00,     // Logical minimum (0) 10
  0x25,0x01,     // Logical maximum (1)
  0x35,0x00,     // Physical minimum (0)
  0x45,0x01,     // Physical maximum (1)
  0x75,0x01,     // Report size (1)
  0x95,0x0d,     // Report Count (13) 20
  0x05,0x09,     // Usage page (button)
  0x19,0x01,     // Usage Minimum (button 1)
  0x29,0x0d,     // Usage Maximum (button 13)
  0x81,0x02,     // Input (Data, Variable, Abs)
  0x95,0x03,     // Report size (3) 30
  0x81,0x01,     // Input (data,var,abs)
  0x05,0x01,     // Usage Page (Generic Desktop)
  0x25,0x07,     // Logical Minimum (7)
  0x46,0x3b,0x01,// physical max 315
  0x75,0x04,     // report size (4) 41
  0x95,0x01,     // report count (1)
  0x65,0x14,     // Unit (eng rot:angular pos),
  0x09,0x39, // Usage (Hat switch)  
  0x81,0x42, // INPUT (Data,Var,Abs,Null)
  0x65,0x00, //49 unit (none) 
  0x95,0x01, // report count 1  
  0x81,0x01,// Input (Cnst, Ary, Abs)
  0x26,0xff,0x00, // logical maximum 255
  0x46,0xff,0x00, // physical maximum 255 
  0x09,0x30, //61 usage x  
  0x09,0x31, // usage y
  0x09,0x32, // usage z
  0x09,0x35, // usage Rz
  0x75,0x08, // report size 8 
  0x95,0x04, //71 report count 4 
  0x81,0x02, // Input (Data,Var,Abs)
  0xC0      };      // End Collection,End Collection   

uchar reportBuffer[7];

static uchar    idleRate;   /* repeat rate for keyboards, never used for mice */


/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    /* The following requests are never used. But since they are required by
     * the specification, we implement them in this example.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        //DBG1(0x50, &rq->bRequest, 1);   /* debug output: print our request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            usbMsgPtr = (void *)&reportBuffer;
            return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* no vendor specific requests implemented */
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

void startPS3Communication(void)
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


void sendPS3Data(dataForController data)
{
/* The data being transferred is in the following format:
	byte0 - 8 buttons ( bit0...bit7 =
				[blue][green][red][yellow][orange][star power][is higher frets][unused]
	byte1 - 5 buttons, 3bits padding bit0...bit7 = 
				[select][start][??][??],[ps3 button],[pad][pad][pad]
	byte2 - hat switch 
			0000 N, 0001 N/E, 0010 E, 0011 SE, 0100 S, 0101 SW, 0110 W, 0111, NW, 1000, nothing pressed
	byte3 - x axis (unused)
	byte4 - y axis (unused)
	byte5 - z axis (Whammy Bar)
	byte6 - rzaxis (Choose solo style)
*/
       //set the buttons to all at default positions
        reportBuffer[0] = 0b00000000;
        reportBuffer[1] = 0b00000000;
        reportBuffer[2] = 0b00001000;
        reportBuffer[3] = 0b10000000;
        reportBuffer[4] = 0b10000000;
        reportBuffer[5] = 0b10000000;
        reportBuffer[6] = 0b00000000;


        // Since 'colorOn' is 1 when a fret is pressed, we shift it to the proper place and | it to set the
        // corresponding button bit to one, which corresponds to a button press.
        reportBuffer[0] |= (data.orangeOn << ORANGE_BIT);
        reportBuffer[0] |= (data.blueOn << BLUE_BIT);
        reportBuffer[0] |= (data.yellowOn << YELLOW_BIT);
        reportBuffer[0] |= (data.redOn << RED_BIT);
        reportBuffer[0] |= (data.greenOn << GREEN_BIT);

        // On the PS3, the strumming shows up as a hat switch, so we have to use conditionals for this..
        if (data.upOn)
                reportBuffer[2] = 0b00000000;
        if (data.downOn)
                reportBuffer[2] = 0b00000100;

		//Finally, Start, Star Power, and the Home button (not working yet)
		reportBuffer[1] |= (data.plusOn << PLUS_BIT);
        reportBuffer[0] |= (data.minusOn << MINUS_BIT);
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
