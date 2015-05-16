#include "Arduino.h"
#include "RpiSlave.h"

void RPiSlave::init(unsigned char address)
{
  TWAR = address << 1;
  ack();
}

ISR(TWI_vect)
{
  if(RPiSlave::handle_event(TWSR, TWDR))
    RPiSlave::nack();
  else
    RPiSlave::ack();
}

void RPiSlave::ack()
{
  TWCR = 
    (1<<TWEN)    // enable TWI
    | (1<<TWIE)  // enable TWI interrupt
    | (1<<TWINT) // clear interrupt flag
    | (1<<TWEA); // ACK
}

void RPiSlave::nack()
{
  TWCR = 
    (1<<TWEN)     // enable TWI
    | (1<<TWIE)   // enable TWI interrupt
    | (1<<TWINT); // clear interrupt flag
}

uint8_t RPiSlave::handle_event(unsigned char event, unsigned char data)
{
  switch(event)
  {
  }
  
  return 0; // default - ACK
}
