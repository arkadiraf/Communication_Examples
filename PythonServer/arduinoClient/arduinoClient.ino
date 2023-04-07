/*
  WiFiAccessPoint.ino creates a WiFi access point and provides telnet connection
*/
#include "M5StickCPlus.h"
#include <WiFi.h>

// M5Stcikc status led
#define LED_BUILTIN 10   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED


// Set these to your desired credentials.
const char* ssid     = "ZuztaPuzta";
const char* password = "milim888";


// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 65432;

void setup() {
    M5.begin();
    Serial.begin(115200);
    delay(10);

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){ // Reconnect to network
    Serial.println("Dropped from network, attempting to recconect");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }else if (!client.connect("192.168.0.105", httpPort)) { // try to connect to the target ip
        Serial.println("connection failed");
        // try to recconnect to network
        WiFi.disconnect();
        Serial.println("Disconnected from network");
  }else{ // connected and all is well
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
      }
      if (Serial.available()){              // if there`s bytes to read from the serial port
        char c = Serial.read();             // read a byte, then
        client.write(c);                    // send in to the client       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Disconnected");
  }    
}
