/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2021-04-29T23:12:09+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: main.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-08T01:48:19+02:00
 * @License: CC by-sa
 */

#include <Arduino.h>
#include <Wire.h>
// #include "SparkFun_Qwiic_Relay.h"
// #include "ArduinoJson.h"
//
// #include <ArduinoOTA.h>
 #include <FS.h>
 #include <SPIFFS.h>
// #include <ESPmDNS.h>
 #include <WiFi.h>
 #include "WebServer.h"
// #include <AsyncTCP.h>
//  #include <ESPAsyncWebServer.h>
// #include <SPIFFSEditor.h>

// #include <ESP8266WiFi.h> //Archivos originales
// #include <ESP8266WebServer.h>
// #include <FS.h>

const char* htmlFile = "/index.html";
// const char* ssid = "NodeMCU";
// const char* password = "xxxxxxxxxxx";
const char *ssid = "FRITZ!Box 6591 Cable SW";         // replace with your SSID
const char *password = "62407078731195560963";
int count = 0;

//ESP8266WebServer server(80);// Linea original
WebServer server(80);

void handleRoot()
{
  server.sendHeader("Location", "/index.html", true);
  server.send(302, "text/plane", "");
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

  else if(path.endsWith(".htm"))
    dataType = "text/html";

  else if(path.endsWith(".css"))
    dataType = "text/css";

  else if(path.endsWith(".js"))
    dataType = "application/javascript";

  else if(path.endsWith(".png"))
    dataType = "image/png";

  else if(path.endsWith(".gif"))
    dataType = "image/gif";

  else if(path.endsWith(".jpg"))
    dataType = "image/jpeg";

  else if(path.endsWith(".ico"))
    dataType = "image/x-icon";

  else if(path.endsWith(".xml"))
    dataType = "text/xml";

  else if(path.endsWith(".pdf"))
    dataType = "application/pdf";

  else if(path.endsWith(".zip"))
    dataType = "application/zip";

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

  for(uint8_t i=0;i<server.args();i++)
  {
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  Serial.println(message);
}

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  SPIFFS.begin();
  Serial.println("File system initialized");
  //
  // WiFi.mode(WIFI_AP);//Codigo original: apesta
  // WiFi.begin(ssid, password);
  //
  // //WiFi.softAP(ssid);
  // Serial.println("");
  // Serial.println(WiFi.localIP());
  WiFi.begin(ssid, password);//Apesta menos
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
    ////////////


  server.on("/", handleRoot);
  server.onNotFound(handleWebRequests);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
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
