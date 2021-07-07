/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2021-05-14T18:13:45+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: WeatherStat_Messages.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-17T18:36:02+02:00
 * @License: CC by-sa
 */


 #include <Arduino.h>
 #include <EEPROM.h>
//#include "WeatherStat_Messages.h"


// String ohno;
//
// ohno="⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⡏⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⠀⠀⠀⠈⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠙⠿⠿⠿⠻⠿⠿⠟⠿⠛⠉⠀⠀⠀⠀⠀⣸⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣴⣿⣿⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⢰⣹⡆⠀⠀⠀⠀⠀⠀⣭⣷⠀⠀⠀⠸⣿⣿⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠈⠉⠀⠀⠤⠄⠀⠀⠀⠉⠁⠀⠀⠀⠀⢿⣿⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣿⢾⣿⣷⠀⠀⠀⠀⡠⠤⢄⠀⠀⠀⠠⣿⣿⣷⠀⢸⣿⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣿⡀⠉⠀⠀⠀⠀⠀⢄⠀⢀⠀⠀⠀⠀⠉⠉⠁⠀⠀⣿⣿⣿\n";
// ohno+="⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿\n";
// destination = (char*) calloc( source.length()+1,sizeof(char) );
//
// for(int i=0; i<source.length(); i++)
//         destination[i] = source.charAt(i);
//         destination[source.length()]='\0';
//
char* RetrieveSSID(uint8_t anfang ){
        char* ssid;
        //Serial.begin(115200);
        size_t laenge = size_t(EEPROM.read(anfang));
        if(laenge)
        {
                ssid = (char*) calloc( laenge+1,sizeof(char) );
                for(int i = 0; i<laenge; i++)
                        ssid[i] = char(EEPROM.read(anfang + i + 1));
                ssid[laenge]='\0';
        }
        else ssid = NULL;

        return ssid;
}

char* RetrievePASSW(uint8_t anfang ){
        char* passw;
        size_t laenge_ssid = size_t(EEPROM.read(anfang));
        if(laenge_ssid)
        {
                size_t laenge_passw = size_t(EEPROM.read(anfang+laenge_ssid+1));
                passw = (char*) calloc( laenge_passw+1,sizeof(char) );
                for(int i = 0; i<laenge_passw; i++)
                        passw[i] = char(EEPROM.read(anfang+laenge_ssid+2+i));
                passw[laenge_passw]='\0';
        }
        else passw = NULL;
        return passw;
}

void SaveSSID(uint8_t anfang, char* ssid){
        size_t laenge =  strlen(ssid);
        EEPROM.write(anfang, laenge);
        EEPROM.writeString(anfang+1, ssid);
        EEPROM.commit();
}

void SavePASSW(uint8_t anfang, char* passw){
        size_t laenge_ssid = size_t(EEPROM.read(anfang));
        if(!laenge_ssid) return;
        size_t laenge_passw =  strlen(passw);
        EEPROM.write(anfang+laenge_ssid+1, laenge_passw);
        for(int i = 0; i<laenge_passw; i++)
                EEPROM.write(anfang+laenge_ssid+2+i, uint8_t(passw[i]));
        EEPROM.commit();
}

void CleanMemoryWifi(uint8_t anfang){
        size_t laenge_ssid = size_t(EEPROM.read(anfang));
        size_t laenge_passw = size_t(EEPROM.read(anfang+laenge_ssid+1));
        size_t stopc = laenge_ssid + laenge_passw +2;
        for(size_t i=0; i<stopc; i++)
                EEPROM.write(anfang + i, 0);
        EEPROM.commit();
}
