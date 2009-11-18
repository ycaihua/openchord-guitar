
#ifndef WIIINTERFACE

#include "../V1.h"
#include "macros.h"
#include "wiimote.h"

#ifndef WII
#error Please change your definition of controllers in V1.h
#endif



// This function does all the communication initialization
void startCommunication(void);

// This one goes ahead and changes the data
void sendData(dataForController);


#define WIIINTERFACE
#endif
