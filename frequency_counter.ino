// Example of timecounter, counts the frequency detected on the TIMER_INPUT_PIN

// ref: http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48_88_168_megaAVR-Data-Sheet-40002074.pdf
// https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
// https://forum.arduino.cc/index.php?topic=638989.0


#include "timercounter.h"

unsigned long lastShown = 0;
uint16_t lastFrequency = -1;

const long timeFrame = 50;

TimerCounter *inputCounter = new TimerCounter();

void setup () {
  Serial.begin(38400);
  while (!Serial);
  Serial.println("Frequency Counter");
  Serial.print("Counts the frequency detected on the TIMER_INPUT_PIN (");
  Serial.print(TIMER_INPUT_PIN);
  Serial.println(") in hz.  Range from 1 hz > 5mhz+");

  pinMode(TIMER_INPUT_PIN, INPUT_PULLUP);

  Serial.print("prestart prescaler: ");
  Serial.println(inputCounter->prescaler());

  inputCounter->startTimer();
}

void loop () {
  if (lastShown < millis()) {
    uint16_t hz;
    hz = inputCounter->Frequency();
    if (hz != lastFrequency) {
      lastFrequency = hz;
      Serial.print("Frequency: ");
      Serial.print(hz);
      Serial.print(" hz");
      Serial.println();
    }
    lastShown = millis() + timeFrame;
  }
}

ISR(TIMER1_OVF_vect) {
  inputCounter->Overflow();
}

ISR(TIMER1_CAPT_vect) {
  inputCounter->Capture();
}
