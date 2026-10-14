
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int BUTTON_PIN = 18;

bool lastDisplayedState = HIGH;
int pressCounter = 0;

void updateDisplay(bool buttonState) {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println(F("ESP32 IoT Homework-1"));

  display.setCursor(0, 20);
  display.print(F("Button: "));

  if (buttonState == LOW) {
    display.println(F("PRESSED"));
  } else {
    display.println(F("RELEASED"));
  }

  display.setCursor(0, 40);
  display.print(F("Count: "));
  display.println(pressCounter);

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {
    }
  }

  updateDisplay(digitalRead(BUTTON_PIN));
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastDisplayedState) {
    lastDisplayedState = reading;

    if (reading == LOW) {
      pressCounter++;

      Serial.print("Button Pressed! Count: ");
      Serial.println(pressCounter);
    }

    updateDisplay(reading);
  }
}
