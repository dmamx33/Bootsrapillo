/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2020-05-02T23:44:59+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: RoboMowerTime.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-14T21:13:35+02:00
 * @License: CC by-sa
 */

 #include <Arduino.h>
 #include "WeatherStat_NTP.h"
 #include "time.h"

String getZeit(){
        String out="";
        tm struct_time_ntp;
        if(!getLocalTime(&struct_time_ntp))
        {
                out = "XX:XX:XX";
        }
        else
        {
                if(struct_time_ntp.tm_hour < 10) out="0";
                out += String(struct_time_ntp.tm_hour)+":";
                if(struct_time_ntp.tm_min<10) out+="0";
                out += String(struct_time_ntp.tm_min);//+":";
                // if(struct_time_ntp.tm_sec<10) out+="0";
                // out += String(struct_time_ntp.tm_sec);
        }
        return out;
}


String getDatum(uint8_t ShowMode){
        String out="";
        const char diasSemana[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
        const char mes_anyo[12][12]={"Januar","Februar","März","April ","Mai","Juni ","Juli ","August","September","October","November","Dezember"};
        tm struct_time_ntp;
        if(!getLocalTime(&struct_time_ntp))
        {
                out = "XX.XX.XX";
        }
        else
        {
                switch (ShowMode) {
                case IN_NUMBERS:
                        //out = String(struct_time_ntp.tm_wday)+ " ";
                        if(struct_time_ntp.tm_mday < 10) out="0";
                        out += String(struct_time_ntp.tm_mday)+".";
                        if((struct_time_ntp.tm_mon++)<10) out+="0";
                        out += String(struct_time_ntp.tm_mon++);
                        //out += String(struct_time_ntp.tm_year+1900);
                        break;
                case IN_LETTERS:
                        out = diasSemana[struct_time_ntp.tm_wday];
                        out += " ";
                        if(struct_time_ntp.tm_mday < 10) out="0";
                        out += String(struct_time_ntp.tm_mday)+" ";
                        out += mes_anyo[struct_time_ntp.tm_mon];
                        //out += String(struct_time_ntp.tm_year+1900);
                        break;
                }
                out += " "+String(struct_time_ntp.tm_year+1900);
        }

        return out;
}

int getMonat(void){
        int mes;
        tm struct_time_ntp;
        if(!getLocalTime(&struct_time_ntp))
        {
                mes = 7;
        }
        else{
                mes =  ++struct_time_ntp.tm_mon;
        }

        return mes;
}
