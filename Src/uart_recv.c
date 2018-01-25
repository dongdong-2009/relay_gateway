#include "uart_recv.h"
S_GeneralSeriaRecvData data_from_RS232_1;
S_GeneralSeriaRecvData data_from_RS232_2;
S_GeneralSeriaRecvData data_from_RS485_1;
S_GeneralSeriaRecvData data_from_RS485_2;
S_GeneralSeriaRecvData data_from_KEYBOARD;


S_Uart_Info Tcm300Info;
S_Uart_Info Rs232_1_Info;
S_Uart_Info Rs232_2_Info;
S_Uart_Info Rs485_1_Info;
S_Uart_Info Rs485_2_Info;
S_Uart_Info KeyBoardInfo;


void StartRecv(UART_HandleTypeDef* com,unsigned char* recv_buf,unsigned short recv_buf_size)
{
	HAL_UART_Receive_DMA(com, recv_buf, recv_buf_size);  
	__HAL_UART_ENABLE_IT(com, UART_IT_IDLE);
}


void InitTcm300Info(void)
{
	Tcm300Info.Com = &TCM300_COM;
	Tcm300Info.Timer = &TCM300_TIMER;	
	Tcm300Info.Queue = SerialQueueHandle;
	Tcm300Info.QueueMsg = TCM300_MSG;

	Tcm300Info.recv_buf = data_from_tcm300.rx_data;
	Tcm300Info.recv_buf_size = TCM300_MAX_RX;
	Tcm300Info.dma_cndtr = &(data_from_tcm300.dma_cndtr);
	Tcm300Info.recv_length = &(data_from_tcm300.data_len);
	Tcm300Info.recv_ok = &(data_from_tcm300.recv_ok);
	StartRecv(&TCM300_COM,data_from_tcm300.rx_data,TCM300_RECV_BUFF_SIZE);
}

void InitRS232_1Info(void)
{
	Rs232_1_Info.Com = &RS232_1_COM;
	Rs232_1_Info.Timer = &RS232_1_TIMER;
	Rs232_1_Info.Queue = SerialQueueHandle;
	Rs232_1_Info.QueueMsg = RS232_1_MSG;

	Rs232_1_Info.recv_buf = data_from_RS232_1.rx_data;
	Rs232_1_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	Rs232_1_Info.dma_cndtr = &(data_from_RS232_1.dma_cndtr);
	Rs232_1_Info.recv_length = &(data_from_RS232_1.data_len);
	Rs232_1_Info.recv_ok = &(data_from_RS232_1.recv_ok);
	StartRecv(&RS232_1_COM,data_from_RS232_1.rx_data,GENERAL_RECV_BUFF_SIZE);
}

void InitRS232_2Info(void)
{
	Rs232_2_Info.Com = &RS232_2_COM;
	Rs232_2_Info.Timer = &RS232_2_TIMER;
	Rs232_2_Info.Queue = SerialQueueHandle;
	Rs232_2_Info.QueueMsg = RS232_2_MSG;

	Rs232_2_Info.recv_buf = data_from_RS232_2.rx_data;
	Rs232_2_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	Rs232_2_Info.dma_cndtr = &(data_from_RS232_2.dma_cndtr);
	Rs232_2_Info.recv_length = &(data_from_RS232_2.data_len);	
	Rs232_2_Info.recv_ok = &(data_from_RS232_2.recv_ok);
	StartRecv(&RS232_2_COM,data_from_RS232_2.rx_data,GENERAL_RECV_BUFF_SIZE);
}

void InitRS485_1Info(void)
{
	Rs485_1_Info.Com = &RS485_1_COM;
	Rs485_1_Info.Timer = &RS485_1_TIMER;
	Rs485_1_Info.Queue = SerialQueueHandle;
	Rs485_1_Info.QueueMsg = RS485_1_MSG;

	Rs485_1_Info.recv_buf = data_from_RS485_1.rx_data;
	Rs485_1_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	Rs485_1_Info.dma_cndtr = &(data_from_RS485_1.dma_cndtr);
	Rs485_1_Info.recv_length = &(data_from_RS485_1.data_len);	
	Rs485_1_Info.recv_ok = &(data_from_RS485_1.recv_ok);
	StartRecv(&RS485_1_COM,data_from_RS485_1.rx_data,GENERAL_RECV_BUFF_SIZE);
}

void InitRS485_2Info(void)
{
	Rs485_2_Info.Com = &RS485_2_COM;
	Rs485_2_Info.Timer = &RS485_2_TIMER;
	Rs485_2_Info.Queue = SerialQueueHandle;
	Rs485_2_Info.QueueMsg = RS485_2_MSG;

	Rs485_2_Info.recv_buf = data_from_RS485_2.rx_data;
	Rs485_2_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	Rs485_2_Info.dma_cndtr = &(data_from_RS485_2.dma_cndtr);
	Rs485_2_Info.recv_length = &(data_from_RS485_2.data_len);	
	Rs485_2_Info.recv_ok = &(data_from_RS485_2.recv_ok);
	StartRecv(&RS485_2_COM,data_from_RS485_2.rx_data,GENERAL_RECV_BUFF_SIZE);
}

void InitKeyboardInfo(void)
{
	KeyBoardInfo.Com = &KEY_BOARD_COM;
	KeyBoardInfo.Timer = &KEY_BOARD_TIMER;
	KeyBoardInfo.Queue = SerialQueueHandle;
	KeyBoardInfo.QueueMsg = KEY_BOARD_MSG;

	KeyBoardInfo.recv_buf = data_from_KEYBOARD.rx_data;
	KeyBoardInfo.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	KeyBoardInfo.dma_cndtr = &(data_from_KEYBOARD.dma_cndtr);
	KeyBoardInfo.recv_length = &(data_from_KEYBOARD.data_len);	
	KeyBoardInfo.recv_ok = &(data_from_KEYBOARD.recv_ok);	
	StartRecv(&KEY_BOARD_COM,data_from_KEYBOARD.rx_data,GENERAL_RECV_BUFF_SIZE);
}

void InitSerialBus(void)
{
	InitTcm300Info();
	InitRS232_1Info();
	InitRS232_2Info();
	InitRS485_1Info();
	InitRS485_2Info();
	InitKeyboardInfo();
}


void UartTimerOpeart(TIM_HandleTypeDef *htim,S_Uart_Info *Info)
{
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;	
	
	if(htim != (Info->Timer))
		{
			return;
		}
	HAL_TIM_Base_Stop_IT(htim);
	
	if(*(Info->dma_cndtr) ==  (Info->Com->hdmarx->Instance->NDTR))
		{			
			HAL_UART_DMAStop(Info->Com);   
			*(Info->recv_length)= (Info->recv_buf_size) -*(Info->dma_cndtr);
			*(Info->recv_ok) = 1;
			HAL_UART_Receive_DMA((Info->Com), (Info->recv_buf), (Info->recv_buf_size)); 
			xQueueSendFromISR((Info->Queue),&(Info->QueueMsg),&xHigherPriorityTaskWoken);
		}
	else
		{
			HAL_TIM_Base_Start_IT(htim);
			*(Info->dma_cndtr) =  Info ->Com->hdmarx->Instance->NDTR;
		}
}

void UartIdleOperat(UART_HandleTypeDef *huart,S_Uart_Info *Info) 
{
	if(Info->Com != huart)
		{
			return;
		}
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
	    {   
	        __HAL_UART_CLEAR_IDLEFLAG(huart);  
		*(Info->dma_cndtr) = huart->hdmarx->Instance->NDTR;
		HAL_TIM_Base_Start_IT(Info->Timer);
	    } 
}


void test_serial(void)
{
	char buf[50] = {0};
	unsigned char loop6 = 0;
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,timer_counter.second_tick);
	HAL_UART_Transmit(&huart1, (unsigned char*)buf, 30, 500);
	osDelay(50);
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,timer_counter.second_tick);
	HAL_UART_Transmit(&huart2, (unsigned char*)buf, 30, 500);
	osDelay(50);
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,timer_counter.second_tick);
	HAL_UART_Transmit(&huart3, (unsigned char*)buf, 30, 500);
	osDelay(50);
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,timer_counter.second_tick);
	HAL_UART_Transmit(&huart4, (unsigned char*)buf, 30, 500);
	osDelay(50);
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,timer_counter.second_tick);
	HAL_UART_Transmit(&huart5, (unsigned char*)buf, 30, 500);
	osDelay(50);
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,timer_counter.second_tick);
	HAL_UART_Transmit(&huart6, (unsigned char*)buf, 30, 500);
	osDelay(50);
}










