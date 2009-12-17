/* processStringState.c
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/
#include "processStringState.h"

void processChords(dataForController* data, int stringState[NUMBER_OF_STRINGS],
									int buttonStringPatterns[][NUMBER_OF_STRINGS])
{
	// First take care of registering strumming
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

	// First, save us some trouble and see if we're not pressing anything at all..
	if ((stringState[0] == 0b00000000) &&
		(stringState[1] == 0b00000000) &&
		(stringState[2] == 0b00000000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
		return;

	// Now see if what's being played on the fretboard matches any of the button patterns in
	//  buttonStringPatterns
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
	else if(stringState[0] == buttonStringPatterns[1][0] &&
	   stringState[1] == buttonStringPatterns[1][1] &&
	   stringState[2] == buttonStringPatterns[1][2] &&
	   stringState[3] == buttonStringPatterns[1][3] &&
	   stringState[4] == buttonStringPatterns[1][4] &&
	   stringState[5] == buttonStringPatterns[1][5]  ) 
		{
			data->redOn = 1;
  		}

	// yellow
	else if(stringState[0] == buttonStringPatterns[2][0] &&
	   stringState[1] == buttonStringPatterns[2][1] &&
	   stringState[2] == buttonStringPatterns[2][2] &&
	   stringState[3] == buttonStringPatterns[2][3] &&
	   stringState[4] == buttonStringPatterns[2][4] &&
	   stringState[5] == buttonStringPatterns[2][5]  ) 
		{
			 data->yellowOn = 1;
  		}
	// blue
	else if(stringState[0] == buttonStringPatterns[3][0] &&
	   stringState[1] == buttonStringPatterns[3][1] &&
	   stringState[2] == buttonStringPatterns[3][2] &&
	   stringState[3] == buttonStringPatterns[3][3] &&
	   stringState[4] == buttonStringPatterns[3][4] &&
	   stringState[5] == buttonStringPatterns[3][5]  ) 
		{
			 data->blueOn = 1;
  		}
	// orange
	else if(stringState[0] == buttonStringPatterns[4][0] &&
	   stringState[1] == buttonStringPatterns[4][1] &&
	   stringState[2] == buttonStringPatterns[4][2] &&
	   stringState[3] == buttonStringPatterns[4][3] &&
	   stringState[4] == buttonStringPatterns[4][4] &&
	   stringState[5] == buttonStringPatterns[4][5]  ) 
		{
			 data->orangeOn = 1;
  		}
	//Green and Red
	else if(stringState[0] == buttonStringPatterns[5][0] &&
	   stringState[1] == buttonStringPatterns[5][1] &&
	   stringState[2] == buttonStringPatterns[5][2] &&
	   stringState[3] == buttonStringPatterns[5][3] &&
	   stringState[4] == buttonStringPatterns[5][4] &&
	   stringState[5] == buttonStringPatterns[5][5]  ) 
		{
			data->greenOn = 1;
			data->redOn = 1;
  		}
	//Red and Yellow
	else if(stringState[0] == buttonStringPatterns[6][0] &&
	   stringState[1] == buttonStringPatterns[6][1] &&
	   stringState[2] == buttonStringPatterns[6][2] &&
	   stringState[3] == buttonStringPatterns[6][3] &&
	   stringState[4] == buttonStringPatterns[6][4] &&
	   stringState[5] == buttonStringPatterns[6][5]  ) 
		{
			data->redOn = 1;
			data->yellowOn = 1;
  		}
	// Yellow and Blue
	else if(stringState[0] == buttonStringPatterns[7][0] &&
	   stringState[1] == buttonStringPatterns[7][1] &&
	   stringState[2] == buttonStringPatterns[7][2] &&
	   stringState[3] == buttonStringPatterns[7][3] &&
	   stringState[4] == buttonStringPatterns[7][4] &&
	   stringState[5] == buttonStringPatterns[7][5]  ) 
		{
			 data->yellowOn = 1;
			 data->blueOn = 1;
  		}
	// Green and Yellow
	else if(stringState[0] == buttonStringPatterns[8][0] &&
	   stringState[1] == buttonStringPatterns[8][1] &&
	   stringState[2] == buttonStringPatterns[8][2] &&
	   stringState[3] == buttonStringPatterns[8][3] &&
	   stringState[4] == buttonStringPatterns[8][4] &&
	   stringState[5] == buttonStringPatterns[8][5]  ) 
		{
			 data->greenOn = 1;
			 data->yellowOn = 1;
  		}
	// Red and Blue
	else if(stringState[0] == buttonStringPatterns[9][0] &&
	   stringState[1] == buttonStringPatterns[9][1] &&
	   stringState[2] == buttonStringPatterns[9][2] &&
	   stringState[3] == buttonStringPatterns[9][3] &&
	   stringState[4] == buttonStringPatterns[9][4] &&
	   stringState[5] == buttonStringPatterns[9][5]  ) 
		{
			 data->redOn = 1;
			 data->blueOn = 1;
  		}

	return;
}

void processNotes(dataForController* data, int stringState[NUMBER_OF_STRINGS],
									int buttonStringPatterns[][NUMBER_OF_STRINGS])
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

	//We also hardcode in a down strum on the empty 6th string, for navigation
	if (stringState[0] &= 1)
	{
		data->downOn = 1;
	}

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

	//Now set the strum buttons properly, so we can have both up and down if we're in this mode
	if ((strumOn >= 0) && (strumOn < 3))
	{
		data->downOn = 1;
	}
	else if ((strumOn >= 3) && (strumOn < 6 ) && (data->downOn != 1))
	{
		data->upOn = 1;
	}

	return;
}

void processFrets(dataForController* data, int stringState[NUMBER_OF_STRINGS],
									int buttonStringPatterns[][NUMBER_OF_STRINGS], 
									int greenTimers[NUMBER_OF_STRINGS], int triedGreen[NUMBER_OF_STRINGS])
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
	return;
}

void processStringState(dataForController* data, int stringState[NUMBER_OF_STRINGS],
									int buttonStringPatterns[][NUMBER_OF_STRINGS], char controllerMode, 
									int greenTimers[NUMBER_OF_STRINGS], int triedGreen[NUMBER_OF_STRINGS])
{
	if (controllerMode == CHORDS) // If we're playing with Chords - default is FRETS
		processChords(data, stringState, buttonStringPatterns);

	else if (controllerMode == NOTES) // We're playing with notes
		processNotes(data, stringState, buttonStringPatterns);
	
	else // We're playing with frets
		processFrets(data, stringState,	buttonStringPatterns, greenTimers, triedGreen);

	return;	
}

// This simple function takes in the controller mode and sets the
//  data struct to certain patterns to give the user feedback
//  as to what state is being pressed
void displayTransitionState(dataForController* data, int controllerMode)
{
	if (controllerMode == FRETS_TO_NOTES)
	{
		data->blueOn = 1;
		data->yellowOn = 1;
	}
	else if (controllerMode == NOTES_TO_CHORDS)
	{
		data->blueOn = 1;
		data->orangeOn = 1;
	}
	else if (controllerMode == CHORDS_TO_FRETS)
	{
		data->yellowOn = 1;
		data->orangeOn = 1;
	}
}


/* This function sets up the data struct when we're in config mode so that it shows
 *  the user which colors are currently being programmed.
 */
void configSetButtonData(dataForController* data, int* buttonToAssign, int buttonStringPatterns[][6], 
						int stringState[], char controllerMode, char* plusLock, char* previousPlusOn, int* debounceTimer)
{
	// Since 'colorOn' is 1 when a fret is pressed, we invert, shift, invert, and & it to set the
	// corresponding button bit to zero, which corresponds to a button press.
	if (*buttonToAssign == 0)
		data->greenOn = 1;
	else if(*buttonToAssign == 1)
		data->redOn = 1;
	else if(*buttonToAssign == 2)
		data->yellowOn = 1;
	else if(*buttonToAssign == 3)
		data->blueOn = 1;
	else if(*buttonToAssign == 4)
		data->orangeOn = 1;
	else if(*buttonToAssign == 5)
	{
		data->greenOn = 1;
		data->redOn = 1;
	}
	else if(*buttonToAssign == 6)
	{
		data->redOn = 1;
		data->yellowOn = 1;
	}
	else if(*buttonToAssign == 7)
	{
		data->yellowOn = 1;
		data->blueOn = 1;
	}
	else if(*buttonToAssign == 8)
	{
		data->greenOn = 1;
		data->yellowOn = 1;
	}
	else if(*buttonToAssign == 9)
	{
		data->redOn = 1;
		data->blueOn = 1;
	}



	// Code to debounce our plus button, the current state of plusOn to it's recent history
	if (data->plusOn != 0)  // So we're pressing plus
	{ 
		if (*previousPlusOn == 0)  // If we weren't pressing plus last time through the main loop
		{ 
			*debounceTimer = TCNT1;  // Set up a timer
			*previousPlusOn = 1;     // remember plus was pressed for the next iteration
			data->plusOn = 0;        // but for now, consider plus not pressed.
		}
		else if ( (TCNT1 - *debounceTimer < DEBOUNCE_DELAY) && (TCNT1 - *debounceTimer >= 0))
		{
			data->plusOn = 0;  // This keeps telling us plus wasn't pressed until it's been held long enough
		}		
	}
	else
	{
		*previousPlusOn = 0; // and if plus wasn't pressed, remember that for next time
	}

	// Now, if you press plus, it will lock in whatever string pattern
	// and advance to working on the next pattern.
	if((data->plusOn) && (*plusLock == 0))
	{

		if (controllerMode == CHORDS) 
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

		*plusLock = 1;  // we set up a lock, so we don't assign all the buttons at once
		*buttonToAssign += 1;
	}
	else if(data->plusOn == 0)
	{
		*plusLock = 0;
	}
	return;
}
