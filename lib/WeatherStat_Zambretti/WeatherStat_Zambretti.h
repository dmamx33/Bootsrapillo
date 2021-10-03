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
 #ifndef WeatherStat_Zambretti_h
 #define WeatherStat_Zambretti_h


const char *weather_forecast[] = {
        "Settled Fine", //1
        "Fine Weather",  //2
        "Becoming Fine", //3
        "Fine Becoming Less Settled", //4
        "Fine, Possibly showers", //5
        "Fairly Fine, Improving", //6
        "Fairly Fine, Possibly showers, early", //7
        "Fairly Fine Showery Later", //8
        "Showery Early, Improving", //9
        "Changeable Mending", //10
        "Fairly Fine , Showers likely", //11
        "Rather Unsettled Clearing Laters", //12
        "Unsettled, Probably Improving", //13
        "Showery Bright Intervals", //14
        "Showery Becoming more unsettled", //15
        "Changeable some rain", //16
        "Unsettled, short fine Intervals", //17
        "Unsettled, Rain later", //18
        "Unsettled, rain at times", //19
        "Very Unsettled, Finer at times", //20
        "Rain at times, worse later", //21
        "Rain at times, becoming very unsettled", //22
        "Rain at Frequent Intervals", //23
        "Very Unsettled, Rain", //24
        "Stormy, possibly improving", //25
        "Stormy, much rain", //26
        "Forecast not ready" //27
};


int calc_zambretti(int curr_pressure, int prev_pressure, int mon);
bool ForecastReady(void);
void ResetForecastData(void);
void SaveMonth(int month);
uint8_t SavedMonth(void);
void PushPressure(int current_pressure);
void GetSavedPressures(int *arreglo);
int GetNumbePress(void);

// #define START_DATA_PRESSURE (141)
// #define START_DATA_WIFI (162)
//
// char* RetrieveSSID(uint8_t anfang );
// char* RetrievePASSW(uint8_t anfang );
// void SaveSSID(uint8_t anfang,char* ssid);
// void SavePASSW(uint8_t anfang,char* passw);
// void CleanMemoryWifi(uint8_t anfang);

 #endif   // that goes allways it closes the library
