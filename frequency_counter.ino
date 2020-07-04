// Example of timecounter, counts the frequency detected on the TIMER_INPUT_PIN

// ref: http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48_88_168_megaAVR-Data-Sheet-40002074.pdf
// https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
// https://forum.arduino.cc/index.php?topic=638989.0


#include "timercounter.h"

unsigned long lastShown = 0;
uint16_t lastFrequency = 0;

const long timeFrame = 500;

void setup () {
  Serial.begin(38400);
  while (!Serial);
  Serial.println("Frequency Counter");
  Serial.print("Counts the frequency detected on the TIMER_INPUT_PIN (");
  Serial.print(TIMER_INPUT_PIN);
  Serial.println(") in hz.  Range from 1 hz > 5mhz+");

  pinMode(TIMER_INPUT_PIN, INPUT_PULLUP);
}

void loop () {
  unsigned long now = millis();

  if (lastShown < now) {
    uint16_t hz;
    noInterrupts();
    hz = InputCounter.Frequency();
    interrupts();

    if (hz != lastFrequency) {
      lastFrequency = hz;
      Serial.print("Frequency: ");
      Serial.println(hz);
      Serial.print(" hz");
      Serial.println();
    }
    lastShown = millis() + timeFrame;
  }
  
}
