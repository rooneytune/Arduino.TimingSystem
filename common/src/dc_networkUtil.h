// dc_networkUtil.h

#ifndef _DC_NETWORKUTIL_h
#define _DC_NETWORKUTIL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

  // compare mac address
	bool macAddressMatched(uint8_t* mac1, uint8_t* mac2);
#endif

