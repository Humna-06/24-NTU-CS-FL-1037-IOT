#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- Pins ----------
const int RED_LED   = 18;
const int GREEN_LED = 19;
const int SDA_PIN   = 21;
const int SCL_PIN   = 22;

// ---------- LEDC config ----------
// Channel 0 and 2 use different timers, so frequencies are independent
const int RED_CHANNEL   = 0;
const int GREEN_CHANNEL = 2;
const int RED_FREQ      = 1000;   // Hz
const int GREEN_FREQ    = 5000;   // Hz
const int RESOLUTION    = 8;      // 8 bit -> 0..255
const int MAX_DUTY      = 255;

// ---------- OLED ----------
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ---------- Fade state (same values go to PWM and to OLED) ----------
int redDuty = 0,   redDir = 1,   redStep = 1;
int greenDuty = 0, greenDir = 1, greenStep = 2;   // different fade speed

unsigned long lastFade = 0, lastOled = 0;

// Move duty one step, reverse direction at the limits
void fadeStep(int &duty, int &dir, int step) {
  duty += dir * step;
  if (duty >= MAX_DUTY) { duty = MAX_DUTY; dir = -1; }
  if (duty <= 0)        { duty = 0;        dir = 1;  }
}

int toPercent(int duty) { return (duty * 100) / MAX_DUTY; }

void updateOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(25, 0);
  display.println("PWM MONITOR");

  display.setCursor(0, 20);
  display.printf("LED1:%3d%% F:%dHz", toPercent(redDuty), RED_FREQ);

  display.setCursor(0, 40);
  display.printf("LED2:%3d%% F:%dHz", toPercent(greenDuty), GREEN_FREQ);

  display.display();
}

void setup() {
  Serial.begin(115200);

  // PWM setup (Arduino-ESP32 core 2.x API)
  ledcSetup(RED_CHANNEL, RED_FREQ, RESOLUTION);
  ledcAttachPin(RED_LED, RED_CHANNEL);
  ledcSetup(GREEN_CHANNEL, GREEN_FREQ, RESOLUTION);
  ledcAttachPin(GREEN_LED, GREEN_CHANNEL);

  // OLED setup
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found!");
    while (true) delay(1000);
  }
  updateOled();
}

void loop() {
  unsigned long now = millis();

  if (now - lastFade >= 10) {          // fade every 10 ms
    lastFade = now;
    fadeStep(redDuty, redDir, redStep);
    fadeStep(greenDuty, greenDir, greenStep);
    ledcWrite(RED_CHANNEL, redDuty);
    ledcWrite(GREEN_CHANNEL, greenDuty);
  }

  if (now - lastOled >= 100) {         // OLED refresh every 100 ms
    lastOled = now;
    updateOled();
  }
}