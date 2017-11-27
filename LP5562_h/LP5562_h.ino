#ifndef LP5562_h
#define LP5562_h

#include Arduino
class LP5562
{
  public:
    // Public LP5562 functions/variables
    LP5562(int address);
    void setDirectPWM(uint8_t pwm, uint8_t color);
    void setLedCtrlMap(uint8_t color, uint8_t ctrl);
    void setPC(); // still needs cpp
    void programEngine(uint8_t eng, uint16_t* program);
    void executeEngine(uint8_t eng);
    void deviceReset();
    void clearInterrupt(); // still needs cpp
    void setCurrent();
    void setPwmHF(uint8_t state);
    void setLogOrLin(); // still needs cpp
    uint16_t rampCMD(uint8_t prescale, uint8_t steptime, uint8_t sign, uint8_t increment);
    uint16_t waitCMD(uint8_t prescale, uint8_t steptime);
    uint16_t setPwmCMD(uint8_t pwm);
    uint16_t goToStartCMD();
    uint16_t branchCMD(uint8_t loop_cnt, uint8_t step_num);
    uint16_t endCMD(uint8_t send_interupt, uint8_t reset);
    uint16_t triggerCMD(uint8_t send_trigger, uint8_t wait_trigger);
  private:
    // I2C functions/variables
    uint8_t _address = 0x30;
    uint8_t _writeI2C(uint8_t cmd_reg, uint16_t data, uint8_t num_bytes);
    uint16_t _readI2C(uint8_t cmd_reg, uint8_t num_bytes);
    void _safeSet2Bits(uint8_t reg, uint8_t bit1, uint8_t bit0, uint8_t bit1_loc, uint8_t bit0_loc);
    
    // Private LP5562 functions/variables
    void _setPowerSave(uint8_t state);

    uint8_t _ENABLE = 0x00;
    uint8_t _OP_MODE = 0x01;
    uint8_t _B_PWM = 0x02;
    uint8_t _G_PWM = 0x03;
    uint8_t _R_PWM = 0x04;
    uint8_t _B_CURRENT = 0x05;
    uint8_t _G_CURRENT = 0x06;
    uint8_t _R_CURRENT = 0x07;
    uint8_t _CONFIG = 0x08;
    uint8_t _ENG1_PC = 0x09;
    uint8_t _ENG2_PC = 0x0A;
    uint8_t _ENG3_PC = 0x0B;
    uint8_t _STATUS = 0x0C;
    uint8_t _RESET = 0x0D;
    uint8_t _W_PWM = 0x0E;
    uint8_t _W_CURRENT = 0x0F;
    uint8_t _LED_MAP = 0x70;
    //ENG1
    uint16_t _ENG1[16] = {0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E};
    //ENG2
    uint16_t _ENG2[16] = {0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E};
    //ENG3
    uint16_t _ENG3[16] = {0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E,
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E};
}

#endif
