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


/* Set PWM direct input from AVR from 0-255 duty cycle. Color is as follows:
   blue:  0
   green: 1
   red:   2
   white: 3
   */
void LP5562::SetDirectPwm(uint8_t pwm, uint8_t color)
{
  int current_state = readI2C(_LED_MAP, 1);
  uint8_t bit0_loc;
  uint8_t bit1_loc;
  switch (color)
  {
    case 0:
      bit0_loc = 0;
      bit1_loc = 1;
      writeI2C(_B_PWM, pwm, 1);
      break;
    case 1:
      bit0_loc = 2;
      bit1_loc = 3;
      writeI2C(_G_PWM, pwm, 1);
      break;
    case 2:
      bit0_loc = 4;
      bit1_loc = 5;
      writeI2C(_R_PWM, pwm, 1);
      break;
    case 3:
      bit0_loc = 6;
      bit1_loc = 7;
      writeI2C(_W_PWM, pwm, 1);
      break;
    default:
      return 0;
  }
  current_state ^= (-0 ^ current_state) & (1UL << bit0_loc);
  current_state ^= (-0 ^ current_state) & (1UL << bit1_loc);
  writeI2C(_LED_MAP, current_state, 1);
}

void LP5562::programEngine()
{
  disablePowerSave();
  
  disablePowerSave();
}


void LP5562:executeEngine()
{
  
}


void setPC()
{
  
}


uint16_t LP5562::rampCMD(uint8_t prescale, uint8_t steptime, uint8_t sign, uint8_t increment)
{
  uint16_t command = 0;
  command |= prescale << 14;
  command |= steptime << 8;
  command |= sign << 7;
  command |= increment;
  return command;
}


int LP5562::waitCMD(uint8_t prescale, uint8_t steptime)
{
  uint16_t command = 0;
  command |= prescale << 14;
  command |= steptime << 8;
  return command;
}


int LP5562::setPwmCMD(uint8_t pwm)
{
  uint16_t command = 0x4000;
  command |= pwm;
  return command;
}


int LP5562::goToStartCMD()
{
  uint16_t command = 0;
  return command;
}


int LP5562::branchCMD(uint8_t loop_cnt, uint8_t step_num)
{
  uint16_t command = 0xA000;
  command |= loop_cnt << 7;
  command |= step_num;
  return command;
}


int LP5562::endCMD(uint8_t send_interupt, uint8_t reset)
{
  uint16_t command = 0xC000;
  command |= send_interupt << 12;
  command |= reset << 11;
  return command;
}


int LP5562::triggerCMD(uint8_t send_trigger, uint8_t wait_trigger)
{
  uint16_t command = 0xE000;
  command |= send_trigger << 1;
  command |= wait_trigger << 7;
  return command;
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
uint16_t LP5563::readI2C(uint8_t cmd_reg, uint8_t num_bytes)
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
