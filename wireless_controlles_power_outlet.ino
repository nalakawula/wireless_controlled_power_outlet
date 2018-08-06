#include <Pushbutton.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoOTA.h>

ESP8266WiFiMulti wifiMulti;

//soft AP
const char *ssid = "SSID";
const char *password = "PASSWD";

ESP8266WebServer server(80);

String getContentType(String filename);
bool handleFileRead(String path);

const int pinTombol = D5;
const int pinLampu = D6;
const int pinKipas = D7;

void setup {
}

void loop {
}
