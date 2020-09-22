/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define STASSID "SimInput"
#define STAPSK  "12345678"

#define LEFT_PADDLE_PIN 14
#define RIGHT_PADDLE_PIN 12

WiFiUDP Udp;

IPAddress ip(192, 168, 4, 1);

const char* ssid     = STASSID;
const char* password = STAPSK;

byte packet[9] = {0,0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  packet[0] = 'B';
  pinMode(LEFT_PADDLE_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PADDLE_PIN, INPUT_PULLUP);
}

void loop() {
  readStates();
  sendPacket();
}
void readStates() {
  packet[1] = !digitalRead(LEFT_PADDLE_PIN);
  packet[2] = !digitalRead(RIGHT_PADDLE_PIN);
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
