
#include "timeUtil.h"

const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;


/// <summary>
/// Updates system time
/// </summary>
/// <param name="dateTimeValue">Expected format: yyyy-MM-dd-HH-mm-ss</param>
void setSystemTime(String dateTimeValue) {

    //Split date/time into array

    //Get year
    int year = getIntegerValueFromDelimitedString(dateTimeValue, '-', 0);

    //Get month
    int month = getIntegerValueFromDelimitedString(dateTimeValue, '-', 1);

    //Get day
     int day = getIntegerValueFromDelimitedString(dateTimeValue, '-', 2);

    //Get hour
     int hour = getIntegerValueFromDelimitedString(dateTimeValue, '-', 3);

         //Get minute

         int minute = getIntegerValueFromDelimitedString(dateTimeValue, '-', 4);

         //Get second
         int second = getIntegerValueFromDelimitedString(dateTimeValue, '-', 5);

   // Serial.println("Extracted date parts from date string");

    struct tm tm;

    tm.tm_year = year - 1900;

    tm.tm_mon = month;

    tm.tm_mday = day;

    tm.tm_hour = hour;

    tm.tm_min = minute;

    tm.tm_sec = second;

        
    char buffer[100];
    sprintf(buffer, "Time Elements: Second = %d, min= %d, hour= %d, day= %d,min= %d, month=%d, year=%d\n", second ,minute, hour, day, month, year);
   // Serial.print(buffer);

    ESP32Time rtc;
        rtc.setTime(second, minute, hour, day, month, year,0);  // 17th Jan 2021 15:24:30
        
//    settimeofday(&now, NULL);
        Serial.printf("Time set to: [%u] - Micros: [%u]\n", rtc.getDateTime(), rtc.getMicros());
        
};

void  printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo , "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");  
}

void setNeworkTime(){

    const char* ssid = "EE-gmjpcs";
    const char* wifiPassword = "rigs-mow-inked";
    const char* ntpServer = "pool.ntp.org";

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
    Serial.println("WiFi disconnected.");
}

String  getCurrentNetworkTimeString(){
  String curTime = "UNKNOWN";
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  } 
  else{
    int year = timeinfo.tm_year +1900;
    int month = timeinfo.tm_mon+1;
    int day = timeinfo.tm_mday;
    int hour = timeinfo.tm_hour;
    int minute = timeinfo.tm_min;
    int second = timeinfo.tm_sec;
   

    char s [100];
    sprintf (s, "%d-%d-%d %d:%d.%d", year, month, day,hour,minute,second);

    curTime = String(s);
    
  }
  return curTime;
  }

String getCurrentSystemTimeString()
{
    String curTime = "UNKNOWN";
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain local time");
        return curTime;
    }

    int year = timeinfo.tm_year + 1900;
    int month = timeinfo.tm_mon + 1;
    int day = timeinfo.tm_mday;
    int hour = timeinfo.tm_hour;
    int minute = timeinfo.tm_min;
    int second = timeinfo.tm_sec;
    

    char s[100];
    sprintf(s, "%d-%d-%d %d:%d.%d", year, month, day, hour, minute, second);

    curTime = String(s);

    return  curTime;

}

std::string getCurrentSystemTimeStdString()
{
    std::string curTime = "UNKNOWN";
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain local time");
        return curTime;
    }

    int year = timeinfo.tm_year + 1900;
    int month = timeinfo.tm_mon + 1;
    int day = timeinfo.tm_mday;
    int hour = timeinfo.tm_hour;
    int minute = timeinfo.tm_min;
    int second = timeinfo.tm_sec;


    char s[100];
    sprintf(s, "%d-%d-%d %d:%d.%d", year, month, day, hour, minute, second);

    curTime = s;

    return  curTime;

}

  void printAccurateTime(){



    struct timeval tmnow;
    struct tm *tm;
    char buf[30], usec_buf[6];
    gettimeofday(&tmnow, NULL);
    tm = localtime(&tmnow.tv_sec);
    strftime(buf,30,"%Y:%m:%dT%H:%M:%S", tm);
    strcat(buf,".");
    int millisec = lrint(tmnow.tv_usec / 1000.0);
    sprintf(usec_buf,"%d",millisec);
    strcat(buf,usec_buf);
    Serial.println(buf);


     

  }



// void setInternalClock(){
//       struct tm timeinfo;
//   if(!getLocalTime(&timeinfo)){
//     Serial.println("Failed to obtain time");
//     return;
//   }    
//     //  setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
//     //         timeinfo.tm_mday, timeinfo.tm_mon+1,timeinfo.tm_year - 100);
// }



