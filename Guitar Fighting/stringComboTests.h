/* stringComboTests.h
    part of the OpenChord V1 AVR code
		www.OpenChord.org

		Alan Chatham
		Copyright 2009

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef STRING_COMBO_TESTS
#define STRING_COMBO_TESTS

// This function tests to see if you're pressing the top 4 strings on the orange fret,
//  thereby activating config mode. Works with both chords and scales.
inline char testForConfigMode(int stringState[])
{
	if ((stringState[0] == 0b00100000) &&
		(stringState[1] == 0b00100000) &&
		(stringState[2] == 0b00100000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
		return 1;
	else
		return 0;
}

//This function tests to see if we're pressing the correct string combination to end config mode
inline char testForEndConfigMode(int stringState[])
{
	if ((stringState[0] == 0b00000000) &&
		(stringState[1] == 0b00100000) &&
		(stringState[2] == 0b00100000) &&
		(stringState[3] == 0b00100000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
		return 1;
	else
		return 0;
}

inline int testForSwitchModes(int stringState[])
{
	// Test for the right keypress to start the mode
	if ((stringState[0] == 0b00000000) &&
		(stringState[1] == 0b00000000) &&
		(stringState[2] == 0b00100000) &&
		(stringState[3] == 0b00100000) &&
		(stringState[4] == 0b00100000) &&
		(stringState[5] == 0b00000000))
	    return 1;
    else
		return 0;
}

inline int testForResetButtonPatterns(int stringState[])
{
	if ((stringState[0] == 0b00100000) &&
		(stringState[1] == 0b00000000) &&
		(stringState[2] == 0b00100000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00100000) &&
		(stringState[5] == 0b00000000))
		return 1;
	else
		return 0;
}

inline int testForMinusChord(int stringState[])
{
	if ((stringState[0] == 0b00000000) &&
		(stringState[1] == 0b00000000) &&
		(stringState[2] == 0b00000000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00100000))
		return 1;
	else
		return 0;
}

inline int testForHomeChord(int stringState[])
{
	if ((stringState[0] == 0b00100000) &&
		(stringState[1] == 0b00000000) &&
		(stringState[2] == 0b00000000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00100000))
		return 1;
	else
		return 0;
}

inline int testForNoStringsPressed(int stringState[])
{
	if ((stringState[0] == 0b00000000) &&
		(stringState[1] == 0b00000000) &&
		(stringState[2] == 0b00000000) &&
		(stringState[3] == 0b00000000) &&
		(stringState[4] == 0b00000000) &&
		(stringState[5] == 0b00000000))
		return 1;
	else
		return 0;
}

#endif
