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

void setup() {
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

  //blok ota
  ArduinoOTA.setHostname("ESP8266");
  ArduinoOTA.setPassword("esp8266");
  
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");


  SPIFFS.begin();
  
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.on("/lampu", handleLampu);
  server.on("/kipas", handleKipas);

  server.begin();
  Serial.println("HTTP server started");
}  
  
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  if (tombol.getSingleDebouncedRelease()) {
    digitalWrite(pinLampu, !digitalRead(pinLampu));
    digitalWrite(pinKipas, !digitalRead(pinKipas));
  }
}

//kompresi
String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
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
