/*
  WiFiAccessPoint.ino creates a WiFi access point and provides telnet connection
*/
//#include <M5StickC.h>
#include "M5Atom.h"
#include <WiFi.h>

// M5Stcikc status led
//#define LED_BUILTIN 10   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// M5Stack Atom statues
extern const unsigned char AtomImageData[375 + 2];

uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

// Set these to your desired credentials.
//char ssid[] = "M5StackAP";
//char password[] = "12345678";
const char* ssid     = "M5StackAP";
const char* password = "12345678";


// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 23;

void setup() {
    //M5.begin();
    M5.begin(true, false, true);
    Serial.begin(115200);
    delay(10);
    setBuff(0x00, 0x00, 0x40);
    M5.dis.displaybuff(DisBuff);
  
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
    
//  pinMode(LED_BUILTIN, OUTPUT); 
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
  }else if (!client.connect("192.168.10.1", httpPort)) { // try to connect to the target ip
        Serial.println("connection failed");
        // try to recconnect to network
        WiFi.disconnect();
        Serial.println("Disconnected from network");
  }else{ // connected and all is well
    setBuff(0x00, 0x40, 0x00);
    M5.dis.displaybuff(DisBuff);
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
    setBuff(0x40, 0x00, 0x00);
    M5.dis.displaybuff(DisBuff);
  }    
}
