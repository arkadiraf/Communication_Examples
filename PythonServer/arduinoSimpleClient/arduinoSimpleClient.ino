#include "M5StickCPlus.h"
#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "ZuztaPuzta";
const char* password = "milim888";
const char* server_ip = "192.168.0.105";
const int server_port = 65432;

WiFiClient client;

void setup() {
  M5.begin();
  Serial.begin(115200);
  
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Connect to Python server
  Serial.println("Connecting to server...");
  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
    client.write(c);
  }
  
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}
