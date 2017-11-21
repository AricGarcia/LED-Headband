#include <TinyWireM.h>
#include <USI_TWI_Master.h>

#include "Arduino.h"
#include "LP5562.h"

LP5562::LP5562(int address)
{
  _address = address;
  TinyWireM.begin();
  wait(1);
  _writeI2C(int reg = 0x00, int data = 0x40);
  wait(1);
  _writeI2C(int reg = 0x08, int data = 0x21);
}

void LP5562::SetDirectPwm()
{
  
  int bit0_loc;
    int bit1_loc;
    switch(color)
    {
        case 0:
            bit0_loc = 0;
            bit1_loc = 1;
            break;
        case 1:
            bit0_loc = 2;
            bit1_loc = 3;
            break;
        case 2:
            bit0_loc = 4;
            bit1_loc = 5;
            break;
        case 3:
            bit0_loc = 6;
            bit1_loc = 7;
            break;
        default:
            return 0;
    }
}


uint8_t LP5562::writeI2C(uint16_t data)
{
  TinyWireM.beginTransmission(_address);
  TinyWireM.send(data);
  uint8_t success = TinyWireM.endTransmission();
  return success;
}

double LP5563::readI2C(uint8_t read_start, uint8_t nbytes)
{
  TinyWireM.beginTransmission(_address);
  TinyWireM.send(read_start);
  T
  TinyWireM.requestFrom(_address, nbytes);
}
