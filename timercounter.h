// Counter using Input Capture Unit on Timer1.
// In WGM mode 12 / CTC on ICR1, it resets the counter when an input capture is triggered.
// On capture, ICF1 ISR samples the prescaler / and ICR1 value to get timebase.  Clock_speed / timebase = frequency.
// Capture ISR also checks if the newly sampled pulse is above the prescaler cutoff (245hz) and switches timer to Fast mode.

// Overflow ISR trigger the autoprescaler.
// Timer auto scales its prescaler to extend the range of frequencies it samples.
// Using a fixed threshold of 245hz to simplify operation, it switches between prescaler 256 for low frequencies and 1 for anything above 245hz
// 245 hz = point at which both prescalers can scale to.
// If TOV1 (overflow interrupt) triggers = no input in that phase, prescaler is checked and updated if needed to 256.
// If already 256, input frequency is zero (or below sample rate of 1hz) so Timer1 resets it frequencies.

// on Arduino UNO Pin 8 (ICP1 pin).
// Note: Since this uses Timer1, Pin 9 and Pin 10 can't be used for
// analogWrite().

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// 16mz cpu clock speed
#define CLOCK_SPEED 16000000
// tick = 0.0625μs | 62.5ns / clock tick)

#define PRESCALER_COUNT 6     // The number of prescalers available to this timer (Including zero = off)
#define PRESCALER_THRESHOLD 400
#define LOW_HZ_PRESCALE 256
#define HIGH_HZ_PRESCALE 0x01

#define TIMER_INPUT_PIN 8  // ICP1 = D08


class TimerCounter {
  private:
    volatile uint16_t frequency;

    // A fixed list of the available prescalers for this timer
    const uint16_t PRESCALERS[PRESCALER_COUNT] {0, 1, 8, 64, 256, 1024};

    void setPrescaler(uint16_t scaler);
    
  public:
    TimerCounter() {};

    // Gets the current prescaler of the timer;
    uint16_t prescaler();

    // Gets the current frequency of the timer;
    uint16_t Frequency() {
      return this->frequency;
    };

    // resets the timer to zero.  All registers are set to zero and timer is disabled.
    void resetTimer();

    // Starts the timer using the higher prescaler.
    void startTimer();

    // Triggered by ISR for timer
    void Overflow();
    void Capture();

};
