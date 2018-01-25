#ifndef __TRANSMIT_H__
#define __TRANSMIT_H__
#include "include.h"
#define TO_RS232_1	1
#define TO_RS232_2	2
#define TO_RS485_1	3
#define TO_RS485_2	4
#define TO_TCM300	5
#define TO_SERVER

#define SOURCE_PORT_RS232_1	1
#define SOURCE_PORT_RS232_2	2
#define SOURCE_PORT_RS485_1	3
#define SOURCE_PORT_RS485_2	4
#define SOURCE_PORT_TCM300	5
#define SOURCE_PORT_SERVER_1	6
#define SOURCE_PORT_SERVER_2	7
#define SOURCE_PORT_CLIENT_1	8
#define SOURCE_PORT_CLIENT_2	9


typedef struct
{
	unsigned char toRS232_1:1;
	unsigned char toRS232_2:1;
	unsigned char toRS485_1:1;
	unsigned char toRS485_2:1;
	unsigned char toTCM300:1;
	unsigned char toTCP_SERVER:1;
	unsigned char toTCP_CLIENT:1;
	unsigned char toUDP:1;
}S_TransmitDirection,*P_S_TransmitDirection;

typedef union
{
	S_TransmitDirection dirction;
	unsigned int dirction_value;
}U_TransmitDirection;



typedef struct
{
	unsigned char SourcePort;
	S_TransmitDirection TargetPort;
}S_DataPort,P_S_DataPort;

void TransimtData(U_TransmitDirection direction,unsigned char* data,unsigned short data_length);

#endif


