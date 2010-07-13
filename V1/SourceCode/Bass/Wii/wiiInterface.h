
#ifndef WIIINTERFACE

#include "../V1.h"
#include "macros.h"
#include "wiimote.h"

#ifndef WII
  #error "You're trying to compile for Wii, but don't have WII defined in configAndConstants.h"
#endif



// This function does all the communication initialization
void startWiiCommunication(void);

// This one goes ahead and changes the data
void sendWiiData(dataForController);


#define WIIINTERFACE
#endif
