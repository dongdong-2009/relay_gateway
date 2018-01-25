#include "transmit.h"


void TransimtData(U_TransmitDirection direction,unsigned char* data,unsigned short data_length)
{
	if(direction.dirction.toRS232_1)
		{
			HAL_UART_Transmit(&RS232_1_COM, data,data_length, 500);
		}
	if(direction.dirction.toRS232_2)
		{
			HAL_UART_Transmit(&RS232_2_COM, data,data_length, 500);
		}
	if(direction.dirction.toRS485_1)
		{
			HAL_UART_Transmit(&RS485_1_COM, data,data_length, 500);
		}
	if(direction.dirction.toRS485_2)
		{
			HAL_UART_Transmit(&RS485_2_COM, data,data_length, 500);
		}
	if(direction.dirction.toTCM300)
		{
			HAL_UART_Transmit(&TCM300_COM, data,data_length, 500);
		}
	if(direction.dirction.toTCP_SERVER)
		{
			
		}
	if(direction.dirction.toTCP_CLIENT)
		{
			
		}
	if(direction.dirction.toUDP)
		{
			
		}

}






