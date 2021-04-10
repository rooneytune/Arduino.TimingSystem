// 
// 
// 

#include "displayRefreshHandler.h"

displayRefreshHandler::displayRefreshHandler(int refreshIntervalMs) {
    RefreshIntervalMs = refreshIntervalMs;
};

bool displayRefreshHandler::refreshRequired() {
    long timeSinceLastUpdate = millis() - LastRefreshTimestampMs;
    //Serial.print("Time since last refresh: ");
   // Serial.println(timeSinceLastUpdate);
    //Serial.print("Last refresh timestamp: ");
   // Serial.println(LastRefreshTimestampMs);

    //Serial.print("Refresh Interval: ");
    //Serial.println(RefreshIntervalMs);

    if (timeSinceLastUpdate > RefreshIntervalMs) {
       // Serial.println("Refresh required !!");
        return true;
    }
    else
        return false;

};

void displayRefreshHandler::updateRefreshTimestamp() {
    LastRefreshTimestampMs = millis();
}
void displayRefreshHandler::setRefreshInterval(int refreshIntervalMs)
{
    RefreshIntervalMs = refreshIntervalMs;
}
;


