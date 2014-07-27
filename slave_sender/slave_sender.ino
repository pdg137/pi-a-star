#include <Wire.h>

class TWIInterface
{
  static uint8_t *raw_data;
  static uint8_t raw_data_size;

  public:
  static void setupReceive(uint8_t *data, uint8_t size);
  static void receive(int numBytes);
};

uint8_t *TWIInterface::raw_data;
uint8_t TWIInterface::raw_data_size;

void TWIInterface::setupReceive(uint8_t *data, uint8_t size)
{
  raw_data = data;
  raw_data_size = size;
  Wire.onReceive(TWIInterface::receive);
}
  
void TWIInterface::receive(int numBytes)
{
  uint8_t i;
  
  uint8_t addr = Wire.read();
  
  for(i=0;i<numBytes-1;i++)
  {
    if(addr+i > raw_data_size)
      return;
    raw_data[addr+i] = Wire.read();
  }
}

void setMotors(int left, int right)
{
  if(left < 0)
  {
    digitalWrite(4, LOW);
    OCR1A = min(-left, 400);
  }
  else
  {
    digitalWrite(4, HIGH);
    OCR1A = min(left, 400);
  }
  
  if(right < 0)
  {
    digitalWrite(5, LOW);
    OCR1B = min(-right, 400);
  }
  else
  {
    digitalWrite(5, HIGH);
    OCR1B = min(right, 400);
  }
}

struct
{
  uint8_t motors_modified;
  int16_t motors[2];
} data;

void setup()
{
  pinMode(13, OUTPUT);
  Wire.begin(20);
  
  TWIInterface::setupReceive((uint8_t *)(&data), sizeof(data));
  
  // 20kHz PWM copied from Zumo shield library
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  setMotors(0,0);
}

void loop()
{
//  if(bytesAvailable())
//    readByte();

  
  setMotors(data.motors[0],data.motors[1]);
}

