#include "timercounter.h"

void TimerCounter::updateFrequency(uint32_t period) {
  this->frequency = period > 0 ? CLOCK_SPEED / period : 0;
}

// Sets the CS12, CS11 & CS10 bits on TCCR1B for the given prescaler.
// If not a valid prescaler (found in prescalers[]) then disables the Timer by setting all bits to zero.
void TimerCounter::setPrescaler(uint16_t scaler) {
  switch (scaler) {
    case 1 : {
        TCCR1B |= (1 << CS10);
        break;
      }
    case 8 : {
        TCCR1B |= (1 << CS11);
        break;
      }
    case 64 : {
        TCCR1B |= (1 << CS11);
        TCCR1B |= (1 << CS10);
        break;
      }
    case 256 : {
        TCCR1B |= (1 << CS12);
        break;
      }
    case 1024 : {
        TCCR1B |= (1 << CS12);
        TCCR1B |= (1 << CS10);
        break;
      }
    default: {  // disable timer
        TCCR1B &= (0 << CS12);
        TCCR1B &= (0 << CS11);
        TCCR1B &= (0 << CS10);
      }
  }
}


uint16_t TimerCounter::prescaler() {
  uint8_t index = TCCR1B & ((1 << CS12) | (1 << CS11) | (1 << CS10));
  return PRESCALERS[index];
}


void TimerCounter::resetTimer() {
  cli();

  // reset prescaler to zero, turns off timer clock.
  setPrescaler(0);

  // Clear control register
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  TIFR1 = 0;  // Clear any Flags set
  TIMSK1 = 0; // Clear all interrupt masks

  OCR1A = 0;
  OCR1B = 0;
  sei();
}


void TimerCounter::startTimer() {
  cli();

  resetTimer();

  // wave gen mode 0, Normal, TOP = 0xFFFF, reset in ICR1 to simulate CTC
  // (CTC using ICR1 as Top (mode 12) disables the input capture pin)
  TCCR1B &= (0 << WGM13) | (0 << WGM12) | (0 << WGM11) | (0 << WGM10);

  // Switch on Input Capture Edge Select (1=Rising, 0=Falling)
  TCCR1B |= (1 << ICES1);

  // enable interrupt ISR for both ICR1 and Overflow
  TIMSK1 |= (1 << ICIE1) | (1 << TOIE1);

  // Turn on the timer in low frequency mode (assuming we'll start with low frequencies).
  setPrescaler(LOW_HZ_PRESCALE);

  sei();

}


void TimerCounter::Overflow() {
  updateFrequency(0);
  
  // Overflow with no capture checks if needs to rescale.
  if (prescaler() == LOW_HZ_PRESCALE)
    return;

  // Switch to low hz scaling by upping the prescaler.
  // This lowers accurcy but gives lower ranges.
  setPrescaler(LOW_HZ_PRESCALE);
}


void TimerCounter::Capture() {
  uint16_t count = ICR1;
  uint16_t prescl = prescaler();
  updateFrequency(prescl * count); // Captured count X prescaler to get count of prescale 1 (maybe Bigger than top)

  // Check if we need to step down the prescaler as frequencies increase.
  if (prescl == HIGH_HZ_PRESCALE)
    return;

  if (frequency >= PRESCALER_THRESHOLD)
    setPrescaler(HIGH_HZ_PRESCALE); // Will be switched back if overflow triggers.

  TCNT1 = 0;  // reset counter;
}



ISR(TIMER1_OVF_vect) {
  InputCounter.Overflow();
}

ISR(TIMER1_CAPT_vect) {
  InputCounter.Capture();
}
