/*
   Joesphan Lu
   Firmware for Wireless Flappy Paddles
   433mhz ASK based
*/
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile


#define RF_TX_PIN 4
#define LEFT_PADDLE 2
#define RIGHT_PADDLE 3


RH_ASK driver(2000, -1, RF_TX_PIN, 0);

uint8_t leftPaddleState = 0;
uint8_t rightPaddleState = 0;

void setup() {
  //rf init
  driver.init();

  pinMode(LEFT_PADDLE, INPUT_PULLUP);
  pinMode(RIGHT_PADDLE, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(RIGHT_PADDLE), right, CHANGE);
  
  attachInterrupt(digitalPinToInterrupt(LEFT_PADDLE), left, CHANGE);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void right() {
  rightPaddleState = !digitalRead(RIGHT_PADDLE);
}
void left() {
  leftPaddleState = !digitalRead(LEFT_PADDLE);
}

void loop() {
  char *sendPaddleState;
  //A, B, C, D states
  
  if (!leftPaddleState && !rightPaddleState) {
    sendPaddleState = "0";
  } else if (!leftPaddleState && rightPaddleState) {
    sendPaddleState = "1";
  } else if (leftPaddleState && !rightPaddleState) {
    sendPaddleState = "2";
  } else if (leftPaddleState && rightPaddleState) {
    sendPaddleState = "3";
  }
  
  
  driver.send((uint8_t *)sendPaddleState, strlen(sendPaddleState));
  driver.waitPacketSent();
}
