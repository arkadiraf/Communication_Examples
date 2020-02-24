/*
  WiFiAccessPoint.ino creates a WiFi access point and provides telnet connection
*/
//#include <M5StickC.h>
#include "M5Atom.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

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

// access point configurations
WiFiServer server(23);
IPAddress ip(192,168,10,1);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

void setup() {
  //M5.begin();
  M5.begin(true, false, true);
    delay(10);
    setBuff(0x00, 0x00, 0x40);
    M5.dis.displaybuff(DisBuff);
  
  WiFi.mode(WIFI_AP);
//  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

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
 WiFiClient client = server.available();    // listen for incoming clients

  if (client) {                             // if you get a client,
    setBuff(0x00, 0x40, 0x00);
    M5.dis.displaybuff(DisBuff);
    
    Serial.println("New Client.");           // print a message out the serial port
    //String currentLine = "";               // make a String to hold incoming data from the client
    unsigned long timeout = millis();
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        timeout = millis();
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
      }else if ((millis() - timeout) > 10000) { // client timeout - search for a better solution
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
      }
      if (Serial.available()){              // if there`s bytes to read from the serial port
        char c = Serial.read();             // read a byte, then
        client.write(c);                    // send in to the client 
        timeout = millis();                 // if the write is unsuccesful it resetes the client connected...      
      } 
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
    setBuff(0x40, 0x00, 0x00);
    M5.dis.displaybuff(DisBuff);
  }    
}
