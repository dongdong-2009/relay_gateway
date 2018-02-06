#ifndef __UART_RECV_H__
#define __UART_RECV_H__
#include "include.h"

#define RS485_2_COM		huart1
#define RS232_1_COM		huart2
#define TCM300_COM		huart3
#define KEY_BOARD_COM		huart4
#define RS232_2_COM		huart5
#define RS485_1_COM		huart6


#define RS485_2_TIMER		htim1
#define RS232_1_TIMER		htim2
#define TCM300_TIMER		htim3
#define KEY_BOARD_TIMER		htim4
#define RS232_2_TIMER		htim5
#define RS485_1_TIMER		htim6

#define RS485_2_MSG		1
#define RS232_1_MSG		2
#define TCM300_MSG		3
#define KEY_BOARD_MSG		4
#define RS232_2_MSG		5
#define RS485_1_MSG		6


#define GENERAL_RECV_BUFF_SIZE	300
#define GENERAL_SEND_BUFF_SIZE	300


typedef struct
{
	UART_HandleTypeDef* Com;
	TIM_HandleTypeDef* Timer;
	QueueHandle_t Queue;
	unsigned char QueueMsg;
	
	unsigned char* recv_buf;
	unsigned short recv_buf_size;
	unsigned short* dma_cndtr;
	unsigned short* recv_length;
	unsigned char* recv_ok;
}S_Uart_Info,*P_S_Uart_Info;

extern S_Uart_Info Tcm300Info;
extern S_Uart_Info Rs232_1_Info;
extern S_Uart_Info Rs232_2_Info;
extern S_Uart_Info Rs485_1_Info;
extern S_Uart_Info Rs485_2_Info;
extern S_Uart_Info KeyBoardInfo;


typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[GENERAL_RECV_BUFF_SIZE];
	unsigned char tx_data[GENERAL_SEND_BUFF_SIZE];
	
	unsigned char recv_ok;	
	unsigned short point;
	unsigned char enable_rxdata;
	unsigned char vBigRxMax;
	PVoid_Function_PVoid Action;
}S_GeneralSeriaRecvData,*P_S_GeneralSeriaRecvData;
extern S_GeneralSeriaRecvData data_from_RS232_1;
extern S_GeneralSeriaRecvData data_from_RS232_2;
extern S_GeneralSeriaRecvData data_from_RS485_1;
extern S_GeneralSeriaRecvData data_from_RS485_2;
extern S_GeneralSeriaRecvData data_from_KEYBOARD;




void InitSerialBus(void);

void StartRecv(UART_HandleTypeDef* com,unsigned char* recv_buf,unsigned short recv_buf_size);

void UartTimerOpeart(TIM_HandleTypeDef *htim,S_Uart_Info *Info);
void UartIdleOperat(UART_HandleTypeDef *huart,S_Uart_Info *Info) ;

#endif

