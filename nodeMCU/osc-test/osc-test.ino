#include <mem.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <OSCMessage.h>

#include "network_config.h"

WiFiUDP Udp;
const IPAddress outIp(192, 168, 0, 105);
const unsigned int outPort = 12345;
char message[] = "/height";
int lastReading = 1;

void setup() {
  Serial.begin(115200);
  wifi_setup();

}
void wifi_setup() {
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

void loop() {

  
  int reading = analogRead(A0);
  
  if (reading != lastReading) {
    //float val = (sin(millis()/1000.0)+1.0)/2.0;
    float val = reading / 1024.0;
    Serial.println(val);
    OSCMessage msg(message);
    msg.add(val);

    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    lastReading = reading;
  }
  delay(10);
}









