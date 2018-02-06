#include "include.h"
#include "protocol.h"


extern osMessageQId KeyQueueHandle;
S_Machine_Relay_Info machine_relay_info;
S_Machine_Windows_Info machine_windows_info;




void set_relay_connect_delay_time(unsigned char relay_num,unsigned char delay_time)
{
	machine_relay_info.relay[relay_num].connect_delay_time = delay_time;
}

void set_relay_disconnect_delay_time(unsigned char relay_num,unsigned char delay_time)
{
	machine_relay_info.relay[relay_num].disconnect_delay_time = delay_time;
}


void deinit_connect_delay_time(void)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < 8; loopx++)
		{
			set_relay_connect_delay_time(loopx,REALAY_30A_CONNECT_DELAY_TIME);
			set_relay_disconnect_delay_time(loopx,REALAY_30A_DISCONNECT_DELAY_TIME);
		}
	for(loopx = 8 ; loopx < 16; loopx++)
		{
			set_relay_connect_delay_time(loopx,REALAY_16A_CONNECT_DELAY_TIME);
			set_relay_disconnect_delay_time(loopx,REALAY_16A_DISCONNECT_DELAY_TIME);
		}
}


/*****************************************
A相	EXTI15-10
B相	EXTI15-10
C相	EXTI15-10

*****************************************/

unsigned int convert_to_exti_msg(unsigned int value)
{
	unsigned int data = 0;
	data = value&0XFFFFFF;
	return (data|0X2000000);
}

void exti_send_msg_to_queue(unsigned int msg,osMessageQId queue)
{	
	unsigned int msg_code;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	msg_code = convert_to_exti_msg(msg);
	xQueueSendFromISR(queue,&msg_code,&xHigherPriorityTaskWoken);
}

void Timer_Send_Msg_To_Queue(unsigned int msg,osMessageQId queue)
{	
	unsigned int msg_code = msg;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	xQueueSendFromISR(queue,&msg_code,&xHigherPriorityTaskWoken);
}	



unsigned char get_operat_from_msg(unsigned int int_msg)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	return msg.msg.relay_operat;
}
unsigned char get_owner_from_msg(unsigned int int_msg)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	return msg.msg.owner;
}
unsigned char get_relay_num_from_msg(unsigned int int_msg)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	return msg.msg.relay_num;
}
unsigned char get_relay_delaytime_from_msg(unsigned int int_msg)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	return msg.msg.relay_delaytime;
}

unsigned int set_operat_into_msg(unsigned int int_msg,unsigned char msg_operat)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	msg.msg.relay_operat = msg_operat;
	return msg.int_msg;
}
unsigned char set_owner_into_msg(unsigned int int_msg,unsigned char msg_ownner)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	msg.msg.owner= msg_ownner;
	return msg.int_msg;
}
unsigned char set_relaynum_into_msg(unsigned int int_msg,unsigned char msg_relaynum)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	msg.msg.relay_num= msg_relaynum;
	return msg.int_msg;
}
unsigned char set_relay_delaytime_into_msg(unsigned int int_msg,unsigned char msg_delaytime)
{
	U_Relay_Msg msg;
	msg.int_msg = int_msg;
	msg.msg.relay_num= msg_delaytime;
	return msg.int_msg;
}

unsigned char get_bit(unsigned int source,unsigned char target_bit)
{
	if(target_bit > 31)
		{
			return 2;
		}
	if(source&(1<<target_bit))
		{
			return 1;
		}
	return 0;
}

void value_set_bit(unsigned int* source,unsigned char target_bit)
{	
	if(target_bit > 31)
		{
			return;
		}
	*source |= (1<<target_bit);
}

void value_clear_bit(unsigned int* source,unsigned char target_bit)
{	
	if(target_bit > 31)
		{
			return;
		}
	*source &= ~(1<<target_bit);
}


void ConnectRelay(unsigned char relay_num)
{
	if(relay_num > (MACHINE_REALY_NUMBER-1))
		{
			return;
		}
	if(machine_relay_info.relay[relay_num].is_relay_busy)
		{
			return;
		}
	machine_relay_info.relay[relay_num].is_relay_busy = 1;
	machine_relay_info.relay[relay_num].relay_operation = 1;
	value_set_bit(&(machine_relay_info.realy_opeart.action_num),relay_num);
	value_set_bit(&(machine_relay_info.realy_opeart.status_num),relay_num);
	machine_relay_info.relay[relay_num].timer_num = register_value_into_timer(200,0);//创建一个200ms的单次软件定时器
	register_queue_into_timer(machine_relay_info.relay[relay_num].timer_num ,RelayQueueHandle);//200ms后向队列RelayQueueHandle发送数据
	switch(machine_relay_info.relay[relay_num].zero_phase)//开启中断
		{
			case 0:
				
				break;
			case 1:				
				HAL_NVIC_EnableIRQ(ZERO_INIT_A);	
				break;
			case 2:
				HAL_NVIC_EnableIRQ(ZERO_INIT_B);					
				break;
			case 3:
				HAL_NVIC_EnableIRQ(ZERO_INIT_C);					
				break;			
		}
}


void DisConnectRelay(unsigned char relay_num)
{
	if(relay_num > (MACHINE_REALY_NUMBER-1))
		{
			return;
		}	
	if(machine_relay_info.relay[relay_num].is_relay_busy)
		{
			return;
		}
	machine_relay_info.relay[relay_num].is_relay_busy = 1;
	machine_relay_info.relay[relay_num].relay_operation = 0;
	value_set_bit(&(machine_relay_info.realy_opeart.action_num),relay_num);
	value_clear_bit(&(machine_relay_info.realy_opeart.status_num),relay_num);
	machine_relay_info.relay[relay_num].timer_num = register_value_into_timer(200,0);//创建一个200ms的单次软件定时器
	register_queue_into_timer(machine_relay_info.relay[relay_num].timer_num ,RelayQueueHandle);//200ms后向队列RelayQueueHandle发送数据发送的数据时软件定时器编号
	switch(machine_relay_info.relay[relay_num].zero_phase)//开启中断
		{
			case 0:
				
				break;
			case 1:				
				HAL_NVIC_EnableIRQ(ZERO_INIT_A);	
				break;
			case 2:
				HAL_NVIC_EnableIRQ(ZERO_INIT_B);					
				break;
			case 3:
				HAL_NVIC_EnableIRQ(ZERO_INIT_C);					
				break;			
		}
}

void TogRelay(unsigned char relay_num)
{
	if(get_bit(machine_relay_info.relay_status, relay_num))
		{
			DisConnectRelay(relay_num);
		}
	else
		{
			ConnectRelay(relay_num);
		}
}

void SetRelayStatus(unsigned int status)
{
	unsigned char loopx = 0;
	for(;loopx<MACHINE_REALY_NUMBER; loopx++)
		{
			if(get_bit(status, loopx))
				{
					ConnectRelay(loopx);
				}
			else
				{
					DisConnectRelay(loopx);
				}
		}
}

unsigned int GetRelayStatus(void)
{
	return machine_relay_info.relay_status;
}

void physical_connect_relay(unsigned char relay_num)
{
	value_set_bit(&(machine_relay_info.relay_status),relay_num);
	switch(relay_num)
		{
			case 0:
				CONNECT_RELAY(1);
				break;				
			case 1:
				CONNECT_RELAY(2);
				break;				
			case 2:
				CONNECT_RELAY(3);
				break;				
			case 3:
				CONNECT_RELAY(4);
				break;				
			case 4:
				CONNECT_RELAY(5);
				break;				
			case 5:
				CONNECT_RELAY(6);
				break;				
			case 6:
				CONNECT_RELAY(7);
				break;				
			case 7:
				CONNECT_RELAY(8);
				break;				
			case 8:
				CONNECT_RELAY(9);
				break;				
			case 9:
				CONNECT_RELAY(10);
				break;
			case 10:
				CONNECT_RELAY(11);
				break;
			case 11:
				CONNECT_RELAY(12);
				break;
			case 12:
				CONNECT_RELAY(13);
				break;
			case 13:
				CONNECT_RELAY(14);
				break;
			case 14:
				CONNECT_RELAY(15);
				break;
			case 15:
				CONNECT_RELAY(16);
				break;				
			case 16:
				CONNECT_RELAY(17);
				break;				
			case 17:
				CONNECT_RELAY(18);
				break;
			case 18:
				CONNECT_RELAY(19);
				break;
			case 19:
				CONNECT_RELAY(20);
				break;		
			default:
				break;
		}
}



void physical_disconnect_relay(unsigned char relay_num)
{
	value_clear_bit(&(machine_relay_info.relay_status),relay_num);
	switch(relay_num)
		{
			case 0:
				DISCONNECT_RELAY(1);
				break;				
			case 1:
				DISCONNECT_RELAY(2);
				break;				
			case 2:
				DISCONNECT_RELAY(3);
				break;				
			case 3:
				DISCONNECT_RELAY(4);
				break;				
			case 4:
				DISCONNECT_RELAY(5);
				break;				
			case 5:
				DISCONNECT_RELAY(6);
				break;				
			case 6:
				DISCONNECT_RELAY(7);
				break;				
			case 7:
				DISCONNECT_RELAY(8);
				break;				
			case 8:
				DISCONNECT_RELAY(9);
				break;				
			case 9:
				DISCONNECT_RELAY(10);
				break;
			case 10:
				DISCONNECT_RELAY(11);
				break;
			case 11:
				DISCONNECT_RELAY(12);
				break;
			case 12:
				DISCONNECT_RELAY(13);
				break;
			case 13:
				DISCONNECT_RELAY(14);
				break;
			case 14:
				DISCONNECT_RELAY(15);
				break;
			case 15:
				DISCONNECT_RELAY(16);
				break;				
			case 16:
				DISCONNECT_RELAY(17);
				break;				
			case 17:
				DISCONNECT_RELAY(18);
				break;
			case 18:
				DISCONNECT_RELAY(19);
				break;
			case 19:
				DISCONNECT_RELAY(20);
				break;		
			default:
				break;
		}
}



unsigned char get_msg_source(unsigned int msg)
{
	return (msg >> 24);
}

unsigned char is_relay_runing_timer(unsigned char relay_num,unsigned char timer_num)
{
	if(get_bit(machine_relay_info.realy_opeart.action_num,relay_num))
		{
			if(machine_relay_info.relay[relay_num].timer_num == timer_num)
				{
					return 1;
				}
		}
	return 0;
}

void clear_relay_info(unsigned char relay_num)
{
	value_clear_bit(&(machine_relay_info.realy_opeart.action_num),relay_num);
	machine_relay_info.relay[relay_num].is_relay_busy = 0;
}

unsigned char IsRelayBusy(void)
{	
	unsigned char loopx = 0;
	for(;loopx < MACHINE_REALY_NUMBER; loopx ++)
		{
			if(machine_relay_info.relay[loopx].is_relay_busy)
				{
					return 1;
				}
		}
	return 0;
}

void deal_timer_msg(unsigned char msg_data)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < MACHINE_REALY_NUMBER ; loopx ++)
		{
			if(get_bit(machine_relay_info.realy_opeart.action_num,loopx))
				{
					if(machine_relay_info.relay[loopx].timer_num == msg_data)
						{
							if(machine_relay_info.relay[loopx].relay_operation)
								{
									physical_connect_relay(loopx);
								}
							else
								{
									physical_disconnect_relay(loopx);
								}
							clear_relay_info(loopx);
							return;
						}
				}
		}
}

unsigned char exti_relay(unsigned char exti_msg,unsigned char* relay)//找出跟中断绑定的 需要动作的  继电器
{
	unsigned char loopx = 0;
	unsigned char step = 0;
	for(;loopx<MACHINE_REALY_NUMBER; loopx ++)
		{
			if((exti_msg == machine_relay_info.relay[loopx].zero_phase)&&(get_bit(machine_relay_info.realy_opeart.action_num,loopx)))
				{
					*(relay + step) = loopx;	
					step ++;
				}
		}
	return step;
}

void exti_delay_connect_relay(void* arg)//arg 是定时器编号
{
	unsigned char loopx = 0;
	unsigned char space = (unsigned int)arg;
	for(; loopx < MACHINE_REALY_NUMBER ; loopx ++)
		{
			if(get_bit(machine_relay_info.realy_opeart.action_num,loopx))
				{
					if(machine_relay_info.relay[loopx].timer_num == space)
						{
							physical_connect_relay(loopx);
							clear_relay_info(loopx);
							return;
						}
				}
		}
	
}

void exti_delay_disconnect_relay(void* arg)
{
	unsigned char loopx = 0;
	unsigned char space = (unsigned int)arg;
	for(; loopx < MACHINE_REALY_NUMBER ; loopx ++)
		{
			if(get_bit(machine_relay_info.realy_opeart.action_num,loopx))
				{
					if(machine_relay_info.relay[loopx].timer_num == space)
						{
							physical_disconnect_relay(loopx);
							clear_relay_info(loopx);
							return;
						}
				}
		}
}

void exti_operat_relay(unsigned char relay)//延时一定时间后  断开/闭合   继电器
{
	timer_free(machine_relay_info.relay[relay].timer_num);
	if(machine_relay_info.relay[relay].relay_operation)
		{
			machine_relay_info.relay[relay].timer_num = register_value_into_timer((machine_relay_info.relay[relay].connect_delay_time),0);
			register_callback_function_into_timer(machine_relay_info.relay[relay].timer_num, exti_delay_connect_relay);
		}
	else
		{
			machine_relay_info.relay[relay].timer_num = register_value_into_timer((machine_relay_info.relay[relay].disconnect_delay_time),0);	
			register_callback_function_into_timer(machine_relay_info.relay[relay].timer_num, exti_delay_disconnect_relay);
		}
}

void deal_exti_msg(unsigned char msg_data)
{
	/*spk_beep(msg_data, 50, 500);
	unsigned char loopx;
	for(loopx = 0 ; loopx < MACHINE_REALY_NUMBER ; loopx ++)
		{
			if(get_bit(machine_relay_info.realy_opeart.action_num,loopx))
				{
					if(machine_relay_info.relay[loopx].zero_phase == msg_data)
						{
							if(machine_relay_info.relay[loopx].relay_operation)
								{
									osDelay(machine_relay_info.relay[loopx].connect_delay_time);
									physical_connect_relay(loopx);
								}
							else
								{
									osDelay(machine_relay_info.relay[loopx].disconnect_delay_time);
									physical_disconnect_relay(loopx);
								}
							timer_free(machine_relay_info.relay[loopx].timer_num);
							clear_relay_info(loopx);
						}
				}
		}	*/

	//physical_connect_relay(msg_data + 7);


	unsigned char relay[MACHINE_REALY_NUMBER] = {0};
	unsigned char operat_num = 0;
	unsigned char loopx = 0;
	operat_num = exti_relay(msg_data,relay);
	for(;loopx < operat_num ; loopx++)
		{
			exti_operat_relay(relay[loopx]);
		}
}

void task_deal_relay(unsigned int msg_code)
{
	unsigned char msg_data;
	msg_data = msg_code&0XFF;
	switch(get_msg_source(msg_code))
		{
		case 1://定时器消息
			deal_timer_msg(msg_data);
			break;

		case 2://外部中断消息
			deal_exti_msg(msg_data);
			break;

		case WINDOWS_TIMER_MSG://窗户运转完毕
			UpLoadMachineStatus();
			break;

		case AUTOUPLOAD_MSG://自动上传状态
			UpLoadMachineStatus();
			break;
		default:
				break;
		}
	
	/*if(0 == machine_relay_info.realy_opeart.action_num)
		{
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		}*/
	if(0 == IsRelayBusy())
		{
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			UpLoadMachineStatus();//上传状态信息
		}
}
/////////////////////////////////////////////////////EXTI//////////////////////////////
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
		{
			case ZERO_PIN_A:				
  				exti_send_msg_to_queue(ZERO_MSG_A,RelayQueueHandle);
				break;

			case ZERO_PIN_B:			
  				exti_send_msg_to_queue(ZERO_MSG_B,RelayQueueHandle);
				break;

			case ZERO_PIN_C:			
  				exti_send_msg_to_queue(ZERO_MSG_C,RelayQueueHandle);
				break;

			default:
				break;
					
		}
}

void init_relay_zero_cfg(void)
{
	unsigned char loopx = 0;
	for(;loopx < MACHINE_REALY_NUMBER ; loopx++)
		{
			machine_relay_info.relay[loopx].zero_phase = MachineInfoCfg.SysConfig.relay_zero_cfg[loopx];
		}
}


/////////////////////////////////////////////////////////////////////////////////////windows//////////////////////

void init_windows(void)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < 2 ; loopx++)
		{
			machine_windows_info.windows[loopx].windows_protect_time = 500;
			//machine_windows_info.windows[loopx].windows_set_run_time = (MachineInfoCfg.SysConfig.windows_run_time > 5000 ? MachineInfoCfg.SysConfig.windows_run_time : 5000); 
			machine_windows_info.windows[loopx].windows_set_run_time = (MachineInfoCfg.SysConfig.windows_run_time > 5 ? ((MachineInfoCfg.SysConfig.windows_run_time)*1000) : 5000); 
			machine_windows_info.windows[loopx].status = windows_closed;
		}	
}

void physical_disconnect_windows_relay(unsigned char windows_num)
{
		switch(windows_num)
		{
			case 0:
				DISCONNECT_RELAY(18);
				DISCONNECT_RELAY(17);
				break;

			case 1:
				DISCONNECT_RELAY(20);
				DISCONNECT_RELAY(19);
				break;

			default:
				
				break;
		}
}


void physical_open_windows(unsigned char windows_num)
{
	switch(windows_num)
		{
			case 0:
				DISCONNECT_RELAY(18);
				CONNECT_RELAY(17);
				break;

			case 1:
				DISCONNECT_RELAY(20);
				CONNECT_RELAY(19);
				break;

			default:
				
				break;
		}
}

void physical_close_windows(unsigned char windows_num)
{
	switch(windows_num)
		{
			case 0:
				DISCONNECT_RELAY(17);
				CONNECT_RELAY(18);
				break;

			case 1:
				DISCONNECT_RELAY(19);
				CONNECT_RELAY(20);
				break;

			default:
				
				break;
		}	
}

void timer_windows_callback_function(void* arg)
{
	unsigned char loopx;
	unsigned int timer_num;
	timer_num = (int)arg;
	for(loopx = 0 ; loopx < 2 ; loopx++)
		{
			if((machine_windows_info.windows[loopx].timer == timer_num) && (1 == machine_windows_info.windows[loopx].is_windows_busy))
				{
					machine_windows_info.windows[loopx].is_windows_busy = 0;
					machine_windows_info.windows[loopx].status++;
					physical_disconnect_windows_relay(loopx);
				}
		}
	Timer_Send_Msg_To_Queue(WINDOWS_TIMER_MSG,RelayQueueHandle);
}

void open_windows(unsigned char windows_num)
{
	P_S_Windows_Info p_windows_info;
	p_windows_info = &(machine_windows_info.windows[windows_num]);

	if((windows_opening == p_windows_info->status) || (windows_opend== p_windows_info->status))
		{
			return;
		}
	if(windows_closing == p_windows_info->status)
		{
			physical_disconnect_windows_relay(windows_num);
			osDelay(p_windows_info->windows_protect_time);
		}
	physical_open_windows(windows_num);
	p_windows_info->status = windows_opening;
	p_windows_info->is_windows_busy = 1;
	p_windows_info->timer = register_value_into_timer(p_windows_info->windows_set_run_time,0);
	register_callback_function_into_timer(p_windows_info->timer,timer_windows_callback_function);	
	
	UpLoadMachineStatus();
}


void close_windows(unsigned char windows_num)
{
	P_S_Windows_Info p_windows_info;
	p_windows_info = &(machine_windows_info.windows[windows_num]);

	if((windows_closing== p_windows_info->status) || (windows_closed== p_windows_info->status))
		{
			return;
		}
	if(windows_opening== p_windows_info->status)
		{
			physical_disconnect_windows_relay(windows_num);
			osDelay(p_windows_info->windows_protect_time);
		}
	physical_close_windows(windows_num);
	p_windows_info->status = windows_closing;
	p_windows_info->is_windows_busy = 1;
	p_windows_info->timer = register_value_into_timer(p_windows_info->windows_set_run_time,0);
	register_callback_function_into_timer(p_windows_info->timer,timer_windows_callback_function);	
	
	UpLoadMachineStatus();
}



