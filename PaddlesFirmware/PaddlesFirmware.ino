/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define STASSID "SimInput"
#define STAPSK  "12345678"

#define RIGHT_PADDLE_PIN 11
#define LEFT_PADDLE_PIN 7
#define HORN_PIN 9
#define ACTIVE_PIN 10

#define TIMEOUT 600000    //10 minutes
WiFiUDP Udp;

IPAddress ip(192, 168, 4, 1);

const char* ssid     = STASSID;
const char* password = STAPSK;

byte packet[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int stopTime;

void setup() {
  stopTime = millis() + TIMEOUT;   //reset timer;
  pinMode(ACTIVE_PIN, OUTPUT);    //disable reset
  digitalWrite(ACTIVE_PIN, HIGH);
  
  pinMode(LEFT_PADDLE_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PADDLE_PIN, INPUT_PULLUP);
  pinMode(HORN_PIN, INPUT_PULLUP);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() == stopTime) {   //sleep if not connected for a long time
      ESP.deepSleep(0);
    }
  }

  packet[0] = 'B';
}

void loop() {
  readStates();
  sendPacket();
  if (packet[0] || packet[1] || packet[2]) {
    stopTime = millis() + TIMEOUT;   //reset timer;
  }
  if (millis() == stopTime) {   //sleep if not connected for a long time
    ESP.deepSleep(0);
  }
}
void readStates() {
  packet[0] = !digitalRead(RIGHT_PADDLE_PIN);
  packet[1] = !digitalRead(LEFT_PADDLE_PIN);
  packet[2] = !digitalRead(HORN_PIN);
}

void sendPacket() {
  Udp.beginPacket(ip, 4210);
  Udp.write(packet[0]);
  Udp.write(packet[1]);
  Udp.write(packet[2]);
  /*
    Udp.write(packet[3]);
    Udp.write(packet[4]);
    Udp.write(packet[5]);
    Udp.write(packet[6]);
    Udp.write(packet[7]);
    Udp.write(packet[8]);
  */
  Udp.endPacket();
}
