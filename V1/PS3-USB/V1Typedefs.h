/* V1Typedefs.h
      Copyright 2009
	     Alan Chatham

	This program is distributed under the terms of the GNU General Public License, v3.

	Further details of licenses and acknowledgements can be found in V1.h
*/

#ifndef V1_TYPEDEFS
#define V1_TYPEDEFS

	// This struct is used to store abstract data about what the controller's state is.
	// It gets filled by the functions in main.c and then gets passed to the ___interface.c files
	typedef struct dataForController
	{
		char greenOn;  // variables to abstractly tell us which buttons are pressed		
		char redOn;
		char yellowOn;
		char blueOn;
		char orangeOn;
		char upOn;
		char downOn;
		char plusOn;
		char minusOn;
	} dataForController;

	inline void clearData(dataForController* data)
	{
		data->orangeOn = 0;
		data->blueOn   = 0;
		data->yellowOn = 0;
		data->redOn    = 0;
		data->greenOn  = 0;
		data->upOn     = 0;
		data->downOn   = 0;
		data->plusOn   = 0;
		data->minusOn  = 0;
	};

	// Macro for No Operation assembly instructions
	#define nop() asm volatile("nop")

	// Macro to as many NOPs as we want
	inline void doXnops(int x)
	{
		while (x >= 0)
		{
			nop();
			x--;
		}
	}

#endif
