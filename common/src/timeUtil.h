#ifndef _TIMEUTIL_h
#define _TIMEUTIL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//#include <TimeLib.h>
#ifndef Wifi
#include <WiFi.h>
#endif // !Wifi

#ifndef time
#include <Time.h>
#endif // !time

#ifndef esp_timer
#include <esp_timer.h>
#endif // !esp_timer

#ifndef ESP32Time
#include <ESP32Time.h>
#endif

#ifndef _DC_STRINGUTIL_h
#include "dc_StringUtil.h"
#endif

std::string getCurrentSystemTimeStdString();

	void setSystemTime(String dateTimeValue);
	String  getCurrentNetworkTimeString();

	String  getCurrentSystemTimeString();

	void setNeworkTime();

	void  printLocalTime();
  void printAccurateTime();

#endif