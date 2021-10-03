/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2020-05-02T23:38:27+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: WeatherStat_Messages.h
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-19T13:54:07+02:00
 * @License: CC by-sa
 */
#include <Arduino.h>
 #ifndef WeatherStat_Memory_h
 #define WeatherStat_Memory_h

//#define START_DATA_PRESSURE (141)
#define START_DATA_WIFI (162)

char* RetrieveSSID(uint8_t anfang );
char* RetrievePASSW(uint8_t anfang );
void SaveSSID(uint8_t anfang,char* ssid);
void SavePASSW(uint8_t anfang,char* passw);
void CleanMemoryWifi(uint8_t anfang);

 #endif   // that goes allways it closes the library
