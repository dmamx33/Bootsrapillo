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
//#include "WeatherStat_Zambretti.h"

 #define P01 1
 #define P02 2
 #define P03 3
 #define P08 4
 #define P09 5
 #define P10 6
 #define NOT_READY 26
 #define MONTH_SAVED (141)
 #define COUNTER_SAMPLES_PRESS (142)
 #define START_DATA_PRESSURE (143)
 #define TOTAL_SAMPLES (10)
 #define PRESS_BIAS (845)

//ZAMBRETTI NUMBER FUNCTION
int calc_zambretti(int curr_pressure, int prev_pressure, int mon){
        if (curr_pressure<prev_pressure) {
                //FALLING
                if (mon>=4 and mon<=9) //April to September
                //summer
                {
                        if (curr_pressure>=1030)
                                return 2; //Fine Weather
                        else if(curr_pressure>=1020 and curr_pressure<1030)
                                return 8; //
                        else if(curr_pressure>=1010 and curr_pressure<1020)
                                return 18;
                        else if(curr_pressure>=1000 and curr_pressure<1010)
                                return 21;
                        else if(curr_pressure>=990 and curr_pressure<1000)
                                return 24;
                        else if(curr_pressure>=980 and curr_pressure<990)
                                return 24;
                        else if(curr_pressure>=970 and curr_pressure<980)
                                return 26;
                        else if(curr_pressure<970)
                                return 26;
                }
                else{
                        //winter
                        if (curr_pressure>=1030)
                                return 2;
                        else if(curr_pressure>=1020 and curr_pressure<1030)
                                return 8;
                        else if(curr_pressure>=1010 and curr_pressure<1020)
                                return 15;
                        else if(curr_pressure>=1000 and curr_pressure<1010)
                                return 21;
                        else if(curr_pressure>=990 and curr_pressure<1000)
                                return 22;
                        else if(curr_pressure>=980 and curr_pressure<990)
                                return 24;
                        else if(curr_pressure>=970 and curr_pressure<980)
                                return 26;
                        else if(curr_pressure<970)
                                return 26;
                }
        }
        else if (curr_pressure>prev_pressure) {
                //RAISING
                if (mon>=4 and mon<=9) {
                        //summer
                        if (curr_pressure>=1030)
                                return 1;
                        else if(curr_pressure>=1020 and curr_pressure<1030)
                                return 2;
                        else if(curr_pressure>=1010 and curr_pressure<1020)
                                return 3;
                        else if(curr_pressure>=1000 and curr_pressure<1010)
                                return 7;
                        else if(curr_pressure>=990 and curr_pressure<1000)
                                return 9;
                        else if(curr_pressure>=980 and curr_pressure<990)
                                return 12;
                        else if(curr_pressure>=970 and curr_pressure<980)
                                return 17;
                        else if(curr_pressure<970)
                                return 17;
                }
                else
                //winter
                {
                        if (curr_pressure>=1030)
                                return 1;
                        else if(curr_pressure>=1020 and curr_pressure<1030)
                                return 2;
                        else if(curr_pressure>=1010 and curr_pressure<1020)
                                return 6;
                        else if(curr_pressure>=1000 and curr_pressure<1010)
                                return 7;
                        else if(curr_pressure>=990 and curr_pressure<1000)
                                return 10;
                        else if(curr_pressure>=980 and curr_pressure<990)
                                return 13;
                        else if(curr_pressure>=970 and curr_pressure<980)
                                return 17;
                        else if(curr_pressure<970)
                                return 17;
                }
        }
        else{
                if (curr_pressure>=1030)
                        return 1;
                else if(curr_pressure>=1020 and curr_pressure<1030)
                        return 2;
                else if(curr_pressure>=1010 and curr_pressure<1020)
                        return 11;
                else if(curr_pressure>=1000 and curr_pressure<1010)
                        return 14;
                else if(curr_pressure>=990 and curr_pressure<1000)
                        return 19;
                else if(curr_pressure>=980 and curr_pressure<990)
                        return 23;
                else if(curr_pressure>=970 and curr_pressure<980)
                        return 24;
                else if(curr_pressure<970)
                        return 26;
        }
}
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
bool ForecastReady(void){
        bool res = false;

        if(EEPROM.read(COUNTER_SAMPLES_PRESS)>=10) res = true;
        else res = false;

        return res;
}

void ResetForecastData(void){
        EEPROM.write(COUNTER_SAMPLES_PRESS, 0);
        EEPROM.commit();
        for (uint8_t i = 0; i < 10; i++) {
                EEPROM.write(START_DATA_PRESSURE + i, 0);
                EEPROM.commit();
        }
}

void SaveMonth(int month){
        EEPROM.write(MONTH_SAVED,uint8_t(month));
        EEPROM.commit();
}

uint8_t SavedMonth(void){
        uint8_t mes=EEPROM.read(MONTH_SAVED);
        return mes;
}

void PushPressure(int current_pressure){

        uint8_t pres_actual = uint8_t(current_pressure - 845);//restale 845 para qe quepa en 8 bits
        uint8_t current_index = EEPROM.read(COUNTER_SAMPLES_PRESS);//cuantos datos hay guardados, indice
        uint8_t aux=0;
        if(current_index!=0)//si es la primera presiona guardar escapa
        {
                for (int i =int(current_index); i>=0; i--) { // este ciclo recorre una posicion cada presion
                        aux = EEPROM.read( START_DATA_PRESSURE + i );
                        if( i<9 ) { // la posicion 10 no se debe guardar, se descarta
                                EEPROM.write(START_DATA_PRESSURE + i + 1, aux);
                                EEPROM.commit();
                        }
                        aux=0;
                }
        }
        EEPROM.write(START_DATA_PRESSURE, pres_actual);//pon la presion actual en la primera posicion
        EEPROM.commit();

        if (current_index>=10){
                if(current_index>10)  {
                        EEPROM.write(COUNTER_SAMPLES_PRESS, 10);//procura siempre sea 9 el numero max de datos
                        EEPROM.commit();
                }
        }
        else{
                EEPROM.write(COUNTER_SAMPLES_PRESS, current_index+1);//actualiza nuevo indice
                EEPROM.commit();
        }


}

void GetSavedPressures(int *arreglo){

        for (uint8_t i = 0; i < 10; i++) {
                arreglo[i]=int(EEPROM.read(START_DATA_PRESSURE+i))+845;
        }

        //return arreglo;
}

int GetNumbePress(void){
        return int(EEPROM.read(COUNTER_SAMPLES_PRESS));
}
// char* RetrieveSSID(uint8_t anfang ){
//         char* ssid;
//         //Serial.begin(115200);
//         size_t laenge = size_t(EEPROM.read(anfang));
//         if(laenge)
//         {
//                 ssid = (char*) calloc( laenge+1,sizeof(char) );
//                 for(int i = 0; i<laenge; i++)
//                         ssid[i] = char(EEPROM.read(anfang + i + 1));
//                 ssid[laenge]='\0';
//         }
//         else ssid = NULL;
//
//         return ssid;
// }
//
// char* RetrievePASSW(uint8_t anfang ){
//         char* passw;
//         size_t laenge_ssid = size_t(EEPROM.read(anfang));
//         if(laenge_ssid)
//         {
//                 size_t laenge_passw = size_t(EEPROM.read(anfang+laenge_ssid+1));
//                 passw = (char*) calloc( laenge_passw+1,sizeof(char) );
//                 for(int i = 0; i<laenge_passw; i++)
//                         passw[i] = char(EEPROM.read(anfang+laenge_ssid+2+i));
//                 passw[laenge_passw]='\0';
//         }
//         else passw = NULL;
//         return passw;
// }
//
// void SaveSSID(uint8_t anfang, char* ssid){
//         size_t laenge =  strlen(ssid);
//         EEPROM.write(anfang, laenge);
//         EEPROM.writeString(anfang+1, ssid);
//         EEPROM.commit();
// }
//
// void SavePASSW(uint8_t anfang, char* passw){
//         size_t laenge_ssid = size_t(EEPROM.read(anfang));
//         if(!laenge_ssid) return;
//         size_t laenge_passw =  strlen(passw);
//         EEPROM.write(anfang+laenge_ssid+1, laenge_passw);
//         for(int i = 0; i<laenge_passw; i++)
//                 EEPROM.write(anfang+laenge_ssid+2+i, uint8_t(passw[i]));
//         EEPROM.commit();
// }
//
// void CleanMemoryWifi(uint8_t anfang){
//         size_t laenge_ssid = size_t(EEPROM.read(anfang));
//         size_t laenge_passw = size_t(EEPROM.read(anfang+laenge_ssid+1));
//         size_t stopc = laenge_ssid + laenge_passw +2;
//         for(size_t i=0; i<stopc; i++)
//                 EEPROM.write(anfang + i, 0);
//         EEPROM.commit();
// }
