#include "Arduino.h"
#include "buttons.h"

uint8_t Buttons::button0;
uint8_t Buttons::button1;
uint8_t Buttons::button2;
  
void Buttons::update()
{ 
  button0 = (digitalRead(18) ? 0 : 1);
  button1 = (digitalRead(15) ? 0 : 1);
  button2 = (digitalRead(14) ? 0 : 1);
}
