// Week3-Lecture2
// Timer Interrupt (Internal)
// Embedded IoT System Fall-2026

// Name: Maham                  Reg#: 1044


#include <Arduino.h>
#define LED 2
hw_timer_t *My_timer = NULL;
void ARDUINO_ISR_ATTR onTimer() {           // ARDUINO_ISR_ATTR == IRAM_ATTR
  digitalWrite(LED, !digitalRead(LED));     // safe in ISR on ESP32
}
void setup() {
  pinMode(LED, OUTPUT);
  // Timer 0, prescaler 80 => 1 tick = 1 µs at 80 MHz CPU clock
  My_timer = timerBegin(0, 80, true);
  // attach ISR; true = edge-triggered interrupt
  timerAttachInterrupt(My_timer, &onTimer, true);
  // call ISR every 1,000,000 µs (10 s); autoreload = true
  timerAlarmWrite(My_timer, 18000000, true);
  timerAlarmEnable(My_timer);
}
void loop() {
  // nothing needed, all handled by interrupts
}