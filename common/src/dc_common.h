/*
 Name:		common.h
 Created:	3/11/2021 5:30:32 PM
 Author:	matt_
 Editor:	http://www.visualmicro.com
*/

#ifndef _dc_common_h
#define _dc_common_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifndef __ESP_WIFI_H__
    #include "esp_wifi.h"
#endif



#include "esp_system.h"

#include "esp_event_loop.h"
#include <esp_now.h>

struct rssiScanResult {
    char distance[15];
    char timeStamp[20];
    int rssi;
    char macAddress[20];
    int messageCount;
};

/**
 * Ref: https://github.com/lpodkalicki/blog/blob/master/esp32/016_wifi_sniffer/main/main.c
 */
typedef struct {
    unsigned frame_ctrl : 16;
    unsigned duration_id : 16;
    uint8_t addr1[6]; /* receiver address */
    uint8_t addr2[6]; /* sender address */
    uint8_t addr3[6]; /* filtering address */
    unsigned sequence_ctrl : 16;
    uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;



#endif

