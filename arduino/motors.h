/*
To use this motor code you will need to set the following in libraries/Servo/src/avr/ServoTimers.h
Find this code:

#elif defined(__AVR_ATmega32U4__)  
#define _useTimer1 
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;

And change _useTimer1 and _timer1 to _useTimer3 and _timer3.
*/

class Motors
{
  static void setServo(int left, int right);
  
  public:
  static void set(int left, int right);
  static void setup(unsigned char servo_pin);
};
