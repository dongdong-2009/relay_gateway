#include "include.h"

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{	  
	/*if(huart == &huart1)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			data_from_tcm300.dma_cndtr = huart->hdmarx->Instance->NDTR; 
			HAL_TIM_Base_Start_IT(&htim1);
		    }  
	  	}	  
	if(huart == &huart2)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			data_from_tcm300.dma_cndtr = huart->hdmarx->Instance->NDTR; 
			HAL_TIM_Base_Start_IT(&htim2);
		    }  
	  	}	  
	if(huart == &huart3)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			data_from_tcm300.dma_cndtr = huart->hdmarx->Instance->NDTR; 
			HAL_TIM_Base_Start_IT(&htim3);
		    }  
	  	}	  
	if(huart == &huart4)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			data_from_tcm300.dma_cndtr = huart->hdmarx->Instance->NDTR; 
			HAL_TIM_Base_Start_IT(&htim4);
		    }  
	  	}	  
	if(huart == &huart5)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			data_from_tcm300.dma_cndtr = huart->hdmarx->Instance->NDTR; 
			HAL_TIM_Base_Start_IT(&htim5);
		    }  
	  	}	  
	if(huart == &huart6)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			data_from_tcm300.dma_cndtr = huart->hdmarx->Instance->NDTR; 
			HAL_TIM_Base_Start_IT(&htim6);
		    }  
	  	}*/

	UartIdleOperat(huart,&Tcm300Info);
	UartIdleOperat(huart,&Rs232_1_Info);
	UartIdleOperat(huart,&Rs232_2_Info);
	UartIdleOperat(huart,&Rs485_1_Info);
	UartIdleOperat(huart,&Rs485_2_Info);
	UartIdleOperat(huart,&KeyBoardInfo);
}





