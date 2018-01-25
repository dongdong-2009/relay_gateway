#include "key.h"
key_data_def rtos_key;



unsigned char get_key_value(unsigned char key_number)
{
	unsigned char key_status;
	switch(key_number)
		{
			case 0:
				//key_status = HAL_GPIO_ReadPin(KEY_1_GPIO_Port,KEY_1_Pin);
				break;				
			
		}
	return key_status;
}



void read_key(void)
{
unsigned char key_trigger_value;

unsigned char loopx = 0;
	for(loopx =0 ;loopx < KEY_NUMBER; loopx++)
	{
		if(GPIO_PIN_RESET == (GPIO_PinState)(get_key_value(loopx)))
			{
				rtos_key.press_time[loopx] = (rtos_key.press_time[loopx] >0XEE) ? 0XEE : rtos_key.press_time[loopx] ;
  				rtos_key.press_time[loopx] ++;
			}
	}
	
	for(rtos_key.key_point = 0; rtos_key.key_point <KEY_NUMBER ;rtos_key.key_point++)
		{
			if((rtos_key.press_time[rtos_key.key_point ]) > 2)
				{
					rtos_key.key_value[rtos_key.key_point ] = key_press(rtos_key.key_point);	
						if((rtos_key.key_ok[rtos_key.key_point ]&0X01) == 0)
									{
										rtos_key.key_ok[rtos_key.key_point ] |= 0X01; 
										key_trigger_value =rtos_key.key_point;
										xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
									}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0X40)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_press(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X02) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X02; 
									key_trigger_value =KEY_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
								}
						}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0XAF)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_long_press(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X04) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X04; 
									key_trigger_value =KEY_LONG_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
								}
						}

				}
		}

	for(loopx = 0;loopx < KEY_NUMBER; loopx++)
	{
		if(GPIO_PIN_SET == (GPIO_PinState)(get_key_value(loopx)))
			{
				if(rtos_key.key_ok[loopx])
					{
						rtos_key.key_ok[loopx] = 0;
						xQueueSend(KeyQueueHandle, &(rtos_key.key_value[loopx]), 10);
					}
				rtos_key.press_time[loopx] = 0;
			}
	}
		
}


void deal_key_value(unsigned key_value)
{
	static unsigned char step = 0;
	switch(key_value)
  		{
  			case(KEY_1_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_2_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_3_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_4_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_5_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_6_TRIGGER):

				break;
			case(KEY_7_TRIGGER):

				break;
			case(KEY_8_TRIGGER):

				break;
			case(KEY_9_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_10_TRIGGER):
				//LED_5_TOG;
				break;

			case(KEY_1_LONG_TRIGGER):
				
				break;

			case(KEY_2_LONG_TRIGGER):
				break;

			case(KEY_3_LONG_TRIGGER):
				break;

			case(KEY_4_LONG_TRIGGER):
				break;

			case(KEY_5_LONG_TRIGGER):
				
				break;

			case(KEY_6_LONG_TRIGGER):
				break;

			case(KEY_7_LONG_TRIGGER):
				break;

			case(KEY_8_LONG_TRIGGER):
				break;

			case(KEY_9_LONG_TRIGGER):
				break;

			case(KEY_10_LONG_TRIGGER):
				break;

			case(KEY_1_LONG_LONG_TRIGGER):
				
				break;
			
			case(KEY_2_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_3_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_4_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_5_LONG_LONG_TRIGGER):
				
				break;

			case(KEY_6_LONG_LONG_TRIGGER):
				break;

			case(KEY_7_LONG_LONG_TRIGGER):
				break;

			case(KEY_8_LONG_LONG_TRIGGER):
				break;

			case(KEY_9_LONG_LONG_TRIGGER):
				break;

			case(KEY_10_LONG_LONG_TRIGGER):
				break;
				


				
  			case(key1_press):
				if(get_bit(machine_relay_info.relay_status,0))
					{
						DisConnectRelay(0);
					}
				else
					{
						ConnectRelay(0);
					}
				break;

			case(key1_long_press):
				
				break;

			case(key1_long_long_press):

				break;
				
			case(key2_press): 
				if(get_bit(machine_relay_info.relay_status,1))
					{
						DisConnectRelay(1);
					}
				else
					{
						ConnectRelay(1);
					}				
				break;

			case(key2_long_press):
				
				break;
				
			case(key3_press):
				if(get_bit(machine_relay_info.relay_status,2))
					{
						DisConnectRelay(2);
					}
				else
					{
						ConnectRelay(2);
					}
				break;

			case(key3_long_press):
			
				break;
				
			case(key4_press):
				if(get_bit(machine_relay_info.relay_status,3))
					{
						DisConnectRelay(3);
					}
				else
					{
						ConnectRelay(3);
					}				
				break;

			case(key4_long_press):
			
				break;
				
			case(key5_press):
				if(get_bit(machine_relay_info.relay_status,4))
					{
						DisConnectRelay(4);
					}
				else
					{
						ConnectRelay(4);
					}				
				
				break;

			case(key5_long_press):
				
				break;

			case(key5_long_long_press):
      
				break;


			case(key6_press):				
					DisConnectRelay(step--);				
				break;


			case(key7_press):
					ConnectRelay(step++);				
				break;

				default:
				break;
				
  		}
}



