#include "Arduino.h"
#include "encoders.h"

int8_t Encoders::count1 = 0;
int8_t Encoders::count2 = 0;
uint32_t Encoders::error1 = 0;
uint32_t Encoders::error2 = 0;

uint8_t last11, last12, last21, last22;

// Encoder 1 uses INT0 and INT1
ISR(INT1_vect,ISR_ALIASOF(INT0_vect));
ISR(INT0_vect)
{
  uint8_t new11 = ((PIND & 0x01) != 0);
  uint8_t new12 = ((PIND & 0x02) != 0);
  
  Encoders::count1 += (last11 ^ new12) - (int)(new11 ^ last12);
  
  if((last11 ^ new11) & (last12 ^ new12))
    Encoders::error1 ++;
    
  last11 = new11;
  last12 = new12;
}

// Encoder two uses PCINT4 and PCINT7 (which trigger the PCINT0 interrupt)  
ISR(PCINT0_vect)
{
  uint8_t new21 = ((PINB & 0x10) != 0);
  uint8_t new22 = ((PINB & 0x80) != 0);
  
  Encoders::count2 += (last21 ^ new22) - (int)(new21 ^ last22);  
  
  if((last21 ^ new21) & (last22 ^ new22))
    Encoders::error2 ++;
  
  last21 = new21;
  last22 = new22;
}

void Encoders::setup()
{
  cli();
  PCMSK0 = 0x90; // enable pin change interrupts on PCINT4 and 7 which are Arduino pins 8, 11, PB4 and PB7
  PCICR = 0xff; // turns on pin change interrupts in general
  PCIFR = 0; // clear interrupt flags
  
  EICRA = 0x05; // set INT0 and INT to interrupt on all edges
  EIMSK = 0x03; // enable INT0 and INT1
  EIFR = 0; // clear interrupt flags
  sei();
}
