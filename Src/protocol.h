#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "include.h"

#define PROTOCOL_ARG_LENGTH	50
/**************************************************

协议特别说明
机器上传的信息规定为固定长度 包含一组数据
第一组数据长度为固定长度PROTOCOL_ARG_LENGTH
则可以计算出协议中"数据长度"为10+PROTOCOL_ARG_LENGTH
机器上传的协议一般格式为:

AA 60     (10+PROTOCOL_ARG_LENGTH)    ID ID ID ID     FUNCTION     01  DATA0   DATA1  ...  DATA(PROTOCOL_ARG_LENGTH-1)    CRC

**************************************************/
#define FRAMEHEAD_0	0XAA
#define FRAMEHEAD_1	0X60

#define CALL_ID			0X00
#define GET_STATUS		0X01
#define GET_RELAY_STATYS	0X02
#define GET_WINDOWS_STATUS	0X03
#define GET_TMP_HUM	0X04
#define GET_SYS_ID_IP 0X05 //广播上传设备配置
#define PULL_SYS_ID_IP 0X06 //指定设备上传设备配置


#define SET_RELAY_STATUS	0X12
#define SET_WINDOWS_STATUS	0X13
#define SET_RELAY_TOG	0X14
#define SET_RELAY_ON	0X15
#define SET_RELAY_OFF	0X16
#define SET_WINDOWS_ON	0X17
#define SET_WINDOWS_OFF	0X18
#define SET_WINDOWS_RUN_TIME	0X22
#define SET_RELAY_ZERO		0X23

#define SET_CONFIG			0X19//控制端设置配置信息包括本机ip配置 控制端配置 串口配置
#define REBOOT_MACHINE		0X20// 重启设备

#define LEARN_SWITCH	0X34
#define CLEAR_SWITCH	0X35

#define SYNC_FLASH		0X3F

#define UPLOAD_MACHINE_ID	0XA0
#define UPLOAD_RELAY_STATUS	0XB2
#define UPLOAD_WINDOWS_STATUS	0XB3
#define UPLOAD_TMP_HUN		0XB4
#define UPLOAD_SYS_ID_IP		0XB5
#define PUSH_SYS_ID_IP		0XB6
#define UPLOAD_SYS_CFG		0XB7

#define GET_SWITCH_INFO		0XE2
#define UPLOAD_SWITCH_INFO	0XE3

#define CLEAR_ALL_SWITCH_INFO	0XFA
#define GET_SOFT_VERSION		0XFC

#define DISABLE_MACHINE	0XFD
#define ENABLE_MACHINE		0XFE

typedef struct
{
	unsigned char _0;
	unsigned char _1;
	unsigned char _2;
	unsigned char _3;	
}S_IntToChar;

typedef union
{
	S_IntToChar _char;
	int _int;
}E_IntToChar;


typedef struct
{
	unsigned char where_data_from;
	unsigned short source_data_length;
	unsigned char all_arg_num;
	
	unsigned char machine_ID[4];

	unsigned char founction_code;
	
	unsigned char* next_arg;
	unsigned char next_arg_length;
	unsigned char crc8;
}S_ProtocolFrame,*P_S_ProtocolFrame;

#define UDP_CTRL_SERVER_BUFF_LENGTH	400
#define UDP_CTRL_SERVER_PORT	6963
typedef struct
{
	unsigned char server_buff[UDP_CTRL_SERVER_BUFF_LENGTH];
	S_Socket_Task_Info server;
}S_UDP_Ctrl_Server_Info,*P_S_UDP_Ctrl_Server_Info;
extern S_UDP_Ctrl_Server_Info udp_ctrl_server;

void UploadData(unsigned char function_code,unsigned char* args);
void Net_Ctrl_DealFunction(void* arg);//远程控制协议解析

void InitUDPCtrlServer(void);
void UpLoadMachineStatus(void);
unsigned char ExecuteProtocolFrame(P_S_ProtocolFrame protocol_frame);



#endif

