#include "Arduino.h"
#include "encoders.h"

int8_t Encoders::count1 = 0;
int8_t Encoders::count2 = 0;
uint32_t Encoders::error1 = 0;
uint32_t Encoders::error2 = 0;
int32_t Encoders::distance = 0;
int32_t Encoders::turn = 0;

uint8_t last11, last12, last21, last22;

// Encoder two uses INT2 and INT3 (digital pins 0 and 1)
ISR(INT3_vect,ISR_ALIASOF(INT2_vect));
ISR(INT2_vect)
{
  uint8_t new21 = ((PIND & 0x08) != 0);
  uint8_t new22 = ((PIND & 0x04) != 0);
  
  Encoders::count2 += (last21 ^ new22) - (int)(new21 ^ last22);
  
  if((last21 ^ new21) & (last22 ^ new22))
    Encoders::error1 ++;
    
  last21 = new21;
  last22 = new22;
}

// Encoder one uses PCINT4 and PCINT7 (which trigger the PCINT0 interrupt)
// (digital pins 8 and 11)
ISR(PCINT0_vect)
{
  uint8_t new11 = ((PINB & 0x10) != 0);
  uint8_t new12 = ((PINB & 0x80) != 0);
  
  Encoders::count1 += (last11 ^ new12) - (int)(new11 ^ last12);  
  
  if((last11 ^ new11) & (last12 ^ new12))
    Encoders::error1 ++;
  
  last11 = new11;
  last12 = new12;
}

void Encoders::setup()
{
  cli();
  PCMSK0 = 0x90; // enable pin change interrupts on PCINT4 and 7 which are Arduino pins 8, 11, PB4 and PB7
  PCICR = 0xff; // turns on pin change interrupts in general
  PCIFR = 0; // clear interrupt flags
  
  EICRA = 0x50; // set INT2 and INT3 to interrupt on all edges
  EIMSK = 0x0c; // enable INT2 and INT3
  EIFR = 0; // clear interrupt flags
  sei();
}

void Encoders::update()
{
  cli();
  if(count1 != 0 || count2 != 0)
  {
    distance += count1 + count2;
    turn += count1 - count2;
    count1 = count2 = 0;
  }
  sei();
}

void Encoders::reset()
{
  distance = 0;
  turn = 0;
}
