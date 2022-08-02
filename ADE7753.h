//**********************************
//Version 1.0
//**********************************

#include "Arduino.h"
#ifndef ADE7753_h
#define ADE7753_h   

#define CH1OS         0x0D  //08B R/W SM
#define CH2OS         0x0E  //08B R/W SM
#define VRMSOS        0x19  //12B R/W S
#define IRMSOS        0x18  //12B R/W S

#define WRITE 0x80 //0x80 = b10000000

#define MODE          0x09  //16B R/W U
#define LINECYC       0x1C  //16B R/W U
#define GAIN          0x0F  //08B R/W U
#define IRQEN         0x0A  //16B R/W U
#define STATUS        0x0B  //16B R   U
#define RSTSTATUS     0x0C  //16B R   U

#define WAVEFORM      0x01  //24B R   S
#define IRMS          0x16  //24B R   U
#define VRMS          0x17  //24B R   U
#define TEMP          0x26  //08B R   S
#define PERIOD        0x27  //16B R   U

#define LAENERGY      0x04  //24B R   U
#define LVAENERGY     0x07  //24B R   U
#define LVARENERGY    0x08  //24B R   S

//-----------CONSTANTS-----------------
#define CYCEND        0x02  //Indicates CYCLE ACUM END event in STATUS REG
#define ZX            0x10  //Indicates ZX event in STATUS REG
#define WSMP          0x08  //Indicates WAVEFORM event in STATUS REG
#define TEMPRDY       0x20  //Indicates TEMP event in STATUS REG

class ADE7753{
  public:
    ADE7753(int _CS);
    void Start(uint32_t SPI_speed , uint8_t chipSelect_Pin);
    long getWAVE(char phase);
    long getVRMS(int N);
    long getIRMS(int N);
    signed long getTEMP(int N);
    long getPERIOD(int N);
    long getInterruptStatus();
    long getResetInterruptStatus();
    
    unsigned char read8bits(char reg);
    signed char read8bits_s(char reg);
    unsigned int read16bits(char reg);
    signed int read16bits_s(char reg);
    unsigned long read24bits(char reg);
    signed long read24bits_s(char reg);
    signed long read24bits_i(char reg);
    
    void write8(char reg, unsigned char data);
    void write16(char reg, unsigned int data);
    void write16_s(char reg, int data);
    void write24(char reg, unsigned long data);
	
  private:
    int CS;

    void enable();
    void disable();
    void calibrateOffset(char phase);
        
};


#endif
