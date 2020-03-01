/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/
#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <esp_wifi.h>

#define LED_BUILTIN 10   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
//char ssid[] = "M5StackAP";
//char password[] = "12345678";
const char* ssid     = "M5StackAP";
const char* password = "12345678";

// access point configurations
WiFiServer server(23);
IPAddress ip(192,168,10,1);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  
  WiFi.mode(WIFI_AP);
  pinMode(LED_BUILTIN, OUTPUT);
  int a = esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N ); //WIFI_PROTOCOL_LR
  //int a= esp_wifi_set_protocol(WIFI_IF_STA,WIFI_PROTOCOL_LR); //WIFI_PROTOCOL_LR
  // a= 0  configured ok :
  Serial.print("Mode: ");
  Serial.println(a);
  
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip,gateway,subnet);
  //IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);
  server.begin();

  Serial.println("Server started");
}

void loop() {
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    //String currentLine = "";                // make a String to hold incoming data from the client
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
    Serial.println("Client Disconnected.");
  }
}
