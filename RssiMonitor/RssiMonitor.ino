

#ifndef displayRefreshHandler_h

#include <Arduino_JSON.h>
#include "dc_networkUtil.h"
#include "dc_StringUtil.h"
#include "displayRefreshHandler.h"
#endif 

#ifndef timeutil_h
    #include <timeUtil.h>
#endif

#ifndef _dc_common_h
    #include "dc_common.h"
#endif

#ifndef _ACCESSPOINTREQUESTHANDLER_h
    #include "accessPointRequestHandler.h"
#endif
//
#ifndef _dc_EventManager_h
    #include "dc_EventManager.h"
#endif //

#ifndef ESP_H
    #include "Esp.h"
#endif

EventManager gMyEventManager;

const int led_pin = 2;

void displayMemoryUsage() {
    uint32_t heapSize = ESP.getHeapSize(); //total heap size
    uint32_t freeHeap = ESP.getFreeHeap(); //available heap
    int freeRamKb = freeRam();
    
    //uint32_t minFreeHeap = ESP.getMinFreeHeap(); //lowest level of free heap since boot
    //uint32_t maxFreeHeap = ESP.getMaxAllocHeap(); //largest block of heap that can be allocated at once

    Serial.printf("Heap Size: %u - Free heap: %u - FreeRam: %u\n",heapSize,freeHeap,freeRamKb);

    
}

int freeRam()
{
    /*extern int __heap_start, * __brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);*/
    return 0;
}


//Fires when AccessPoint recieves a command to alter RecordMode state
//This toggles state of 'Record' LED 
void apToggleRecordMode(int eventCode, int pinState)
{
    int eventsInQueue = gMyEventManager.getNumEventsInQueue();
    String curTime = getCurrentSystemTimeString();
    Serial.printf("Processing Event: apToggleRecordModeEvent. \n PinState: %u - Time: %s\n", pinState,curTime.c_str());
    displayMemoryUsage();
    // RSSI settings updated! 
    if (pinState == 0) {
        digitalWrite(led_pin, LOW);
    }
    else {
        digitalWrite(led_pin, HIGH);
    }
    
}

void apClearRssiScanResultSet(int eventCode, int eventData)
{
    
    String curTime = getCurrentSystemTimeString();
    Serial.printf("Processing Event: apClearRssiScanResultSet - Time: %s\n",curTime.c_str());
    displayMemoryUsage();
    //Clear the result array
    clearRssiScanResultSet();
    
}
//
void apSendRssiSettingsToClient(int eventCode, int eventData) {

    String curTime = getCurrentSystemTimeString();
    Serial.printf("Processing Event: apSendRssiSettingsToClient - Time: %s\n", curTime.c_str());    
    displayMemoryUsage();
    sendRssiSettingsToClient(eventData);
}  


void setup() {

    Serial.begin(115200);
    displayMemoryUsage();

    // Init LED and turn off
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);

    //Ensure Spiffs loaded OK
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //Init AP, web & socket server
    initialiseAccessPoint();
    initialiseWebServer();
    initialiseWebSocketServer();

    //Pass address of event manager to accesspoint
    eventManager_set(&gMyEventManager);

    //Add listeners for AP events
    gMyEventManager.addListener(EventManager::kEventWebServer_toggleRecordMode, apToggleRecordMode);
    gMyEventManager.addListener(EventManager::kEventWebServer_clearRssiScanResultSet, apClearRssiScanResultSet);
    gMyEventManager.addListener(EventManager::kEventWebServer_sendRssiSettingsToClient, apSendRssiSettingsToClient);
    

    displayMemoryUsage();
    Serial.println("Setup complete");

}



// the loop function runs over and over again forever
void loop() {
    // Look for and handle WebSocket data
    webSocketLoop();

    //Checks for pending events 
    gMyEventManager.processEvent();

}
