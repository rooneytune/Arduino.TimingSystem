// 
// 
// 

#include "accessPointRequestHandler.h"

// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);


EventManager *mEventManager;

const int rssiScanMaxResultCount = 100;
rssiScanResult rssiScanResultSet[rssiScanMaxResultCount];
int rssiScanResultCount;


char msg_buf[100];
int rssiSettings_ledState = 0;
char rssiSettings_distance[15] = "0";


const char* msg_toggle_led = "toggleLED";
const char* msg_get_led = "getLEDState";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;


const char* ssid = "RoonsAP";
const char* apPassword = "12345";


char* wifi_sniffer_packet_type_two_str(wifi_promiscuous_pkt_type_t type) {
    switch (type) {
    case WIFI_PKT_MGMT: return "MGMT";
    case WIFI_PKT_DATA: return "DATA";
    default:
    case WIFI_PKT_MISC: return "MISC";
    }
}
void clearRssiScanResultSet() {
    //for (int i = 0; i < rssiScanResultCount; i++) {
    //    rssiScanResultSet[i] = rssiScanResult{ 0, };
    //}
    std::fill_n(rssiScanResultSet, rssiScanMaxResultCount, rssiScanResult{});
    rssiScanResultCount = 0;
};
std::string rssiSettings_getScanResults() {

    // std::vector<rssiScanResult> resultSet = generateTestRssiResultSet();
    
    Serial.printf("Result Count: U%\n", rssiScanResultCount);
    JSONVar myJsonArray;

    for (int i = 0; i < rssiScanResultCount; i++) {
        rssiScanResult item = rssiScanResultSet[i];
        JSONVar myObject;
        myObject["index"] = i + 1;
        myObject["rssi"] = item.rssi;
        myObject["distance"] = item.distance;
        myObject["timeStamp"] = item.timeStamp;
        myObject["messageCountPerSecond"] = item.messageCount;
        
        myJsonArray[i] = myObject;

    }

    Serial.print("myArray.length() = ");
    Serial.println(myJsonArray.length());

    std::string jsonString;
    if (myJsonArray.length() > 0) {
        // JSON.stringify(myVar) can be used to convert the json var to a String
        jsonString = JSON.stringify(myJsonArray).c_str();

        Serial.print("JSON resultSet = ");
        Serial.printf("s%\n", jsonString);
    }
    return jsonString;
};
void addRssiScanResult(rssiScanResult scan)
{
    if (rssiScanResultCount <= rssiScanMaxResultCount - 1) {
        //Populate array with results up tp max index value
        rssiScanResultSet[rssiScanResultCount] = scan;
        rssiScanResultCount++;
    }

//    rssiScanResultSet.push_back(scan);
};


void eventManager_set(EventManager *manager) {
    mEventManager = manager;
};

 //std::vector<rssiScanResult> generateTestRssiResultSet() {

 //    std::vector<rssiScanResult> resultSet;

 //    /*rssiScanResult item1 ;
 //    item1.distance = "10";
 //    item1.rssi = -50;
 //    item1.timeStamp = "2021-01-01-10-01-01";
 //    resultSet.push_back(item1);

 //    rssiScanResult item2;
 //    item2.distance = "5";
 //    item2.rssi = -40;
 //    item2.timeStamp =  "2021-01-01-10-01-02" ;
 //    resultSet.push_back(item2);

 //    rssiScanResult item3;
 //    item3.distance = "2";
 //    item3.rssi = -30;
 //    item3.timeStamp = "2021-01-01-10-01-03";
 //    resultSet.push_back(item3);*/

 //    return resultSet;
 //}



void webSocketLoop()
{
    webSocket.loop();
}
void initialiseWebServer() {

    // On HTTP request for root, provide index.html file
    server.on("/", HTTP_GET, onIndexRequest);

    // On HTTP request for rssiResults, provide rssiResults.html file
    server.on("/rssiResults.html", HTTP_GET, onRssiResultsRequest);

    // On HTTP request for style sheet, provide style.css
    server.on("/style.css", HTTP_GET, onCSSRequest);

    // Handle requests for pages that do not exist
    server.onNotFound(onPageNotFound);

    server.begin();



    rssiSettings_ledState = 0;
};
void initialiseWebSocketServer() {
    // Start WebSocket server and assign callback
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);
}
void initialiseAccessPoint()
{

    // Start access point
    WiFi.softAP(ssid, apPassword);

    // Print our IP address
    Serial.println();
    Serial.println("AP running");
    Serial.print("My IP address: ");
    Serial.println(WiFi.softAPIP());


    //Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        //  return;
    }

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("ESP-NOW event setup complete");

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

};



std::string rssiSettings_getHtmlFormState() {

    JSONVar myObject;
    //String distance =  rssiSettings_distance.c_str() ;
    

    myObject["ledState"] = rssiSettings_ledState;
    myObject["distance"] = rssiSettings_distance;
    myObject["serverTime"] = getCurrentSystemTimeString();
    //myObject["systemTime"] = TestSub

    // JSON.stringify used to convert the json var to a String
    return JSON.stringify(myObject).c_str();

    //Serial.print("JSON.stringify(myObject) = ");
    //Serial.println(jsonString);
   // return jsonString.c_str();
    
}

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t* payload, size_t length) {
    bool sendFormStateUpdate = false;
    IPAddress ip = webSocket.remoteIP(client_num);
    // Figure out the type of WebSocket event
    switch (type) {

        // Client has disconnected
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", ip);
        break;

        // New client has connected
    case WStype_CONNECTED:
    {
        Serial.printf("Connection from [%u]\n", ip);
       
    }
    break;

    // Handle text messages from client
    case WStype_TEXT:
    {
        // Print out raw message
        Serial.printf("Received text: [%s] - From: [%u]\n",  payload,ip);

        String payloadString = (char*)payload;

        

        if (strcmp((char*)payload, "rssiSettings_toggleLED") == 0) {
            // Toggle LED state
            Serial.printf("Current LED State: %u\n", rssiSettings_ledState);

            if (rssiSettings_ledState==1) {
                rssiSettings_ledState = 0;
            }
            else { 
                rssiSettings_ledState = 1; 
            }

            Serial.printf("Toggling LED to %u\n", rssiSettings_ledState);
            //Throw event to update LED state
            mEventManager->queueEvent(EventManager::kEventWebServer_toggleRecordMode, rssiSettings_ledState);
            sendFormStateUpdate = true;
        }
        else if (strcmp((char*)payload, "rssiSettings_ClearResultLog") == 0) {
            // Clear result log
            Serial.println("Received request to clear log");
            //Add event to clear queue
            mEventManager->queueEvent(EventManager::kEventWebServer_clearRssiScanResultSet, 0);
            sendFormStateUpdate = true;
        }
        //
        else if (payloadString.startsWith("rssiSettings_distanceUpdated=")) {
            // Get distance value
            int valueIndex = payloadString.indexOf('=');  //finds location of =
            int length = payloadString.length();  //Get string length
            int valueLength = length - (valueIndex + 1);
            String ctlValue = payloadString.substring(valueIndex + 1, length);

            Serial.print("Updated value: ");
            Serial.println(ctlValue);

            //Update global var
            strcpy(rssiSettings_distance ,ctlValue.c_str());
            sendFormStateUpdate = true;
        }
        else if (payloadString.startsWith("rssiSettings_setCurrentDateTime=")) {
            // Use time value from sender
            int valueIndex = payloadString.indexOf('=');  //finds location of =
            int length = payloadString.length();  //Get string length
            int valueLength = length - (valueIndex + 1);
            String ctlValue = payloadString.substring(valueIndex + 1, length);

            //Update system time
            setSystemTime(ctlValue);
            sendFormStateUpdate = true;
        }
        else if (strcmp((char*)payload, "rssiSettings_getFormState") == 0) {
            sendFormStateUpdate = true;
        }
        else if (strcmp((char*)payload, "rssiResults_getScanResultData") == 0) {
            // Return state of form using Json string 
            std::string scanData = rssiSettings_getScanResults();
            int dataLength = scanData.length()+1;
            //scanData.toCharArray(msg_buf, dataLength);
            //Serial.printf("Sending to [%u]: %s\n", ip, msg_buf);
            //webSocket.sendTXT(client_num, msg_buf);
            webSocket.sendTXT(client_num, scanData.c_str());
        }
        else {
            // Message not recognized
            Serial.printf("[%s] Message not recognized", (char*)payload);
        }
    }
    break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
        break;
    }

    if (sendFormStateUpdate) {        
        //Add event to sent updated rssi settings to Client
        mEventManager->queueEvent(EventManager::kEventWebServer_sendRssiSettingsToClient, client_num);
    }
}
void sendRssiSettingsToClient(const uint8_t& client_num)
{
    // Return state of form using Json string 
    std::string htmlState = rssiSettings_getHtmlFormState();
    Serial.printf("HTML State: %s/n",htmlState);
    webSocket.sendTXT(client_num, htmlState.c_str());
    /*htmlState.toCharArray(msg_buf, 100);
    Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
    webSocket.sendTXT(client_num, msg_buf);*/
}
;

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest* request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
        "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/index.html", "text/html");
};

// Callback: send rssiResults 
void onRssiResultsRequest(AsyncWebServerRequest* request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
        "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/rssiResults.html", "text/html");
};

void on(AsyncWebServerRequest* request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
        "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/index.html", "text/html");
};

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest* request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
        "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/style.css", "text/css");
};

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest* request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
        "] HTTP GET request of " + request->url());
    request->send(404, "text/plain", "Not found");
};

//Create a struct_message called myData
String myData;

displayRefreshHandler mPacketsPerSecondHandler = displayRefreshHandler(1000);
uint8_t mWatchPacketCount = 0;


//callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Message: ");
    Serial.println(myData);


    char macStr[18];
    Serial.print("Packet to: ");
    // Copies the sender mac address to a string
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println(macStr);

};

/// <summary>
/// Creates a rssi scanResult from values provided
/// </summary>
/// <param name="distance"></param>
/// <param name="rssi"></param>
/// <param name="timeStamp"></param>
/// <param name="macAddress"></param>
/// <param name="messageCount"></param>
/// <returns></returns>
rssiScanResult generateRssiLog(char distance[15], int rssi, char timeStamp[20], char macAddress[20], int messageCount) {
    rssiScanResult item1;
    strcpy(item1.distance, distance);
    strcpy(item1.timeStamp, timeStamp);
    strcpy(item1.macAddress, macAddress);
    item1.rssi = rssi;
    item1.messageCount = messageCount;

    return item1;
};
void promiscuous_rx_cb(void* buf, wifi_promiscuous_pkt_type_t type) {

    // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
    //if (type != WIFI_PKT_MGMT)
    //    return;

    //Check if record mode is on
    if (rssiSettings_ledState == 1) {
        if (type != WIFI_PKT_MGMT)
            return;

        const wifi_promiscuous_pkt_t* ppkt = (wifi_promiscuous_pkt_t*)buf;
        const wifi_ieee80211_packet_t* ipkt = (wifi_ieee80211_packet_t*)ppkt->payload;
        const wifi_ieee80211_mac_hdr_t* hdr = &ipkt->hdr;


        uint8_t watchMacAddress[] = { 0xC4, 0x4F, 0x33, 0x7F, 0xD8, 0x89 };
        uint8_t inboundAddress[] = { hdr->addr2[0], hdr->addr2[1], hdr->addr2[2], hdr->addr2[3], hdr->addr2[4], hdr->addr2[5] };

        bool macMatch = macAddressMatched(inboundAddress, watchMacAddress);


        if (macMatch) {
            //Increment packet counter
            mWatchPacketCount++;

            //Get current time
            std::string curTime = getCurrentSystemTimeStdString();
            char theTime[20];
            strcpy(theTime, curTime.c_str());

            if (mPacketsPerSecondHandler.refreshRequired()) {
                //1 second has elapsed, assemble log data

                //Create macAddress string
                char watchMacStr[18];
                snprintf(watchMacStr, sizeof(watchMacStr), "%02x:%02x:%02x:%02x:%02x:%02x",
                    watchMacAddress[0], watchMacAddress[1], watchMacAddress[2], watchMacAddress[3], watchMacAddress[4], watchMacAddress[5]);
                //Create scan log instance    
                rssiScanResult log = generateRssiLog(rssiSettings_distance, ppkt->rx_ctrl.rssi, theTime, watchMacStr, mWatchPacketCount);
                //Add log to array
                addRssiScanResult(log);
                mPacketsPerSecondHandler.updateRefreshTimestamp();

                //reset packet counter
                mWatchPacketCount = 0;
            }



            //Packet address matches that of known TX device
            //printf("PACKET TYPE=%s, CHAN=%02d, RSSI=%02d,"
            //    " ADDR2=%02x:%02x:%02x:%02x:%02x:%02x\n",
            //    wifi_sniffer_packet_type_two_str(type),
            //    ppkt->rx_ctrl.channel,
            //    ppkt->rx_ctrl.rssi,
            //    /* ADDR2 */
            //    hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
            //    hdr->addr2[3], hdr->addr2[4], hdr->addr2[5]
            //);


        };
    }


};

