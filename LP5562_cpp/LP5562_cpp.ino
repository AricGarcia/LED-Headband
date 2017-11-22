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
  switch (color)
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

/* Send up to two bytes at a time. cmd_reg is register to begin writing to. LP5562 supports 
   auto register increment so if sending 2 bytes it will auto increment to the next 
   reg in line (good for writing to engine command regs). num_bytes ensures that you 
   can send a 0 byte before a non 0 byte. It should only be commanded to be 1 or 2, 
   no more and no less. */
uint8_t LP5562::writeI2C(uint8_t cmd_reg, uint16_t data, uint8_t num_bytes)
{
  TinyWireM.beginTransmission(_address);
  TinyWireM.send(cmd_reg);
  for (uint8_t i = 0; i <= 8; i += 8)
  {
    if (i >= (8 * (2 - num_bytes)))
    {
      uint16_t byte_to_send = data<<i;
      byte_to_send = byte_to_send>>8;
      TinyWireM.send(byte_to_send);
    }
  }
  uint8_t success = TinyWireM.endTransmission();
  return success;
}

/* Read up to 2 bytes. cmd_reg is the register where you want to begin reading from. 
   nbytes is how many bytes you want from it.*/
double LP5563::readI2C(uint8_t cmd_reg, uint8_t num_bytes)
{
  uint16_t received = 0;
  TinyWireM.beginTransmission(_address);
  TinyWireM.send(cmd_reg);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(_address, num_bytes);
  while (TinyWireM.available())
  {
    received = received << 8;
    received += TinyWireM.receive();
  }
}
