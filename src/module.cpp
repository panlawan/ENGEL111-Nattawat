#include "module.h"

const char* ssid = "nattawat";            // WiFi SSID
const char* password = "";  // WiFi PSK

AsyncWebServer server(80); //http

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_wifiAP(){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    Serial.print("\nWiFi shared on SSID: ");
    Serial.print(ssid);
    Serial.print(" ,PSK: ");
    Serial.println(password);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());  //Default 192.168.4.1
}

void ConfigServer(){
  Serial.begin(115200);
  
  // 1st step starting SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("Error Strating SPIFFS!!!");
    return;
  }

  // 2nd step Shere WiFi AP
  setup_wifiAP();

  // 3rd step Create DNS 
  if(!MDNS.begin("nattawat")){
    Serial.println("Error Starting DNS");
    return;
  }

  // 4th step Server on script and css file
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/styles.css"); });
  server.on("/bscripts.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/bscripts.js"); });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });
  server.on("/shop", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/shop.html"); });

// nattawat.local
  MDNS.addService("http", "tcp", 80);
  server.begin();
}

void handleIndex(AsyncWebServerRequest *request){

}