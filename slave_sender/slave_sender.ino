#include <Wire.h>

#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))

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

void setup()
{
  pinMode(13, OUTPUT);
  
  Wire.begin(20);
  Wire.onRequest(request);
  Wire.onReceive(receive);
  
  digitalWrite(23, LOW);
  digitalWrite(22, HIGH);

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

uint8_t sensor;

void loop()
{
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  sensor = analogRead(A3);
}

uint8_t buf[1];

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void request()
{
  buf[0] = 37;
  Wire.write(buf, 1);
}

void receive(int numBytes)
{
  int16_t speeds[2];
  uint8_t *raw_speeds = (uint8_t *)speeds;
  int i;
  
  for(i=0;i<4;i++)
  {
    if(!Wire.available())
      return;
    raw_speeds[i] = Wire.read();
  }
  digitalWrite(13, HIGH);
  return;
  setMotors(speeds[0],speeds[1]);
}
