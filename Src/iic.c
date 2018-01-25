#include "iic.h"

S_IIC_Interface IIC_SHT20_1;
S_IIC_Interface IIC_SHT20_2;
S_IIC_Interface IIC_SHT20_3;
S_IIC_Interface IIC_24C0X;

#define IIC_SHT20_1_SCL_PORT 	I2C_SCL_1_GPIO_Port
#define IIC_SHT20_1_SDA_PORT 	I2C_SDA_1_GPIO_Port
#define IIC_SHT20_1_SCL_PIN		I2C_SCL_1_Pin
#define IIC_SHT20_1_SDA_PIN		I2C_SDA_1_Pin

#define IIC_SHT20_2_SCL_PORT 	I2C_SCL_2_GPIO_Port
#define IIC_SHT20_2_SDA_PORT 	I2C_SDA_2_GPIO_Port
#define IIC_SHT20_2_SCL_PIN		I2C_SCL_2_Pin
#define IIC_SHT20_2_SDA_PIN		I2C_SDA_2_Pin

#define IIC_SHT20_3_SCL_PORT 	I2C_SCL_4_GPIO_Port
#define IIC_SHT20_3_SDA_PORT 	I2C_SDA_4_GPIO_Port
#define IIC_SHT20_3_SCL_PIN		I2C_SCL_4_Pin
#define IIC_SHT20_3_SDA_PIN		I2C_SDA_4_Pin

#define IIC_24C0X_SCL_PORT 		I2C_SCL_3_GPIO_Port
#define IIC_24C0X_SDA_PORT 		I2C_SDA_3_GPIO_Port
#define IIC_24C0X_SCL_PIN		I2C_SCL_3_Pin
#define IIC_24C0X_SDA_PIN		I2C_SDA_3_Pin

#define SHT20_IIC_DELAY_TIME 	5
#define _24C0X_IIC_DELAY_TIME 	20


void Delay_1_us(void)
{
	unsigned int loopx = 38;
	while(loopx--);
}

void Delay_us(unsigned int n)
{
	unsigned int loopx = n;
	while(loopx --)
		{
			Delay_1_us();
		}
}


void IIC_SHT20_1_SCL0(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_1_SCL_PORT,IIC_SHT20_1_SCL_PIN,GPIO_PIN_RESET);
}

void IIC_SHT20_1_SCL1(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_1_SCL_PORT,IIC_SHT20_1_SCL_PIN,GPIO_PIN_SET);
}


void IIC_SHT20_1_SDA0(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_1_SDA_PORT,IIC_SHT20_1_SDA_PIN,GPIO_PIN_RESET);
}


void IIC_SHT20_1_SDA1(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_1_SDA_PORT,IIC_SHT20_1_SDA_PIN,GPIO_PIN_SET);
}

unsigned char IIC_SHT20_1_ReadSDA(void)
{
	return (unsigned char)HAL_GPIO_ReadPin(IIC_SHT20_1_SDA_PORT, IIC_SHT20_1_SDA_PIN);
}



void IIC_SHT20_2_SCL0(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_2_SCL_PORT,IIC_SHT20_2_SCL_PIN,GPIO_PIN_RESET);
}

void IIC_SHT20_2_SCL1(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_2_SCL_PORT,IIC_SHT20_2_SCL_PIN,GPIO_PIN_SET);
}


void IIC_SHT20_2_SDA0(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_2_SDA_PORT,IIC_SHT20_2_SDA_PIN,GPIO_PIN_RESET);
}


void IIC_SHT20_2_SDA1(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_2_SDA_PORT,IIC_SHT20_2_SDA_PIN,GPIO_PIN_SET);
}

unsigned char IIC_SHT20_2_ReadSDA(void)
{
	return (unsigned char)HAL_GPIO_ReadPin(IIC_SHT20_2_SDA_PORT, IIC_SHT20_2_SDA_PIN);
}


void IIC_SHT20_3_SCL0(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_3_SCL_PORT,IIC_SHT20_3_SCL_PIN,GPIO_PIN_RESET);
}

void IIC_SHT20_3_SCL1(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_3_SCL_PORT,IIC_SHT20_3_SCL_PIN,GPIO_PIN_SET);
}


void IIC_SHT20_3_SDA0(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_3_SDA_PORT,IIC_SHT20_3_SDA_PIN,GPIO_PIN_RESET);
}


void IIC_SHT20_3_SDA1(void)
{
	HAL_GPIO_WritePin(IIC_SHT20_3_SDA_PORT,IIC_SHT20_3_SDA_PIN,GPIO_PIN_SET);
}

unsigned char IIC_SHT20_3_ReadSDA(void)
{
	return (unsigned char)HAL_GPIO_ReadPin(IIC_SHT20_3_SDA_PORT, IIC_SHT20_3_SDA_PIN);
}


void IIC_24C0X_SCL0(void)
{
	HAL_GPIO_WritePin(IIC_24C0X_SCL_PORT,IIC_24C0X_SCL_PIN,GPIO_PIN_RESET);
}

void IIC_24C0X_SCL1(void)
{
	HAL_GPIO_WritePin(IIC_24C0X_SCL_PORT,IIC_24C0X_SCL_PIN,GPIO_PIN_SET);
}


void IIC_24C0X_SDA0(void)
{
	HAL_GPIO_WritePin(IIC_24C0X_SDA_PORT,IIC_24C0X_SDA_PIN,GPIO_PIN_RESET);
}


void IIC_24C0X_SDA1(void)
{
	HAL_GPIO_WritePin(IIC_24C0X_SDA_PORT,IIC_24C0X_SDA_PIN,GPIO_PIN_SET);
}

unsigned char IIC_24C0X_ReadSDA(void)
{
	return (unsigned char)HAL_GPIO_ReadPin(IIC_24C0X_SDA_PORT, IIC_24C0X_SDA_PIN);
}









unsigned char CheckIICInterface(P_S_IIC_Interface iic_interface)
{
	if((iic_interface->IIC_SCL_0)&&(iic_interface->IIC_SCL_1)&&(iic_interface->IIC_SDA_1)&&(iic_interface->IIC_SDA_0)&&(iic_interface->IIC_READ_SDA))
		{
			return 1;
		}
	return 0;
}

void InitSHT20_1Interface(void)
{
	IIC_SHT20_1.IIC_SCL_0 = IIC_SHT20_1_SCL0;
	IIC_SHT20_1.IIC_SCL_1 = IIC_SHT20_1_SCL1;
	IIC_SHT20_1.IIC_SDA_0 = IIC_SHT20_1_SDA0;
	IIC_SHT20_1.IIC_SDA_1 = IIC_SHT20_1_SDA1;
	IIC_SHT20_1.IIC_READ_SDA= IIC_SHT20_1_ReadSDA;
}


void InitSHT20_2Interface(void)
{
	IIC_SHT20_2.IIC_SCL_0 = IIC_SHT20_2_SCL0;
	IIC_SHT20_2.IIC_SCL_1 = IIC_SHT20_2_SCL1;
	IIC_SHT20_2.IIC_SDA_0 = IIC_SHT20_2_SDA0;
	IIC_SHT20_2.IIC_SDA_1 = IIC_SHT20_2_SDA1;
	IIC_SHT20_2.IIC_READ_SDA= IIC_SHT20_2_ReadSDA;
}

void InitSHT20_3Interface(void)
{
	IIC_SHT20_3.IIC_SCL_0 = IIC_SHT20_3_SCL0;
	IIC_SHT20_3.IIC_SCL_1 = IIC_SHT20_3_SCL1;
	IIC_SHT20_3.IIC_SDA_0 = IIC_SHT20_3_SDA0;
	IIC_SHT20_3.IIC_SDA_1 = IIC_SHT20_3_SDA1;
	IIC_SHT20_3.IIC_READ_SDA= IIC_SHT20_3_ReadSDA;
}

void InitIIC_24C0X_1Interface(void)
{
	IIC_24C0X.IIC_SCL_0 = IIC_24C0X_SCL0;
	IIC_24C0X.IIC_SCL_1 = IIC_24C0X_SCL1;
	IIC_24C0X.IIC_SDA_0 = IIC_24C0X_SDA0;
	IIC_24C0X.IIC_SDA_1 = IIC_24C0X_SDA1;
	IIC_24C0X.IIC_READ_SDA= IIC_24C0X_ReadSDA;
}


void StartIIC(P_S_IIC_Interface iic_interface)
{
	iic_interface->IIC_SDA_1();
	iic_interface->IIC_SCL_1();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SDA_0();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_0();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SDA_1();	
}

void StopIIC(P_S_IIC_Interface iic_interface)
{
	iic_interface->IIC_SDA_0();
	iic_interface->IIC_SCL_0();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_1();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SDA_0();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SDA_1();	
}

void IICAck(P_S_IIC_Interface iic_interface)
{
	iic_interface->IIC_SDA_0();
	iic_interface->IIC_SCL_0();	
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_1();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_0();
}

void IICNAck(P_S_IIC_Interface iic_interface)
{
	iic_interface->IIC_SDA_1();
	iic_interface->IIC_SCL_0();	
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_1();
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_0();
}


unsigned char IICWaitAck(P_S_IIC_Interface iic_interface)//返回1表示有回复
{	
	iic_interface->IIC_SCL_0();	
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SDA_1();	
	Delay_us(SHT20_IIC_DELAY_TIME);
	iic_interface->IIC_SCL_1();	
	Delay_us(SHT20_IIC_DELAY_TIME);
	if(iic_interface->IIC_READ_SDA())
		{
			iic_interface->IIC_SCL_0();
			return 0;
		}
	iic_interface->IIC_SCL_0();
	return 1;
}

unsigned char IICWriteByte(P_S_IIC_Interface iic_interface,unsigned char data)
{
	unsigned char loop8 = 0;
	iic_interface->IIC_SCL_0();
	Delay_us(SHT20_IIC_DELAY_TIME);
	for(loop8 = 0; loop8<8 ; loop8++)
		{
			if(0 == (data & 0X80))	
				{iic_interface->IIC_SDA_0();}
			else
				{iic_interface->IIC_SDA_1();}			
			Delay_us(SHT20_IIC_DELAY_TIME);
			iic_interface->IIC_SCL_1();		
			Delay_us(SHT20_IIC_DELAY_TIME);	
			iic_interface->IIC_SCL_0();
			Delay_us(SHT20_IIC_DELAY_TIME);	
			data = (data<<1);
		}	
	iic_interface->IIC_SCL_0();
	return 1;
}

unsigned char IICReadByte(P_S_IIC_Interface iic_interface,unsigned char ack)
{
	unsigned char loop8 = 0;
	unsigned char read_data;	
	iic_interface->IIC_SDA_1();	
	for(loop8 = 0; loop8<8 ; loop8++)
		{
			iic_interface->IIC_SCL_0();		
			Delay_us(SHT20_IIC_DELAY_TIME);
			iic_interface->IIC_SCL_1();		
			Delay_us(SHT20_IIC_DELAY_TIME);	
			read_data = (read_data<<1);
			if(iic_interface->IIC_READ_SDA())
				{
					read_data = read_data|0X01;
				}
			iic_interface->IIC_SCL_0();
			Delay_us(SHT20_IIC_DELAY_TIME);	
		}	
	iic_interface->IIC_SCL_0();
	Delay_us(SHT20_IIC_DELAY_TIME);	
	return read_data;
}

unsigned char SHT2x_CheckCRCSub(unsigned char vData[],unsigned char vDataLong, unsigned char vCheckSum)
{
	unsigned char  vCRC_Data=0X00;
        unsigned char  vLoop;
        unsigned char  vTemp;

        for(vLoop=0X00;vLoop<vDataLong;++vLoop)
        {
        	vCRC_Data^=vData[vLoop];

                for (vTemp=0X08;vTemp>0;--vTemp)
                {
                	if (vCRC_Data&0X80) 
                	{
                		vCRC_Data=(vCRC_Data<<1)^POLYNOMIAL;
                	}
                        else
                        {
                        	vCRC_Data=(vCRC_Data<<1);
                        }
                }
        }

        if(vCRC_Data!=vCheckSum)
        {       return 0X00;                     }
        else
        {       return 0XFF;                     }
}


unsigned char SHT20Reset(P_S_IIC_Interface iic_interface)
{
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, cSOFT_RESET);	
	IICWaitAck(iic_interface);
	StopIIC(iic_interface);
	return 1;
}

unsigned char SetSHT20Mode(P_S_IIC_Interface iic_interface)
{
	unsigned char reg_data;
	unsigned char res = 0;
	StartIIC(iic_interface);	
	IICWriteByte(iic_interface, SHT_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, cUSER_REG_R);
	IICWaitAck(iic_interface);
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_R);
	IICWaitAck(iic_interface);
	reg_data = IICReadByte(iic_interface, ACK);
	IICNAck(iic_interface);
	reg_data = (reg_data&0X7E)|cSHT2x_RES_10_13BIT;	
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, cUSER_REG_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, reg_data);
	IICWaitAck(iic_interface);
	StopIIC(iic_interface);
	//HAL_Delay(5);//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<osdelay
	osDelay(5);
	
	reg_data= 0x55;
	return res;
}

void InitSHT20(P_S_IIC_Interface iic_interface)
{
	SHT20Reset(iic_interface);
	SetSHT20Mode(iic_interface);
}

unsigned int GetTem(P_S_IIC_Interface iic_interface)
{
	unsigned char tmp[3] = {0,0,0};
	unsigned int data;	
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, cTRIG_T_MEASUREMENT_POLL);
	IICWaitAck(iic_interface);

	osDelay(80);
	
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_R);
	IICWaitAck(iic_interface);

	tmp[0]= IICReadByte(iic_interface, ACK);
	IICAck(iic_interface);
	tmp[1]= IICReadByte(iic_interface, ACK);
	IICAck(iic_interface);
	tmp[2]= IICReadByte(iic_interface, ACK);
	IICNAck(iic_interface);
	StopIIC(iic_interface);
	if(SHT2x_CheckCRCSub(tmp,2,tmp[2]))
		{
			data = (unsigned int)tmp[0] <<8;	
			data += (unsigned int)tmp[1];			
		}
	return data;
}

unsigned int GetHum(P_S_IIC_Interface iic_interface)
{
	unsigned char tmp[3] = {0,0,0};
	unsigned int data;	
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, cTRIG_RH_MEASUREMENT_POLL);
	IICWaitAck(iic_interface);

	osDelay(80);
	
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, SHT_ADDR_R);
	IICWaitAck(iic_interface);

	tmp[0]= IICReadByte(iic_interface, ACK);
	IICAck(iic_interface);
	tmp[1]= IICReadByte(iic_interface, ACK);
	IICAck(iic_interface);
	tmp[2]= IICReadByte(iic_interface, ACK);
	IICNAck(iic_interface);
	StopIIC(iic_interface);
	if(SHT2x_CheckCRCSub(tmp,2,tmp[2]))
		{
			data = (unsigned int)tmp[0] <<8;	
			data += (unsigned int)tmp[1];			
		}
	return data;
}

double ChangeTem(unsigned int tmp)
{
	double res = 0.0;
	res = (double)tmp;
	res = ((res/65536)*175.72-46.85);
	return res;
}

double ChangeHum(unsigned int tmp)
{
	double res = 0.0;
	res = (double)tmp;
	res = (res*0.001907) -6.0;
	return res;
}


//////////////////////////////////////////////24C0X
unsigned char Check24C0XBusy(P_S_IIC_Interface iic_interface)//	返回1表示busy
{
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, EEPROM_ADDR_W);	
	Delay_us(SHT20_IIC_DELAY_TIME);
	if(IICWaitAck(iic_interface))
		{
			return 0;
		}
	return 1;
}

void IIC24C0XWriteByte(P_S_IIC_Interface iic_interface,unsigned char address,unsigned char data)
{
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, EEPROM_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, address);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, data);
	IICNAck(iic_interface);
	StopIIC(iic_interface);
	while(Check24C0XBusy(iic_interface));
	StopIIC(iic_interface);
}

void IIC24C0XWrite(P_S_IIC_Interface iic_interface,unsigned char address,unsigned char* data,unsigned short data_length)
{
	unsigned short loopx = 0;
	for(loopx = 0 ; loopx < data_length;loopx ++)	
		{
			IIC24C0XWriteByte(iic_interface,(address+loopx),*(data+loopx));
		}
}



void IIC24C0XRead(P_S_IIC_Interface iic_interface,unsigned char address,unsigned char* data,unsigned short data_length)
{
	unsigned short loopx = data_length;
	unsigned char* tmp_data = data;
	StartIIC(iic_interface);
	IICWriteByte(iic_interface, EEPROM_ADDR_W);
	IICWaitAck(iic_interface);
	IICWriteByte(iic_interface, address);
	IICWaitAck(iic_interface);

	StartIIC(iic_interface);
	IICWriteByte(iic_interface, EEPROM_ADDR_R);
	IICWaitAck(iic_interface);
	while(loopx--)
		{
			*(tmp_data) = IICReadByte(iic_interface,ACK);
			tmp_data++;
			IICAck(iic_interface);
		}
	IICNAck(iic_interface);
	StopIIC(iic_interface);	
}




/////////////////////////////////////////////////////////////////

void InitIICBus(void)//必须在系统启动后调用
{
  InitSHT20_1Interface();
  InitSHT20_2Interface();
  InitSHT20_3Interface();
  InitIIC_24C0X_1Interface();
  InitSHT20(&IIC_SHT20_1);
  InitSHT20(&IIC_SHT20_2);
  InitSHT20(&IIC_SHT20_3);
}








