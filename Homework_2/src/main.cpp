
#include <Arduino.h>

const int RED_LED = 18;
const int GREEN_LED = 19;

const int RED_CHANNEL = 0;
const int GREEN_CHANNEL = 2;

const int RED_FREQUENCY = 1000;
const int GREEN_FREQUENCY = 5000;

const int RESOLUTION = 8;
const int MAX_DUTY = 255;

void setup() {
  Serial.begin(115200);

  ledcSetup(RED_CHANNEL, RED_FREQUENCY, RESOLUTION);
  ledcAttachPin(RED_LED, RED_CHANNEL);

  ledcSetup(GREEN_CHANNEL, GREEN_FREQUENCY, RESOLUTION);
  ledcAttachPin(GREEN_LED, GREEN_CHANNEL);
}

void loop() {
  for (int duty = 0; duty <= MAX_DUTY; duty++) {
    ledcWrite(RED_CHANNEL, duty);
    ledcWrite(GREEN_CHANNEL, MAX_DUTY - duty);
    delay(10);
  }

  for (int duty = MAX_DUTY; duty >= 0; duty--) {
    ledcWrite(RED_CHANNEL, duty);
    ledcWrite(GREEN_CHANNEL, MAX_DUTY - duty);
    delay(10);
  }
}
