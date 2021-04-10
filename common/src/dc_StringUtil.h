// dc_StringUtil.h

#ifndef _DC_STRINGUTIL_h
#define _DC_STRINGUTIL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


String getStringValueFromDelimitedString(String data, char separator, int index);

std::string  getStdStringValueFromDelimitedString(String data, char separator, int index);

int getIntegerValueFromDelimitedString(String data, char separator, int index);

#endif

