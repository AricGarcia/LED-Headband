#ifndef LP5562_h
#define LP5562_h

#include Arduino
class LP5562
{
  public:
    // Public LP5562 functions/variables
    LP5562(int address);
    void setPWM();
    void setPC();
    void programEngine();
    void startEngine();
    void executeEngine();
    void deviceReset();
  private:
    // I2C functions/variables
    uint8_t _address;
    int dataRate = 8;
    void _startCondition();
    void _sendData(int reg);
    void _endCondition();
    uint8_t _writeI2C(int reg, int data);
    uint8_t _readI2C();
    
    // Private LP5562 functions/variables
    void _enablePowerSave();
    void _disablePowerSave();
    void _setLEDMap(int mode, int color);

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
    uint8_t _PROG_MEM_ENG1_CMD1 = 0x10;
    uint8_t _PROG_MEM_ENG1_CMD2 = 0x12;
    uint8_t _PROG_MEM_ENG1_CMD3 = 0x14;
    uint8_t _PROG_MEM_ENG1_CMD4 = 0x16;
    uint8_t _PROG_MEM_ENG1_CMD5 = 0x18;
    uint8_t _PROG_MEM_ENG1_CMD6 = 0x1A;
    uint8_t _PROG_MEM_ENG1_CMD7 = 0x1C;
    uint8_t _PROG_MEM_ENG1_CMD8 = 0x1E;
    uint8_t _PROG_MEM_ENG1_CMD9 = 0x20;
    uint8_t _PROG_MEM_ENG1_CMD10 = 0x22;
    uint8_t _PROG_MEM_ENG1_CMD11 = 0x24;
    uint8_t _PROG_MEM_ENG1_CMD12 = 0x26;
    uint8_t _PROG_MEM_ENG1_CMD13 = 0x28;
    uint8_t _PROG_MEM_ENG1_CMD14 = 0x2A;
    uint8_t _PROG_MEM_ENG1_CMD15 = 0x2C;
    uint8_t _PROG_MEM_ENG1_CMD16 = 0x2E;
    //ENG2
    uint8_t _PROG_MEM_ENG2_CMD1 = 0x30;
    uint8_t _PROG_MEM_ENG2_CMD2 = 0x32;
    uint8_t _PROG_MEM_ENG2_CMD3 = 0x34;
    uint8_t _PROG_MEM_ENG2_CMD4 = 0x36;
    uint8_t _PROG_MEM_ENG2_CMD5 = 0x38;
    uint8_t _PROG_MEM_ENG2_CMD6 = 0x3A;
    uint8_t _PROG_MEM_ENG2_CMD7 = 0x3C;
    uint8_t _PROG_MEM_ENG2_CMD8 = 0x3E;
    uint8_t _PROG_MEM_ENG2_CMD9 = 0x40;
    uint8_t _PROG_MEM_ENG2_CMD10 = 0x42;
    uint8_t _PROG_MEM_ENG2_CMD11 = 0x44;
    uint8_t _PROG_MEM_ENG2_CMD12 = 0x46;
    uint8_t _PROG_MEM_ENG2_CMD13 = 0x48;
    uint8_t _PROG_MEM_ENG2_CMD14 = 0x4A;
    uint8_t _PROG_MEM_ENG2_CMD15 = 0x4C;
    uint8_t _PROG_MEM_ENG2_CMD16 = 0x4E;
    //ENG3
    uint8_t _PROG_MEM_ENG3_CMD1 = 0x50;
    uint8_t _PROG_MEM_ENG3_CMD2 = 0x52;
    uint8_t _PROG_MEM_ENG3_CMD3 = 0x54;
    uint8_t _PROG_MEM_ENG3_CMD4 = 0x56;
    uint8_t _PROG_MEM_ENG3_CMD5 = 0x58;
    uint8_t _PROG_MEM_ENG3_CMD6 = 0x5A;
    uint8_t _PROG_MEM_ENG3_CMD7 = 0x5C;
    uint8_t _PROG_MEM_ENG3_CMD8 = 0x5E;
    uint8_t _PROG_MEM_ENG3_CMD9 = 0x60;
    uint8_t _PROG_MEM_ENG3_CMD10 = 0x62;
    uint8_t _PROG_MEM_ENG3_CMD11 = 0x64;
    uint8_t _PROG_MEM_ENG3_CMD12 = 0x66;
    uint8_t _PROG_MEM_ENG3_CMD13 = 0x68;
    uint8_t _PROG_MEM_ENG3_CMD14 = 0x6A;
    uint8_t _PROG_MEM_ENG3_CMD15 = 0x6C;
    uint8_t _PROG_MEM_ENG3_CMD16 = 0x6E;
    
}

#endif
