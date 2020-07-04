### Freqeucy Counter

Experiment in frequency counting, using the Input Capute unit of Timer1.

Timer1 is 16 bit counter timer, giving reasonable resolution in capturing.
To increase the range over the fixed resoultion, the Timer uses Prescalres,
Clock divders, extending the range of the clock at the expense of accuracy.

The Counter uses just 2 of the available Prescalers, 1 and 256.
Both can "meet" at a common frequency of around 245hz which is used as a switching point.
Frequencies sampled below this point are sampled with a prescaler of 256, allowing samples to drop
lower that 1hz.
Freqeuncies above this point use the "raw" clock speed or prescaler of one.
This samples within in a shorter period, but with higher accuracy.


As each pulse arrives, it triggers the input capture interrupt.
Input capture is set up to trigger on rising edge, having the inut level set high intiially,
this indicates the end of the cycle period.

The count value of the Timer is multipled by the current prescaler to get the "actual" click count
of that pulse.

Each pulse will update the current frequency and reset the timer back to zero.

OVerflow occurs when a pulse does not arrive within a cycle of the Timer.
The Frequency is set to zero and the prescaler adjusted to the low frequency level (256)

The pin sampled is fixed on Digital pin 8, as fixed to the Timer.  


## Usage
The Counter is used via a single, static instance, `InputCounter`  
Simply start the counter in the `setup()` method:  
`InputCounter.startTimer()`  

In the `loop()` the current freqeucny can be read with:  
`InputCounter.Frequency()`  

