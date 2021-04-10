// 
// 
// 

#include "dc_networkUtil.h"


bool macAddressMatched(uint8_t* mac1, uint8_t* mac2) {  // compare mac address
    for (int i = 0; i < 6; i++) {

        if (mac1[i] != mac2[i]) {
            return false;
        }
    }
    return true;
}