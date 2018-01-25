#ifndef __IIC_H__
#define __IIC_H__
#include "include.h"




#define ACK 0
#define NACK 1

#define POLYNOMIAL  0X131

#define SHT_ADDR_W 128
#define SHT_ADDR_R 129

#define EEPROM_ADDR_W 0XA0
#define EEPROM_ADDR_R 0XA1


#define     cPOLYNOMIAL  0X131

typedef enum{
             cTRIG_T_MEASUREMENT_HM=0XE3,       // command trig. Temperature meas. hold master
             cTRIG_RH_MEASUREMENT_HM=0XE5,      // command trig. humidity meas. hold master
             cTRIG_T_MEASUREMENT_POLL=0XF3,     // command trig. Temperature meas. no hold master
             cTRIG_RH_MEASUREMENT_POLL=0XF5,    // command trig. humidity meas. no hold master
             cUSER_REG_W=0XE6,                  // command writing user register
             cUSER_REG_R=0XE7,                  // command reading user register
             cSOFT_RESET=0XFE                   // command soft reset
            }cEtSHT2xCommand;

typedef enum{
             cSHT2x_RES_12_14BIT=0X00,          // RH=12bit, T=14bit
             cSHT2x_RES_8_12BIT=0X01,           // RH= 8bit, T=12bit
             cSHT2x_RES_10_13BIT=0X80,          // RH=10bit, T=13bit
             cSHT2x_RES_11_11BIT=0X81,          // RH=11bit, T=11bit
             cSHT2x_RES_MASK=0X81               // Mask for res. bits (7,0) in user reg.
            }cEtSHT2xResolution;

typedef enum{
             cSHT2x_EOB_ON=0X40,                // end of battery
             cSHT2x_EOB_MASK=0X40               // Mask for EOB bit(6) in user reg.
            }cEtSHT2xEob;

typedef enum{
             cSHT2x_HEATER_ON=0X04,             // heater on
             cSHT2x_HEATER_OFF=0X00,            // heater off
             cSHT2x_HEATER_MASK=0X04            // Mask for Heater bit(2) in user reg.
            }cEtSHT2xHeater;

// measurement signal selection
typedef enum{
             cHUMIDITY,
             cTEMP
            }cEtSHT2xMeasureType;


typedef struct
{
	Void_Function_Void IIC_SCL_0;
	Void_Function_Void IIC_SCL_1;
	Void_Function_Void IIC_SDA_0;
	Void_Function_Void IIC_SDA_1;
	UChar_Function_Void IIC_READ_SDA;


	
	Int_Function_Int Get_temp;
	Int_Function_Int Get_Hum;
}S_IIC_Interface,*P_S_IIC_Interface;

extern S_IIC_Interface IIC_SHT20_1;
extern S_IIC_Interface IIC_SHT20_2;
extern S_IIC_Interface IIC_SHT20_3;
extern S_IIC_Interface IIC_24C0X;
////////////////////////////////////////////////////////////////////////

void InitIICBus(void);

///////////////////////////////////////////////////////////////////////

unsigned char CheckIICInterface(P_S_IIC_Interface iic_interface);


void InitSHT20_1Interface(void);
void InitSHT20_2Interface(void);
void InitSHT20_3Interface(void);
void InitIIC_24C0X_1Interface(void);

void InitSHT20(P_S_IIC_Interface iic_interface);
unsigned int GetHum(P_S_IIC_Interface iic_interface);
unsigned int GetTem(P_S_IIC_Interface iic_interface);
double ChangeTem(unsigned int tmp);
double ChangeHum(unsigned int tmp);


///////////////////////////////////24C0X
void IIC24C0XWrite(P_S_IIC_Interface iic_interface,unsigned char address,unsigned char* data,unsigned short data_length);
void IIC24C0XRead(P_S_IIC_Interface iic_interface,unsigned char address,unsigned char* data,unsigned short data_length);

#endif

