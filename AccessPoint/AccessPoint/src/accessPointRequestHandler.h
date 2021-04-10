// accessPointRequestHandler.h

#ifndef _ACCESSPOINTREQUESTHANDLER_h
#define _ACCESSPOINTREQUESTHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifndef _ESPAsyncWebServer_H_
#include "ESPAsyncWebServer.h"
#endif // !_ESPAsyncWebServer_H_

#ifndef WEBSOCKETSSERVER_H_
#include "WebSocketsServer.h"
#endif // !WEBSOCKETSSERVER_H_

#ifndef _ARDUINO_JSON_H_
#include "Arduino_JSON.h"
#endif // !_ARDUINO_JSON_H_



#ifndef __ESP_NOW_H__
#include <esp_now.h>
#endif // !__ESP_NOW_H__

#ifndef WiFi
#include "WiFi.h"
#endif // !WiFi

#ifndef AsyncClient
#include <AsyncTCP.h>
#endif // !AsyncClient

#ifndef SPIFFS
#include "SPIFFS.h"
#endif // !SPIFFS

#ifndef _STL_VECTOR_H
#include <vector>
#endif // !_STL_VECTOR_H


#ifndef time
#include <Time.h>
#endif // !time

#ifndef esp_timer
#include <esp_timer.h>
#endif // !esp_timer

//
#ifndef displayRefreshHandler_h
	#include "displayRefreshHandler.h"
#endif 

#ifndef timeutil_h

#include <timeUtil.h>
#endif

#ifndef _dc_common_h
	#include "dc_common.h"
	
#endif

#ifndef _DC_NETWORKUTIL_h
	#include "dc_networkUtil.h"
#endif

#ifndef esp_wifi_type_h

#include "esp_wifi_types.h"
#endif

//
//
#ifndef _dc_EventManager_h
	#include "dc_EventManager.h"
#endif //



//#ifndef testClass.H
//#include <testClass.h>
//#endif 

void eventManager_set(EventManager* manager);
void sendRssiSettingsToClient(const uint8_t& client_num);

void webSocketLoop();

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t* payload, size_t length);

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest* request);

// Callback: send rssiResults 
void onRssiResultsRequest(AsyncWebServerRequest* request);

void on(AsyncWebServerRequest* request);

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest* request);

void onPageNotFound(AsyncWebServerRequest* request);

void initialiseWebServer();
void initialiseWebSocketServer();

void initialiseAccessPoint();


void addRssiScanResult(rssiScanResult scan);
void clearRssiScanResultSet();

rssiScanResult generateRssiLog(char distance[15], int rssi, char timeStamp[20], char macAddress[20], int messageCount);

void promiscuous_rx_cb(void* buf, wifi_promiscuous_pkt_type_t type);
//callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);

#endif

