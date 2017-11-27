#include <TinyWireM.h>
#include <USI_TWI_Master.h>

#include "Arduino.h"
#include "LP5562.h"


/*
 * Initiate the LP5562. Address options are 0 1 2 or 3. 
 * This value is dependant on how the addr pins are 
 * configured when wiring the chip.
*/
LP5562::LP5562(int address)
{
  _address |= address;
  TinyWireM.begin();
  wait(1);
  writeI2C(_ENABLE, 0x40, 1);
  wait(1);
  writeI2C(_CONFIG, 0x21, 1);
}


/* Set PWM direct input from AVR from 0-255 duty cycle. Color is as follows:
   blue:  0
   green: 1
   red:   2
   white: 3
   */
void LP5562::SetDirectPwm(uint8_t pwm, uint8_t color)
{
  switch (color)
  {
    case 0:
      writeI2C(_B_PWM, pwm, 1);
      break;
    case 1:
      writeI2C(_G_PWM, pwm, 1);
      break;
    case 2:
      writeI2C(_R_PWM, pwm, 1);
      break;
    case 3:
      writeI2C(_W_PWM, pwm, 1);
      break;
    default:
      return 0;
  }
  
}


/*  
 *   programEngine takes an input of eng from [0:2] which corresponds to 
 *  [ENG3:ENG1]. program is defined as the pointer of the first command 
 *  to be programmed to the engine.
*/
void LP5562::programEngine(uint8_t eng, uint16_t* program)
{
  // Set const values for loading to appropriate engine
  switch (eng)
  {
    case 0:
      uint16_t* engine = _ENG1;
      uint8_t bit0_loc = 4;
      uint8_t bit1_loc = 5;
      break;
    case 1:
      uint16_t* engine = _ENG2;
      uint8_t bit0_loc = 2;
      uint8_t bit1_loc = 3;
      break;
    case 2:
      uint16_t* engine = _ENG3;
      uint8_t bit0_loc = 0;
      uint8_t bit1_loc = 1;
      break;
    default:
      return 0;
  }
  // Disable power save mode
  setPowerSave(0);
  // Find current state of mode register
  uint16_t current_state = readI2C(_OP_MODE, 1);
  // Enforce load program mode to mode register
  current_state ^= (-1 ^ current_state) & (1UL << bit0_loc);
  current_state ^= (-0 ^ current_state) & (1UL << bit1_loc);
  writeI2C(_OP_MODE, current_state, 1);
  // Write program
  for(uint8_t i=0; i<=15; i++)
  {
    writeI2C(engine[i], *program, 2);
    program++;
  }
  // Enforce disabled mode to mode register
  current_state ^= (-0 ^ current_state) & (1UL << bit0_loc);
  current_state ^= (-0 ^ current_state) & (1UL << bit1_loc);
  writeI2C(_OP_MODE, current_state, 1);
  // Renable power save mode
  setPowerSave(1);
}


void LP5562:executeEngine(uint8_t eng)
{
  switch (eng)
  {
    case 0:
      uint8_t bit0_loc = 4;
      uint8_t bit1_loc = 5;
      break;
    case 1:
      uint8_t bit0_loc = 2;
      uint8_t bit1_loc = 3;
      break;
    case 2:
      uint8_t bit0_loc = 0;
      uint8_t bit1_loc = 1;
      break;
    default:
      return 0;
  }
  // Set mode to run
  safeSet2Bits(_OP_MODE, 1, 0, bit1_loc, bit0_loc);
  // Set enable to run
  safeSet2Bits(_ENABLE, 1, 0, bit1_loc, bit0_loc);
}


/* Set control method for each color:
 *  Color:        Metod:
 *    blue:  0      I2C:  0
 *    green: 1      ENG1: 1
 *    red:   2      ENG2: 2
 *    white: 3      ENG3: 3
 */
void LP5562::setLedCtrlMap(uint8_t color, uint8_t ctrl)
{
  uint8_t bit0_loc;
  uint8_t bit1_loc;
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
  safeSet2Bits(_LED_MAP, (ctrl>>1)&1, ctrl&1, bit1_loc, bit0_loc)
}


/*
 * 
*/
void LP5562::setPC()
{
  
}


/*
 * Write all FF to reset register to reset device to defaults.
*/
void LP5562::deviceReset()
{
  writeI2C(_RESET, 0xFF, 1);
}


/*
 * Clears an engine interrupt by reading the status register.
*/
void LP5562::clearInterrupt()
{
  readI2C(_STATUS, 1);
}


/*
 * Set current between 0-255 translates to 0.0mA-25.5mA in increments of 0.1mA
 * Color:
 *  blue:  0
 *  green: 1
 *  red:   2
 *  white: 3
*/
void LP5562::setCurrent(uint8_t color, uint16_t current)
{
  switch (color)
  {
    case 0:
      writeI2C(_B_CURRENT, current, 1);
    case 1:
      writeI2C(_G_CURRENT, current, 1);
    case 2:
      writeI2C(_R_CURRENT, current, 1);
    case 3:
      writeI2C(_W_CURRENT, current, 1);
    default:
      return 0;
  }
}


/*
 * Set PWM Switching Frequency. 0 is 256Hz and 1 is 558Hz.
*/
void LP5562::setPwmHF(uint8_t state)
{
  uint16_t config_state = readI2C(_CONFIG, 1);
  config_state ^= (-state ^ config_state) & (1UL << 6);
  writeI2C(_CONFIG, config_state, 1);
}


/*
 * 
*/
void LP5562::setLogOrLin()
{
  
}


/*
 * Set power save on or off. Set state to 1 for on and 0 for off.
*/
void LP5562::setPowerSave(uint8_t state)
{
  uint16_t config_state = readI2C(_CONFIG, 1);
  config_state ^= (-state ^ config_state) & (1UL << 5);
  writeI2C(_CONFIG, config_state, 1);
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


/*
 * prescale: 0 for 0.49ms cycle time, 1 for 15.2ms cycle time
 * steptime: 
*/
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


/* 
 *  Send up to two bytes at a time. cmd_reg is register to begin writing to. LP5562 supports 
 *  auto register increment so if sending 2 bytes it will auto increment to the next 
 *  reg in line (good for writing to engine command regs). num_bytes ensures that you 
 *  can send a 0 byte before a non 0 byte. It should only be commanded to be 1 or 2, 
 *  no more and no less. 
 */
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
 *  nbytes is how many bytes you want from it.
 */
uint16_t LP5562::readI2C(uint8_t cmd_reg, uint8_t num_bytes)
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


void LP5562::safeSet2Bits(uint16_t reg, uint8_t bit1, uint8_t bit0, uint8_t bit1_loc, uint8_t bit0_loc)
{
  uint16_t current_state = readI2C(reg, 1);
  current_state ^= (-bit0 ^ current_state) & (1UL << bit0_loc);
  current_state ^= (-bit1 ^ current_state) & (1UL << bit1_loc);
  writeI2C(reg, current_state, 1);
}

/*

#include <iostream>

using namespace std;

uint16_t
rampCMD (uint8_t prescale, uint8_t steptime, uint8_t sign, uint8_t increment)
{
  uint16_t command = 0;
  command |= prescale << 14;
  command |= steptime << 8;
  command |= sign << 7;
  command |= increment;
  return command;
}


uint16_t
waitCMD (uint8_t prescale, uint8_t steptime)
{
  uint16_t command = 0;
  command |= prescale << 14;
  command |= steptime << 8;
  return command;
}


uint16_t
setPwmCMD (uint8_t pwm)
{
  uint16_t command = 0x4000;
  command |= pwm;
  return command;
}


void
run_program (uint16_t * prog_start)
{
  for (int i = 0; i <= 15; i++)
    {
      cout << *prog_start << endl;
      prog_start++;
    }
}


int
main ()
{
  uint16_t program[16] = {
    rampCMD (1, 42, 1, 85),
    waitCMD (1, 42),
    setPwmCMD (160),
    rampCMD (1, 42, 0, 85),
    waitCMD (0, 42),
    waitCMD (1, 42),
    setPwmCMD (160),
    rampCMD (1, 42, 0, 85),
    waitCMD (1, 42),
    setPwmCMD (160),
    rampCMD (1, 42, 0, 85),
    waitCMD (1, 42),
    setPwmCMD (160)
  };

  uint16_t *apple = program;
  run_program (&apple[0]);
}

*/
