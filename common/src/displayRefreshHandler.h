// displayRefreshHandler.h

#ifndef _DISPLAYREFRESHHANDLER_h
#define _DISPLAYREFRESHHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class displayRefreshHandler {

public:
	long  LastRefreshTimestampMs = 0;
	int RefreshIntervalMs = 1000;

	displayRefreshHandler(int refreshIntervalMs);
	bool refreshRequired();
	void updateRefreshTimestamp();
	void setRefreshInterval(int refreshIntervalMs);
};

#endif

