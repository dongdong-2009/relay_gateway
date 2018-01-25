#ifndef __RELAY_H__
#define __RELAY_H__
#include "include.h"

#define OPERAT_RELAY(number,status)		HAL_GPIO_WritePin(RL_##number##_GPIO_Port, RL_##number##_Pin, (GPIO_PinState)status)

#define CONNECT_RELAY(number)		HAL_GPIO_WritePin(RL_##number##_GPIO_Port, RL_##number##_Pin, GPIO_PIN_SET)
#define DISCONNECT_RELAY(number)		HAL_GPIO_WritePin(RL_##number##_GPIO_Port, RL_##number##_Pin, GPIO_PIN_RESET)

#define ZERO_INIT_A	EXTI15_10_IRQn
#define ZERO_INIT_B	EXTI15_10_IRQn
#define ZERO_INIT_C	EXTI15_10_IRQn

#define ZERO_PIN_A GPIO_PIN_12
#define ZERO_PIN_B GPIO_PIN_13
#define ZERO_PIN_C GPIO_PIN_14

#define ZERO_MSG_NULL	0
#define ZERO_MSG_A	1
#define ZERO_MSG_B	2
#define ZERO_MSG_C	3

#define WINDOWS_TIMER_MSG		4
#define AUTOUPLOAD_MSG		5

//////继电器在零点处延时闭合时间
/************************
30A继电器闭合动作时间14-15ms
30A继电器断开动作时间18ms

16A继电器闭合动作时间5ms
16A继电器断开动作时间8ms

************************/



#define REALAY_30A_CONNECT_DELAY_TIME		2
#define REALAY_30A_DISCONNECT_DELAY_TIME		5


#define REALAY_16A_CONNECT_DELAY_TIME		5
#define REALAY_16A_DISCONNECT_DELAY_TIME		2


#define RELAY_DEFAULT_CONNECT_DELAY_TIME		6
#define RELAY_DEFAULT_DISCONNECT_DELAY_TIME		12



typedef struct
{
	unsigned char owner:2;//过零检测相位选择
	unsigned char relay_delaytime:6;
	unsigned char relay_num:5;
	unsigned char relay_operat;
}S_Relay_Msg;



typedef struct
{
	unsigned int action_num;//那一bit为1表示需要操作这一bit
	unsigned int status_num;//每一个bit代表相应继电器的状态
}S_RelayOperat,*P_S_RelayOperat;

typedef union
{
	unsigned int int_msg;
	S_Relay_Msg msg;	
}U_Relay_Msg;

typedef enum
{
	realy_connect_cmd = 1,
	relay_disconnect_cmd,
	realy_connect_timeout,
	relay_disconnect_timeout,
	relay_connect_interrupt,
	relay_disconnect_interrupt,
	relay_timeout,
	interrupt_occur,
}E_Relay_Operat;


/*typedef enum
{
	interrupt_occur = 10,
	outtime_occur,
}E_Relay_Event;*/


typedef struct
{
	unsigned char is_relay_busy:1;
	unsigned char zero_phase :2;//过零检测的相位 ABC
	unsigned char relay_operation:1;//继电器动作 1闭合 0断开
	unsigned char timer_num;//为了省一个字节浪费了一中午!!!!
	unsigned char connect_delay_time;
	unsigned char disconnect_delay_time;
}S_Relay_Status,*P_S_Relay_Status;


#define MACHINE_REALY_NUMBER 16
typedef struct
{
	unsigned int relay_status;
	S_RelayOperat realy_opeart;
	S_Relay_Status relay[MACHINE_REALY_NUMBER];
}S_Machine_Relay_Info,*P_S_Machine_Relay_Info;
extern S_Machine_Relay_Info machine_relay_info;


//////////////////////////////////////////////////////////////////////////////////

typedef enum
{	
	windows_halt = 0,
	windows_runing,

	
	windows_opening,
	windows_opend,
	windows_closing,
	windows_closed,
}E_Windows_Status;

typedef struct
{
	unsigned char is_windows_busy;
	unsigned char timer;
	E_Windows_Status status;
	unsigned short windows_protect_time;//正反转切换保护时间
	unsigned int windows_runing_time;
	unsigned int windows_set_run_time;
}S_Windows_Info,*P_S_Windows_Info;

typedef struct
{
	unsigned char windows_relay_status;
	S_Windows_Info windows[2];
}S_Machine_Windows_Info,*P_S_Machine_Windows_Info;
extern S_Machine_Windows_Info machine_windows_info;

void Timer_Send_Msg_To_Queue(unsigned int msg,osMessageQId queue);

//void open_windows(P_S_Windows_Info windows_info);
//void close_windows(P_S_Windows_Info windows_info);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


unsigned char get_bit(unsigned int source,unsigned char target_bit);
void value_set_bit(unsigned int* source,unsigned char target_bit);
void value_clear_bit(unsigned int* source,unsigned char target_bit);


void task_deal_relay(unsigned int msg_code);
void DisConnectRelay(unsigned char relay_num);
void ConnectRelay(unsigned char relay_num);
void TogRelay(unsigned char relay_num);
void init_relay_zero_cfg(void);

void SetRelayStatus(unsigned int status);
unsigned int GetRelayStatus(void);

void deinit_connect_delay_time(void);
void init_windows(void);
void open_windows(unsigned char windows_num);
void close_windows(unsigned char windows_num);

#endif

