/**
 * @Author: Rafael Aranda M.Sc Student FH Aachen- CIDESI <rafa>
 * @Date:   2021-27-04
 * @Email:  rafael.aranda@alumni.fh-aachen.de
 * @Filename: WeatherStat_BlynkApp.h
 * @Last modified by:   rafa
 * @Last modified time: 2021-27-04
 * @License: CC by-sa
 */
//////////// Tutorial WeatherStat_BlynkApp_h /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this library contains two functions
//
// the function:
//                p = iaq_to_percentage(x);
//
// will reurn the value of the input times 25
//
//               p = x*25
//
// both "p" and "x" are integers
//
// the function:
//                q = get_properties_for_iaq_level(iaq_value);
//
// will reurn a structure whith the corresponding values of led_color led_intensity and iaq_description to the iaq_value of the input
//
//                q ={string = led_color,int = led_intensity,string = iaq_description}
//
// to use this funtion in main q most also be declare in main(); to do this one mos write the declaration:
//
//               iaq_level q;
//
/////////////////////// end of tutortial WeatherStat_BlynkApp_h ////////////////////////////////////////////////////////////////////////////////////
#ifndef WeatherStat_CO2sensor_h //This goes allways Luke
#define WeatherStat_CO2sensor_h // this also
#include <Arduino.h>      // that one is optional in the .h i think
// difinitions for the ESP32 regarding the C02_sensor for the library
#define MHZ19_PWM_PIN 34                // PWM Pin des MHZ19
//#define MHZ19_MAX_PPM 2000              // Messbereich des MHZ19
#define MHZ19_MAX_PPM 5000
//////////// end of definitions
//////////////////////////////////  declaration of variables ////////////////////////////////////////////////////////////
//unsigned long timeHigh;
//unsigned long timeLow;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////  declaration of functions /////////////////////////////////////////////////////////////
float get_CO2_measure();
#endif   // that goes allways it closes the library

/////////////////////////////////  end of the library ////////////////////////////////////////////////////////////////
