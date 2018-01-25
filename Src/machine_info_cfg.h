#ifndef __MACHINE_INFO_CFG__
#define __MACHINE_INFO_CFG__
#include "include.h"


extern unsigned char AutoUpLoadTimer;

#define BAUD_RATE_4800 0
#define BAUD_RATE_9600 1
#define BAUD_RATE_19200 2
#define BAUD_RATE_57600 3
#define BAUD_RATE_115200 4

#define TCP_SERVER_PROTOCOL	1
#define TCP_CLIENT_PROTOCOL	2
#define UDP_PROTOCOL	3
#define SERIAL_PROTOCOL	4

#define CFG_EFFECT	0X48
#define CFG_INVALID	0X45

#define P_9600	0
#define P_19200	1
#define P_57600	2
#define P_115200	3


typedef struct
{
	unsigned char ip_3;
	unsigned char ip_2;
	unsigned char ip_1;
	unsigned char ip_0;
}S_IPArray;
typedef union
{
	S_IPArray ip_array;
	unsigned int ip_int;
}U_IP;


typedef struct
{
	unsigned char learn_flag:1;
	unsigned char clear_flag:1;
}S_Learn_Clear_Flags,*P_S_Learn_Clear_Flags;




typedef struct
{
	//unsigned char IP[4];
	//unsigned char NetMask[4];
	//unsigned char GateWay[4];
	//unsigned char DNS[4];

	unsigned int IP;
	unsigned int NetMask;
	unsigned int GateWay;
	unsigned int DNS;
	
}S_IPConfig_Info,*P_S_IPConfig_Info;

typedef struct
{
	unsigned int BaudRate;
}S_Uart_Config_Info,*P_S_Uart_Config_Info;


typedef struct
{
	unsigned short auto_upload_time;
	unsigned int windows_run_time;
	
	S_IPConfig_Info IPAddress;
	S_Uart_Config_Info RS485_2_COM_cfg;
	S_Uart_Config_Info RS232_1_COM_cfg;
	S_Uart_Config_Info TCM300_COM_cfg;
	S_Uart_Config_Info KEY_BOARD_COM_cfg;
	S_Uart_Config_Info RS232_2_COM_cfg;
	S_Uart_Config_Info RS485_1_COM_cfg;
	
	unsigned char relay_zero_cfg[MACHINE_REALY_NUMBER];
	
}S_SystemConfig_Info,*P_S_SystemConfig_Info;
extern S_SystemConfig_Info SysConfig;



typedef struct
{
	unsigned char temperature;// 数值减去100 为当前温度值
	unsigned char humidity;
}S_Hum_Temp,*P_S_Hum_Temp;




#define CTRL_SIDE_BUFF_SIZE		500

typedef void (*void_Function_PUC_Int) (unsigned char* data,unsigned int size);

typedef struct
{
	unsigned char* ctrl_data;
	unsigned short ctrl_data_size;
	unsigned char ctrl_effect;//控制端是否有效
	void* socket_info;

	void_Function_PUC_Int send_to_ctrl_side;

	

	unsigned char Ctrl_Protocol;
	unsigned int Ctrl_IP;
	unsigned short Ctrl_Port;
	unsigned short Device_Port;//本机端口
}S_Ctrl_Info,*P_S_Ctrl_Info;


typedef struct
{

	unsigned char* Device_Data;
	unsigned short Data_Size;


	
	unsigned int Transmit;
	unsigned char Ctrl_Protocol;
	unsigned int Net_IP;
	unsigned short Ner_Port;
	unsigned short Device_Port;
	unsigned char Device_Name[30];
}S_Net_Device,*P_S_Net_Device;









typedef struct
{
	S_Learn_Clear_Flags learn_clear;
	S_Hum_Temp Temp_Hum[3];
	S_SystemConfig_Info SysConfig;
	S_Ctrl_Info Ctrl_Side[4];
	S_Net_Device Net_Device[6];
}S_Machine_Info_Cfg,*P_S_Machine_Info_Cfg;
extern S_Machine_Info_Cfg MachineInfoCfg;




unsigned int IPStrToInt(const char *ip) ;
char *IntToStr(const int ip, char *buf);


void Init_SysConfig(void);



void SaveMachineConfig(unsigned char* recv_data);
void UpLoadSys_Cfg(void);


unsigned char Is_Ctrl_Side_Effect(void);
void Init_Ctrl_Server(void);

#endif

