#include "M5Atom.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
WiFiUDP Udp;  // Creation of wifi Udp instance

char packetBuffer[255];

// Set these to your desired credentials.
const char* ssid     = "M5StackJoy";
const char* password = "12345678";
// access point configurations
unsigned int localPort = 2050;

IPAddress ipServer(192, 168, 10, 1);   // Declaration of default IP for server
IPAddress ipClient(192, 168, 10, 2);  // Static ip option
IPAddress Subnet(255, 255, 255, 0);

void setup() {
  M5.begin(true, false, true);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // ESP-32 as client
  //here config LR mode
  int a= esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N ); //WIFI_PROTOCOL_LR
  //int a= esp_wifi_set_protocol(WIFI_IF_STA,WIFI_PROTOCOL_LR); //WIFI_PROTOCOL_LR
  // a= 0  configured ok : 
  Serial.print("Mode: ");
  Serial.println(a); 
  //WiFi.config(ipClient, ipServer, Subnet);  // static ip option
  int8_t power = 0;
  esp_wifi_get_max_tx_power(&power);
  //esp_wifi_set_max_tx_power(78);
  Serial.print("Power (78 max, 19.5dB): ");
  Serial.println(power); 
  WiFi.begin(ssid, password);
  //Wifi connection, we connect to master
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  Udp.begin(localPort);
}

void loop() {
     //problems whith connection
    if ( WiFi.status() != WL_CONNECTED ) 
    {
        Serial.println( "|" );
        int tries = 0;
        WiFi.begin( ssid, password );
        while( WiFi.status() != WL_CONNECTED ) {
            tries++;
            Serial.print("attempt: ");
            Serial.print(tries);
            Serial.print(" status: ");
            Serial.println(( WiFi.status()));
            delay( 500 );
        }
        Serial.print( "Connected " );
        Serial.println( WiFi.localIP() );
    }
    

//SENDING
    Udp.beginPacket(ipServer,localPort);   //Initiate transmission of data
    
    Udp.printf("Millis: ");
    
    char buf[20];   // buffer to hold the string to append
    unsigned long testID = millis();   // time since ESP-32 is running millis() 
    sprintf(buf, "%lu", testID);  // appending the millis to create a char
    Udp.printf(buf);  // print the char
    
    Udp.printf("\r\n");   // End segment
    
    Udp.endPacket();  // Close communication
    
    Serial.print("sent: ");   // Serial monitor for user 
    Serial.println(buf);
    
delay(5); // 
 
//RECEPTION
  int packetSize = Udp.parsePacket();   // Size of packet to receive
  if (packetSize) {       // If we received a package
    
    int len = Udp.read(packetBuffer, 255);
    
    if (len > 0) packetBuffer[len-1] = 0;
    Serial.print("Recieved: ");
    Serial.print(Udp.remoteIP());Serial.print(" / ");
    Serial.print(Udp.remotePort()); Serial.print(" / ");
    Serial.print(packetSize);Serial.print(" / ");
    Serial.print(millis());Serial.print(" / ");
    Serial.println(packetBuffer);
  }
Serial.println("");
delay(500);
}
