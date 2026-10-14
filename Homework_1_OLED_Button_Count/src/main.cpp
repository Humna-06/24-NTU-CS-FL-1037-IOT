
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int BUTTON_PIN = 18;
const unsigned long debounceDelay = 50;

bool lastButtonState = HIGH;
bool currentButtonState = HIGH;

unsigned long lastDebounceTime = 0;
int pressCounter = 0;

void updateDisplay() {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println(F("ESP32 IoT Homework-1"));

  display.setCursor(0, 16);
  display.println(F("Status: ACTIVE"));

  display.setCursor(0, 30);
  display.print(F("Button: "));

  if (currentButtonState == LOW) {
    display.println(F("PRESSED"));
  } else {
    display.println(F("RELEASED"));
  }

  display.setCursor(0, 48);
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

  updateDisplay();
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      if (currentButtonState == LOW) {
        pressCounter++;

        Serial.print("Button Pressed! Count: ");
        Serial.println(pressCounter);
      }

      updateDisplay();
    }
  }

  lastButtonState = reading;
}
