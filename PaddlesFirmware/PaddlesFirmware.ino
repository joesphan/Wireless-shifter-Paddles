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

#define TIMEOUT 600000    //10 minutes
#define PORT 4210
#define IP 192, 168, 4, 1
WiFiUDP Udp;

IPAddress ip(IP);

const char* ssid     = STASSID;
const char* password = STAPSK;

byte packet[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int stopTime;

void setup() {
  stopTime = millis() + TIMEOUT;   //reset sleep timer;
  
  pinMode(LEFT_PADDLE_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PADDLE_PIN, INPUT_PULLUP);
  pinMode(HORN_PIN, INPUT_PULLUP);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() == stopTime) {   //sleep if not connected for a long time
      light_sleep();
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
    light_sleep();
  }
}
void readStates() {
  packet[0] = !digitalRead(RIGHT_PADDLE_PIN);
  packet[1] = !digitalRead(LEFT_PADDLE_PIN);
  packet[2] = !digitalRead(HORN_PIN);
}

void sendPacket() {
  Udp.beginPacket(ip, PORT);
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

void light_sleep(){
   wifi_station_disconnect();
   wifi_set_opmode_current(NULL_MODE);
   wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
   wifi_fpm_open(); // Enables force sleep
   gpio_pin_wakeup_enable(GPIO_ID_PIN(RIGHT_PADDLE_PIN), GPIO_PIN_INTR_LOLEVEL);
   wifi_fpm_do_sleep(0xFFFFFFF);
 }
