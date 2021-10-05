/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2021-04-29T23:12:09+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: main.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-20T00:57:58+02:00
 * @License: CC by-sa
 */

#include <Arduino.h>
#include <Wire.h>
// #include "SparkFun_Qwiic_Relay.h"
// #include "ArduinoJson.h"
#include <EEPROM.h>// #include <ArduinoOTA.h>
 //#include <FS.h>
 #include <SPIFFS.h>
// #include <ESPmDNS.h>
 #include <WiFi.h>
 #include "WebServer.h"
// #include <AsyncTCP.h>
//  #include <ESPAsyncWebServer.h>
// #include <SPIFFSEditor.h>
#include "WeatherStat_NTP.h"
#include "WeatherStat_Messages.h"
#include "WeatherStat_CO2sensor.h"
#include "WeatherStat_Memory.h"
#include "WeatherStat_Zambretti.h"
#define BSEC_MAX_STATE_BLOB_SIZE     (139)
// #include <ESP8266WiFi.h> //Archivos originales
// #include <ESP8266WebServer.h>
// #include <FS.h>

const char* htmlFile = "/index.html";
// const char* ssid = "NodeMCU";
// const char* password = "xxxxxxxxxxx";
char *ssid = "FRITZ!Box 6591 Cable SW";           // replace with your SSID
char *password = "62407078731195560963";
//char *ssid = "FRITZ!Box 6591 Cable BE";          // replace with your SSID
//char *password = "07225443701792235194";  // replace with your Password
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;  //Germany is GMT +1, expressed in seconds
const int daylightOffset_sec = 3600;
enum netmode {CONNECTED_TO_INTERNET, WIRELESS_ACCESS_POINT};
netmode Network_status;
int count = 0;
String cadena_envio;
uint8_t contador=10;
int dummyco2, dummyiaq, dummybreathvoc;
volatile int i=0;
volatile int minutes=0;
float varCo2M=400.0;
//ESP8266WebServer server(80);// Linea original
WebServer server(80);
TaskHandle_t Task2;
TaskHandle_t Task1;
TaskHandle_t Task5;
void loop1(void *parameter);
void loop2(void *parameter);
void loop5(void *parameter);

void handleRoot()
{
        server.sendHeader("Location", "/index.html", true);
        server.send(302, "text/plane", "Hola Hola Hola");
}


bool loadFromSpiffs(String path)
{
        String dataType = "text/plain";

        if(path.endsWith("/"))
                path += "index.htm";

        if(path.endsWith(".src"))
                path = path.substring(0, path.lastIndexOf("."));

        else if(path.endsWith(".html"))
                dataType = "text/html";


        else if(path.endsWith(".css"))
                dataType = "text/css";

        else if(path.endsWith(".js"))
                dataType = "application/javascript";



        File dataFile = SPIFFS.open(path.c_str(), "r");
        if(server.hasArg("download"))
                dataType = "application/octet-stream";
        if(server.streamFile(dataFile, dataType) != dataFile.size())
        {

        }

        dataFile.close();
        return true;
}



void handleWebRequests()
{
        if(loadFromSpiffs(server.uri())) return;

        String message = "File not detected\n\n";
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET)?"GET":"POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";

        for(uint8_t i=0; i<server.args(); i++)
        {
                message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
        }

        server.send(404, "text/plain", message);
        Serial.println(message);
}

void setup() {
        // put your setup code here, to run once:
        Serial.begin(115200);
        EEPROM.begin(512);
        //Serial.print("EEMPROM: ");
        //Serial.println(EEPROM.length());
        delay(1000);

        pinMode(MHZ19_PWM_PIN, INPUT);
        Serial.println();

        SPIFFS.begin();
        Serial.println("File system initialized");
        // SaveSSID(START_DATA_WIFI, ssid);
        // SavePASSW(START_DATA_WIFI, password);
        // ESP.restart();
        //CleanMemoryWifi(START_DATA_WIFI);
        char *ssid = RetrieveSSID(START_DATA_WIFI);
        char *password = RetrievePASSW(START_DATA_WIFI);
        if(ssid && password)
        {
                Serial.print("Red encontrada: ");
                Serial.println(ssid);
                Serial.print("Contraseña encontrada: ");
                Serial.println(password);
        }
        else{
                Serial.println("Contraseña o password no encontrado");
                ssid = (char*) calloc( strlen("FRITZ!Box 6591 Cable SW")+1,sizeof(char) );
                strcpy(ssid,"FRITZ!Box 6591 Cable SW");     // replace with your SSID
                password = (char*) calloc( strlen("62407078731195560963")+1,sizeof(char) );
                strcpy(password, "62407078731195560963");
        }
        delay(3000);
        WiFi.begin(ssid, password);//Apesta menos
        int k = 0;
        while (WiFi.status() != WL_CONNECTED) {
                k++;
                delay(1000);
                Serial.println("Connecting to WiFi..");
                if(k == 10) ESP.restart();
        }
        free(ssid);
        free(password);
        // Print ESP32 Local IP Address
        Serial.println(WiFi.localIP());
        Network_status = CONNECTED_TO_INTERNET;
        ////////////
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        SaveMonth(getMonat());
        Serial.println("Mes guardado " + String(SavedMonth()));
        //ResetForecastData();
        server.on("/", handleRoot);
        server.on("/info", [](){
                server.send(200, "application/json", cadena_envio);
        });
        server.on("/temperature", HTTP_GET,[](){
                server.send(200, "text/json", String(contador));
        });
        server.on("/iaq", HTTP_GET,[](){
                server.send(200, "text/json", String(dummyiaq));
        });
        server.on("/co2", HTTP_GET,[](){
                server.send(200, "text/json", String(varCo2M ));
        });
        server.on("/breathvoc_h", HTTP_GET,[](){
                server.send(200, "text/json", String(dummybreathvoc));
        });
        server.onNotFound(handleWebRequests);
        server.begin();

        xTaskCreatePinnedToCore(loop1,"Task_1",20000,NULL,2,&Task1,0);
        delay(500);
        xTaskCreatePinnedToCore(loop2,"Task_2",10000,NULL,1,&Task2,1);
        delay(500);
        xTaskCreatePinnedToCore(loop5,"Task_5",20000,NULL,1,&Task5,0);
        delay(500);

}

void loop(){
        i++;
        if(i==60) {
                minutes++;
                i=0;
        }
        yield();
        //Serial.println(cadena_envio);
        vTaskDelay(1000);
}

void loop1(void *parameter) {
        // put your main code here, to run repeatedly:
        while(true) {
                if (millis()%3000==0)
                {
                        contador++;
                        //cadena_envio = String(contador)+"-"+String(contador*);
                        //return (String)"[\"" + temp + "\",\"" + hum + "\",\"" + sealevel + "\"]";
                        // cadena_envio = "[\"" + String(contador) + "\",\"" + String(contador*2) + "\",\"" + String(contador*3)+ "\"]";
                        cadena_envio = StartJS;//Initializer JSON chain
                        cadena_envio += String(contador) + SpacerJS;//temp//0
                        cadena_envio += String(contador*2) + SpacerJS;//pres//1
                        cadena_envio += String(contador*3) + SpacerJS;//hum//2
                        cadena_envio += getDatum(IN_LETTERS)+" "+ getZeit()+ SpacerJS;//fecha//3
                        cadena_envio += messages_runin_stat[1] +SpacerJS;//status//4
                        dummyco2 = random(1,1000);
                        cadena_envio += String(varCo2M ) + SpacerJS; //co2 mess//5
                        cadena_envio += String(dummyco2-random(10,50)) + SpacerJS; //co2 stimation//6
                        int dummyaccuracy = random(1,4);
                        cadena_envio += messages_accuracy[dummyaccuracy] + SpacerJS; //co2 stimation accuracy//7
                        cadena_envio += "info o medidas a tomar" + SpacerJS; //co2 suggested acti//8
                        dummybreathvoc = random(1,200);
                        cadena_envio += String(dummybreathvoc) + SpacerJS;//breath VOC//9
                        cadena_envio += messages_accuracy[dummyaccuracy] + SpacerJS;//breath VOC accuracy//10
                        cadena_envio += String(random(1,100)) + SpacerJS;//Gas percentage//11
                        cadena_envio += messages_accuracy[dummyaccuracy] + SpacerJS;//Gas percentage accuracy//12
                        dummyiaq = random(0,500);
                        cadena_envio += String(dummyiaq)+ SpacerJS;//IAQ//13
                        cadena_envio += messages_accuracy[dummyaccuracy]+ SpacerJS;//IAQ Accuracy//14
                        cadena_envio += messages_impact[iaq_Index2Level(dummyiaq)]+ SpacerJS;//IAQ Impact//15
                        cadena_envio += messages_saction[iaq_Index2Level(dummyiaq)]+ SpacerJS;//IAQ Suggested actions//16
                        cadena_envio += messages_iaqcolors[iaq_Index2Level(dummyiaq)]+ SpacerJS;//IAQ Color//17
                        cadena_envio += messages_quality[iaq_Index2Level(dummyiaq)]+ SpacerJS;//IAQ quality //18
                        //cadena_envio += "whatsgoingon" + SpacerJS;
                        cadena_envio += weather_forecast[random(1,27)]+ SpacerJS;//Forecast//19
                        cadena_envio += "holo";//por si acaso//20
                        // cadena_envio += "oh_no_me_da_amsiedad";
                        cadena_envio += StopJS;//Finalizer JSON chain
                        //long sdf=random(1,500);
                        //Serial.println(cadena_envio);
                }

                server.handleClient();
        }
        vTaskDelay(10);
}

void loop2(void *parameter){

        while(true) {
                //Serial.println("---in loop2 before measure");
                yield();
                varCo2M = get_CO2_measure();

                //Serial.print("---in loop2 after measure: ");
                //Serial.println(varCo2M);
                vTaskDelay(5);
        }

}

void loop5(void *parameter){

        int sec=59;//regresar a 59
        int min=9;//regresar a 9
        int monate=0;
        int Zamb=0;
        int presion=1000;
        int Presiones[10];

        while(1) {
                if(sec==60)//regresar a 60
                {
                        sec = 0;
                        min++;
                }
                else sec++;

                if(min==10) ///cambiar aqui, poner 10 (minutos)
                {

                        Serial.println("CICLO CADA 10 MINUTOS " + getZeit());

                        if(Network_status==CONNECTED_TO_INTERNET)//lee el mes ya sea de internet o memoria
                                monate = getMonat();
                        else
                                monate = SavedMonth();

                        Serial.println(" Presion actual falsa: "+String(presion));
                        PushPressure(presion);//mete la presion actual en la fila

                        if(ForecastReady())// ya se guardaron 10 presiones?
                        {
                                Serial.println("    <<<< Forecast ready");
                                Serial.print("    "+String(GetNumbePress())+" - ");
                                GetSavedPressures(Presiones);
                                for (uint8_t i = 0; i < 10; i++) {
                                        Serial.print(String(Presiones[i])+" ");
                                }
                                Serial.println(" ");
                                // Zamb=calc_zambretti((P010+P09+P08)/3,(P03+P02+P01)/3, month); // Aprox average of last measurement in 30 min, and 90-120 ago.


                        }
                        else// no se han guardado 10 presiones...
                        {
                                Serial.println("   <<<< " + String(weather_forecast[26]) );
                                Serial.print("    "+String(GetNumbePress())+" - ");
                                GetSavedPressures(Presiones);
                                for (uint8_t i = 0; i < 10; i++) {
                                        Serial.print(String(Presiones[i])+" ");
                                }
                                Serial.println(" ");
                        }

                        if(presion==1090) presion=1000;//esto es de prueba
                        presion++;//solo aumenta el valor de la presion a lo pendejo

                        min = 0;//resetea el timer!
                        Serial.println(" ");
                }
                vTaskDelay(1000);//aguanta 1 segundo we
        }

}


// #define RELAY_ADDR 0x6D
// #define INCORR_PARAM 0xFF
// #define SUCCESS 0x00
// Qwiic_Relay quadRelay(RELAY_ADDR);
//
//
//
// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");
// AsyncEventSource events("/events");
//
// String configPath = "/home.json";
// char ssid[30] = "FRITZ!Box 6591 Cable SW";         // replace with your SSID
// char password[30] = "62407078731195560963";
// // char ssid[30] = "";
// // char password[30] = "";
// const char* hostName = "downstairs";
// const char* http_username = "admin";
// const char* http_password = "admin";
//
// // A local way to save the state of our relays without relying on the
// // serialization process of the Arduino Json library.
// void wifiSettings(){
//
//   // SPIFFS.open() supports all Stream methods
//   //https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
//   File configFile = SPIFFS.open(configPath, "r");
//   if(!configFile)
//     Serial.println("Could not open file.");
//
//   uint16_t size = configFile.size();
//
//   // Do I want to allocate size according to the size above?
//   // Hard code the bytes according to ArduinoJSON web page.
//   const size_t capacity = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(3) + 100;
//   DynamicJsonDocument doc(capacity);
//
//   DeserializationError err = deserializeJson(doc, configFile);
//
//   if(err) {
//     Serial.print("Deserialization error:");
//     Serial.println(err.c_str());
//   }
//
//   doc["SSID"].as<String>().toCharArray(ssid, 30);
//   doc["Password"].as<String>().toCharArray(password, 30);
//
//   configFile.close();
//
// }
//
// // Get the state of the relay.... Will need to write this to a JSON file.
// byte getRelayStatus(int relay){
//
//   if( relay < 1 || relay > 4)
//     return INCORR_PARAM;
//
//   int relay_state = quadRelay.getState(relay);
//   return relay_state;
//
// }
//
// byte turnOnRelay(int relay){
//
//   if( relay < 1 || relay > 5)
//     return INCORR_PARAM;
//
//   quadRelay.turnRelayOn(relay);
//   int relay_state = quadRelay.getState(relay);
//   // Write JSON here....
//
//   if (relay == 5){
//     quadRelay.turnAllRelaysOn();
//     return SUCCESS;
//   }
//
//   return SUCCESS;
// }
//
// byte turnOffRelay(int relay){
//
//   if( relay < 1 || relay > 5)
//     return INCORR_PARAM;
//
//   quadRelay.turnRelayOff(relay);
//   int relay_state = quadRelay.getState(relay);
//   // Write JSON here...
//
//   if (relay == 5){
//     quadRelay.turnAllRelaysOff();
//     return SUCCESS;
//   }
//
//   return SUCCESS;
// }
//
// void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
//         if(type == WS_EVT_CONNECT) {
//                 Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
//                 client->printf("Hello Client %u :)", client->id());
//                 client->ping();
//         } else if(type == WS_EVT_DISCONNECT) {
//                 Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
//         } else if(type == WS_EVT_ERROR) {
//                 Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
//         } else if(type == WS_EVT_PONG) {
//                 Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
//         } else if(type == WS_EVT_DATA) {
//                 AwsFrameInfo * info = (AwsFrameInfo*)arg;
//                 String msg = "";
//                 if(info->final && info->index == 0 && info->len == len) {
//                         //the whole message is in a single frame and we got all of it's data
//                         Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
//
//                         if(info->opcode == WS_TEXT) {
//                                 for(size_t i=0; i < info->len; i++) {
//                                         msg += (char) data[i];
//                                 }
//                         } else {
//                                 char buff[3];
//                                 for(size_t i=0; i < info->len; i++) {
//                                         sprintf(buff, "%02x ", (uint8_t) data[i]);
//                                         msg += buff;
//                                 }
//                         }
//                         Serial.printf("%s\n",msg.c_str());
//
//                         if(info->opcode == WS_TEXT)
//                                 client->text("I got your text message");
//                         else
//                                 client->binary("I got your binary message");
//                 } else {
//                         //message is comprised of multiple frames or the frame is split into multiple packets
//                         if(info->index == 0) {
//                                 if(info->num == 0)
//                                         Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
//                                 Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
//                         }
//
//                         Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
//
//                         if(info->opcode == WS_TEXT) {
//                                 for(size_t i=0; i < len; i++) {
//                                         msg += (char) data[i];
//                                 }
//                         } else {
//                                 char buff[3];
//                                 for(size_t i=0; i < len; i++) {
//                                         sprintf(buff, "%02x ", (uint8_t) data[i]);
//                                         msg += buff;
//                                 }
//                         }
//                         Serial.printf("%s\n",msg.c_str());
//
//                         if((info->index + len) == info->len) {
//                                 Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
//                                 if(info->final) {
//                                         Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
//                                         if(info->message_opcode == WS_TEXT)
//                                                 client->text("I got your text message");
//                                         else
//                                                 client->binary("I got your binary message");
//                                 }
//                         }
//                 }
//         }
// }
//
// // This loads the global structs that hold such information
// struct relayStates {
//         uint8_t relay_one;
//         uint8_t relay_two;
//         uint8_t relay_three;
//         uint8_t relay_four;
// };
//
// // char ssid[30] = "";
// // char password[30] = "";
// // const char* hostName = "downstairs";
// // const char* http_username = "admin";
// // const char* http_password = "admin";
//
// relayStates rStates;
//
//
// //http://downstairs.local/index
// //http://downstairs.local/relay.html
// void setup(){
//
//
//         Serial.begin(115200);
//         Serial.setDebugOutput(true);
//
//         if(SPIFFS.begin()) Serial.println("Mounted Successfully");
//
//         wifiSettings();
//
//         WiFi.mode(WIFI_AP_STA);
//         WiFi.softAP(hostName);
//
//         WiFi.begin(ssid, password);
//
//         if (WiFi.waitForConnectResult() != WL_CONNECTED) {
//                 Serial.printf("STA: Failed!\n");
//                 WiFi.disconnect(false);
//                 delay(1000);
//                 WiFi.begin(ssid, password);
//         }
//
//         Serial.print("Local IP: ");
//         Serial.println(WiFi.localIP());
//
//         //Send OTA events to the browser
//         ArduinoOTA.onStart([]() {
//                 events.send("Update Start", "ota");
//         });
//         ArduinoOTA.onEnd([]() {
//                 events.send("Update End", "ota");
//         });
//
//         ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//                 char p[32];
//                 sprintf(p, "Progress: %u%%\n", (progress/(total/100)));
//                 events.send(p, "ota");
//         });
//
//         ArduinoOTA.onError([](ota_error_t error) {
//                 if(error == OTA_AUTH_ERROR) events.send("Auth Failed", "ota");
//                 else if(error == OTA_BEGIN_ERROR) events.send("Begin Failed", "ota");
//                 else if(error == OTA_CONNECT_ERROR) events.send("Connect Failed", "ota");
//                 else if(error == OTA_RECEIVE_ERROR) events.send("Recieve Failed", "ota");
//                 else if(error == OTA_END_ERROR) events.send("End Failed", "ota");
//         });
//
//         ArduinoOTA.setHostname(hostName);
//         ArduinoOTA.begin();
//
//         MDNS.addService("http","tcp",80);
//
//         ws.onEvent(onWsEvent);
//         server.addHandler(&ws);
//
//         events.onConnect([](AsyncEventSourceClient *client){
//                 client->send("hello!",NULL,millis(),1000);
//         });
//         server.addHandler(&events);
//
//         server.addHandler(new SPIFFSEditor(SPIFFS, http_username,http_password));
//
//         // Bootstrap -------------------
//         server.on("/bootstrap/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
//                 request->send(SPIFFS, "/bootstrap/bootstrap.min.css", "text/css");
//         });
//
//         server.on("/jquery/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
//                 request->send(SPIFFS, "/jquery/jquery.min.js", "text/js");
//         });
//
//         server.on("/bootstrap/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
//                 request->send(SPIFFS, "/bootstrap/bootstrap.min.js", "text/js");
//         });
//         // Bootstrap End -----------------
//
//         // Relay visuals -----------------
//         server.on("relay_scripts.js", HTTP_GET, [](AsyncWebServerRequest *request){
//                 request->send(SPIFFS, "relay_scripts.js", "text/js");
//         });
//
//         // When a button is pressed
//         // -------Relay One---------
//         server.on("/relay.html/RELAY_1_ON", HTTP_GET, [](AsyncWebServerRequest *request){
//                 quadRelay.toggleRelay(1);
//                 request->send(200);
//         });
//
//         // -------Relay Two---------
//         server.on("/relay.html/RELAY_2_ON", HTTP_GET, [](AsyncWebServerRequest *request){
//                 quadRelay.toggleRelay(2);
//                 request->send(200);
//         });
//
//         server.serveStatic("/", SPIFFS, "/").setDefaultFile("relay.html");
//
//         server.onNotFound([](AsyncWebServerRequest *request){
//                 Serial.printf("NOT_FOUND: ");
//                 if(request->method() == HTTP_GET)
//                         Serial.printf("GET");
//                 else if(request->method() == HTTP_POST)
//                         Serial.printf("POST");
//                 else if(request->method() == HTTP_DELETE)
//                         Serial.printf("DELETE");
//                 else if(request->method() == HTTP_PUT)
//                         Serial.printf("PUT");
//                 else if(request->method() == HTTP_PATCH)
//                         Serial.printf("PATCH");
//                 else if(request->method() == HTTP_HEAD)
//                         Serial.printf("HEAD");
//                 else if(request->method() == HTTP_OPTIONS)
//                         Serial.printf("OPTIONS");
//                 else
//                         Serial.printf("UNKNOWN");
//                 Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());
//
//                 if(request->contentLength()) {
//                         Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
//                         Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
//                 }
//
//                 int headers = request->headers();
//                 int i;
//                 for(i=0; i<headers; i++) {
//                         AsyncWebHeader* h = request->getHeader(i);
//                         Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
//                 }
//
//                 int params = request->params();
//                 for(i=0; i<params; i++) {
//                         AsyncWebParameter* p = request->getParam(i);
//                         if(p->isFile()) {
//                                 Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//                         } else if(p->isPost()) {
//                                 Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//                         } else {
//                                 Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//                         }
//                 }
//
//                 request->send(404);
//         });
//         server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
//                 if(!index)
//                         Serial.printf("UploadStart: %s\n", filename.c_str());
//                 Serial.printf("%s", (const char*)data);
//                 if(final)
//                         Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
//         });
//         server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//                 if(!index)
//                         Serial.printf("BodyStart: %u\n", total);
//                 Serial.printf("%s", (const char*)data);
//                 if(index + len == total)
//                         Serial.printf("BodyEnd: %u\n", total);
//         });
//         server.begin();
//
//         // Quad Relay
//         Wire.begin();
//         if (quadRelay.begin()) {
//                 Serial.println("Ready to flip some switches.");
//         }
//         else
//                 Serial.println("Could not communicate with Quad Relay.");
//
// }
//
// void loop(){
//         ArduinoOTA.handle();
// }


// else if(path.endsWith(".png"))
//   dataType = "image/png";
//
// else if(path.endsWith(".gif"))
//   dataType = "image/gif";
//
// else if(path.endsWith(".jpg"))
//   dataType = "image/jpeg";
//
// else if(path.endsWith(".ico"))
//   dataType = "image/x-icon";
//
// else if(path.endsWith(".xml"))
//   dataType = "text/xml";
//
// else if(path.endsWith(".pdf"))
//   dataType = "application/pdf";
//
// else if(path.endsWith(".zip"))
//   dataType = "application/zip";
