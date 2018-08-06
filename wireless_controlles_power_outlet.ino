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

Pushbutton tombol(pinTombol);

void handleLampu();
void handleKipas();

void setup {
  pinMode(pinLampu, OUTPUT);
  pinMode(pinKipas, OUTPUT);

  digitalWrite(pinLampu, LOW);
  digitalWrite(pinKipas, LOW);

  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  //multi SSID
  wifiMulti.addAP("dd-wrt_vap", "Kero.007.123");
  wifiMulti.addAP("Viva", "hidupsehat");

  Serial.println("Connecting ...");
  int i = 0;

  //coba konek
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
  }

  //fail over ke mode AP jika gagal konek
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println('\n');
    Serial.print("Can not connect to AP, fallback to AP Mode ");
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  }

  if (MDNS.begin("esp8266")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}

void loop {
}

void handleLampu() {
  digitalWrite(pinLampu, !digitalRead(pinLampu));
  server.sendHeader("location", "/");
  server.send(303);
}

void handleKipas() {
  digitalWrite(pinKipas, !digitalRead(pinKipas));
  server.sendHeader("location", "/");
  server.send(303);
}
