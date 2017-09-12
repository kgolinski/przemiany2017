#include <mem.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "OSCMessage.h"

#include "network_config.h"

const IPAddress outIp(192, 168, 0, 105);
const unsigned int outPort = 12345;
char message[] = "/height";

/* KONFIGURACJA
 *  Adres IP
 * Port 
 * Komunikat
 * 
 * Dostepne komunikaty
 * /height
 * /density
 * /erosion
 * /dilation
 * /groundColor 
 * /waterLevel 
 * /waterColor
 * /waterOpacity
 * /climate
 * /treeAmount
 * /treeType
 * /treeColor
 * /rotation
 * /cameraAngle
 * /cameraDistance
 * /cameraFov
 * 
 */
 
int lastReading = 1;
WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  wifiSetup();
}

void loop() {
  int reading = analogRead(A0);
  if (reading != lastReading) {
    float val = reading / 1024.0;
    Serial.println(val);
    sendMessage(val);
    lastReading = reading;
  }
  delay(10);
}
void wifiSetup() {
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, KEY);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendMessage(float val){
    OSCMessage msg(message);
    msg.add(val);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
}




