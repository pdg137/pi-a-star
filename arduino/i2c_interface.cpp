#include "Arduino.h"
#include "i2c_interface.h"
#include "Wire.h"

uint8_t *I2cInterface::raw_data;
uint8_t I2cInterface::raw_data_size;

uint8_t *I2cInterface::raw_report;
uint8_t I2cInterface::raw_report_size;

void I2cInterface::setup(uint8_t address, uint8_t *data, uint8_t data_size,
    uint8_t * report, uint8_t report_size)
{
  Wire.begin(address);
  raw_data = data;
  raw_data_size = data_size;
  raw_report = report;
  raw_report_size = report_size;
  Wire.onReceive(I2cInterface::receive);
  Wire.onRequest(I2cInterface::request);
}
  
void I2cInterface::receive(int numBytes)
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

void I2cInterface::request()
{
  Wire.write(raw_report, raw_report_size);
}
