/**
 * @Author: Rafael Aranda M.Sc Student FH Aachen- CIDESI <rafa>
 * @Date:   2021-27-04
 * @Email:  rafael.aranda@alumni.fh-aachen.de
 * @Filename: WeatherStat_BlynkApp.cpp
 * @Last modified by:   rafa
 * @Last modified time: 2021-27-04
 * @License: CC by-sa
 */

 #include <Arduino.h>                       // tipical arduino bakärei
 #include "WeatherStat_CO2sensor.h"                    // name from the library created also known als Brötchen des Tages
 #include "time.h"                          // time library for some reason
 unsigned long timeHigh;
 unsigned long timeLow;
 unsigned long threshold_mesured_estimated = 1000; //value acceptable between estimation and measurement to validate measure
 unsigned long difference_mesured_estimated =0;

///////////////////////////////        funtion to conver the measure of acurracy from 0 to 4 to a percentage from 0 to 100/////////////////////////////////////////////////
float get_CO2_measure(){
  timeHigh = pulseIn(MHZ19_PWM_PIN, HIGH, 2200000);// Alle Zeiten in Mikrosekunden
  timeLow = pulseIn(MHZ19_PWM_PIN, LOW, 2200000);
  String temp ="   Time High: " + String(timeHigh) + "  Low: " + String(timeLow);
  //Serial.println(temp);
  unsigned long periodDuration = timeHigh + timeLow;


  const unsigned long highLeadTime = 2000;                      // Der Sensor sendet zunächst 2000 µsec HIGH als Signal, dass ein neuer Wert gesendet wird.
  const unsigned long lowEndTime = 2000;                        // Der Sensor sendet am Ende des Wertes noch 2000 µsec LOW als Endmarker.

  // Mit dem PWM Signal überträgt der Sensor den Wert als Bruchteil des Messbereichs ...
  float pulseHighRatio = (timeHigh - highLeadTime)/(float)(periodDuration - highLeadTime - lowEndTime);
  // .. erst durch die Multiplaktion mit dem Messbereich erhält man den absoluten Messwert.
  float CO2_measured = MHZ19_MAX_PPM * pulseHighRatio;

  return CO2_measured;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
