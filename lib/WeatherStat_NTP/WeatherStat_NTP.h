/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2020-05-02T23:38:27+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: RoboMowerTime.h
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-14T19:51:09+02:00
 * @License: CC by-sa
 */

#ifndef WeatherStat_NTP_h //This goes allways Luke
#define WeatherStat_NTP_h // this also


#define IN_NUMBERS 1
#define IN_LETTERS 2
//include <Arduino.h>
// #define SECONDS 0
// #define MINUTES 1
// #define HOURS 2
// #define DAY 3
// #define MONTH 4
// #define YEAR 5
//const uint8_t DaysPerMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
// January - 31 days
// February - 28 days in a common year and 29 days in leap years
// March - 31 days
// April - 30 days
// May - 31 days
// June - 30 days
// July - 31 days
// August - 31 days
// September - 30 days
// October - 31 days
// November - 30 days
// December - 31 days

// definition of functions for the .cpp

String getZeit(void);
String getDatum(uint8_t ShowMode);
int getMonat(void);

#endif   // that goes allways it closes the library
