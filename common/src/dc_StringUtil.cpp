// 
// 
// 

#include "dc_StringUtil.h"

String getStringValueFromDelimitedString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int  getIntegerValueFromDelimitedString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
    int retValue = 0;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    if (found > index) {
        int valueLength = strIndex[1] - strIndex[0];
        char* charValue;

        String valueAtIndex = data.substring(strIndex[0], strIndex[1]);
            
        retValue = valueAtIndex.toInt();

        return  retValue;

    }

}

std::string getStdStringValueFromDelimitedString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
    std::string retValue = "";

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    if (found > index) {
        int valueLength =  strIndex[1]- strIndex[0];
        char* charValue;
        
        String valueAtIndex = data.substring(strIndex[0], strIndex[1]);

        retValue = valueAtIndex.c_str();
        return  retValue;
    }

}
