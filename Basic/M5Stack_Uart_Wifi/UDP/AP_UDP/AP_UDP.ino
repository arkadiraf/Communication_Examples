#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
WiFiUDP Udp; // Creation of wifi Udp instance

char packetBuffer[255];

#define LED_BUILTIN 10   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
// Set these to your desired credentials.
const char* ssid     = "M5StackJoy";
const char* password = "12345678";
// access point configurations
unsigned int localPort = 2050;
IPAddress ip(192,168,10,1);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);
IPAddress clientIP(192,168,10,2);// default ip address
unsigned int clientPort = 2050;

void setup() {
  M5.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  //here config LR mode
  int a= esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N ); //WIFI_PROTOCOL_LR
  //int a= esp_wifi_set_protocol(WIFI_IF_AP,WIFI_PROTOCOL_LR); //WIFI_PROTOCOL_LR
  Serial.print("Mode: ");
  Serial.println(a);
  int8_t power = 0;
  esp_wifi_get_max_tx_power(&power);
  //esp_wifi_set_max_tx_power(78);
  Serial.print("Power (78 max, 19.5dB): ");
  Serial.println(power); 
  WiFi.softAP(ssid, password);  // ESP-32 as access point
  WiFi.softAPConfig(ip,gateway,subnet);
  delay(5);
  //WiFi.begin(ssid, password);
  Udp.begin(localPort);
  delay(5);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) packetBuffer[len-1] = 0;
      Serial.print("Got Packet: ");
      clientIP = Udp.remoteIP();
      clientPort = Udp.remotePort();
      Serial.print(clientIP);Serial.print(" / ");
      Serial.print(clientPort);Serial.print(" / ");
      Serial.print(packetSize);Serial.print(" / ");
      Serial.println(packetBuffer);
      // send packet
      Udp.beginPacket(clientIP,clientPort);
      Udp.printf("received");
      Udp.printf(packetBuffer);
      Udp.printf("\r\n");
      Udp.endPacket();
   }
}
