#include "tcm300.h"
S_Recv_From_Tcm300 data_from_tcm300;
S_ID id;


void read_tcm300_id(void)
{
	data_from_tcm300.tx_data[0]= 0X55;
	data_from_tcm300.tx_data[1]= 0X00;
	data_from_tcm300.tx_data[2]= 0X01;
	data_from_tcm300.tx_data[3]= 0X00;
	data_from_tcm300.tx_data[4]= 0X05;
	data_from_tcm300.tx_data[5]= 0X70;
	data_from_tcm300.tx_data[6]= 0X03;
	data_from_tcm300.tx_data[7]= 0X09;
	id.read_tcm300_id_flag = 1;
	HAL_UART_Transmit(&TCM300_COM,data_from_tcm300.tx_data,8,800);
	HAL_Delay(20);
//	while(1)
		{
			//if(data_from_tcm300.recv_ok == 1)
				//{
					data_from_tcm300.recv_ok = 0;
					if(VerifyCRC8Sub(&data_from_tcm300.rx_data[6],(data_from_tcm300.rx_data[2]+data_from_tcm300.rx_data[3])) == 0XFF)
						{
							if(data_from_tcm300.rx_data[4] == 0X02)
								{
									id.TCM300_ID[0] = data_from_tcm300.rx_data[15];
									id.TCM300_ID[1] = data_from_tcm300.rx_data[16];
									id.TCM300_ID[2] = data_from_tcm300.rx_data[17];
									id.TCM300_ID[3] = data_from_tcm300.rx_data[18];
									if((id.TCM300_ID[0] == 0x00)&&(id.TCM300_ID[1] == 0x00)&&(id.TCM300_ID[2] == 0x00)&&(id.TCM300_ID[3] == 0x00))
										{
											id.read_tcm300_id_flag = 0;
											return;
										}
									else
										{
											id.read_tcm300_id_flag = 0;
											return;
										}
								}
						}
					id.read_tcm300_id_flag = 0;
					return;
				//}
		}
	//id.read_tcm300_id_flag = 0;
	//send_data_to_gateway_by_msc(0x68,0x88,"hello",5);
}








void set_send_gateway_command_code(unsigned char manufacturer_id,unsigned char master_command,unsigned char command)
{
	data_from_tcm300.tx_data[6] = 0XD1;
	data_from_tcm300.tx_data[7] = manufacturer_id;	 ///
	data_from_tcm300.tx_data[8] = master_command;	///
	data_from_tcm300.tx_data[9] = command;
}

void set_send_gateway_data_replace_id_code(unsigned char **data,unsigned char* length)
{
	unsigned char loopx,tmp_loop;
	data_from_tcm300.tx_data[10] = 0; 
	data_from_tcm300.tx_data[11] = 0;
	data_from_tcm300.tx_data[12] = 0;
	data_from_tcm300.tx_data[13] = 0;
	data_from_tcm300.tx_data[14] = 0;
	tmp_loop = *length;
	tmp_loop = tmp_loop >5? 5: tmp_loop;
	for(loopx = 0;loopx < tmp_loop;loopx ++)
		{
			data_from_tcm300.tx_data[10+loopx] = *((*data)++); 
			(*length) --;
		}
}


void Set_Send_Gateway_DataCodeSub(unsigned char *data, unsigned char* data_length)
{
	unsigned char* tmp_data;
	unsigned char length;
	unsigned char  vLoop;
	length = *data_length;
	tmp_data = data;	
	if(length ==0X00)
	{       return;                                           }

	for(vLoop=0X01;vLoop<=length;vLoop++)
	{
		data_from_tcm300.tx_data[vLoop+0X0E]=*tmp_data;
		tmp_data++;
        }
}

void set_send_gateway_head_code(unsigned char data_len)
{
	data_from_tcm300.tx_data[0] = 0X55;
	data_from_tcm300.tx_data[1] = 0x00;
	data_from_tcm300.tx_data[2] = 0X0E + data_len;
	data_from_tcm300.tx_data[3] = 0X06;
	data_from_tcm300.tx_data[4] = 0X01;
	SetCRC8Sub(&data_from_tcm300.tx_data[1],4);	
}

void set_send_gateway_end_code(unsigned char data_len)
{
	unsigned char vlong;
	data_from_tcm300.tx_data[data_len + 0X0F] = id.TCM300_ID[0];
	data_from_tcm300.tx_data[data_len + 0X10] = id.TCM300_ID[1];
	data_from_tcm300.tx_data[data_len + 0X11] = id.TCM300_ID[2];
	data_from_tcm300.tx_data[data_len + 0X12] = id.TCM300_ID[3];
	data_from_tcm300.tx_data[data_len + 0X13] = 0X00;
	data_from_tcm300.tx_data[data_len + 0X14] = 0X03;
	
	data_from_tcm300.tx_data[data_len + 0X15] = 0XFF;
	data_from_tcm300.tx_data[data_len + 0X16] = 0XFF;
	data_from_tcm300.tx_data[data_len + 0X17] = 0XFF;
	data_from_tcm300.tx_data[data_len + 0X18] = 0XFF;
	data_from_tcm300.tx_data[data_len + 0X19] = 0XFF;
	vlong = data_len+0X14;
	 SetCRC8Sub(&data_from_tcm300.tx_data[0X06],vlong);
}


void send_data_to_gateway_by_msc(unsigned char master_command,unsigned char salve_command,unsigned char* data,unsigned char length)
{
	unsigned char send_data_length;
	unsigned char** send_data;
	
	send_data_length = length;
	send_data = &data;	
	set_send_gateway_command_code(0X68,master_command,salve_command);
	set_send_gateway_data_replace_id_code(send_data, &send_data_length);
	Set_Send_Gateway_DataCodeSub( *send_data,&send_data_length);
	if(length<6)
		{
			set_send_gateway_head_code(0);
			set_send_gateway_end_code(0);
			HAL_UART_Transmit(&TCM300_COM,(data_from_tcm300.tx_data),27,900);
		}
	else
		{
			set_send_gateway_head_code(length -5);
			set_send_gateway_end_code(length - 5);
			HAL_UART_Transmit(&TCM300_COM,data_from_tcm300.tx_data,length +22,900);
		}
	
}


void DeCodeRPSRadio(unsigned char* recv_data,P_S_Switch_Info SwitchInfo)
{
	SwitchInfo->tcm300_ID[0] = *(recv_data+8);	
	SwitchInfo->tcm300_ID[1] = *(recv_data+9);	
	SwitchInfo->tcm300_ID[2] = *(recv_data+10);	
	SwitchInfo->tcm300_ID[3] = *(recv_data+11);

	SwitchInfo->key_value = *(recv_data+7);
}


void Deal_RPS_RadioSub(void)
{
	S_Switch_Info SwitchInfo = {0};
	unsigned char switch_index = 0XFF;
	DeCodeRPSRadio(data_from_tcm300.rx_data,&SwitchInfo);
	
	switch_index = get_learn_space(&SwitchInfo);
					
	if(Is_Learn_Actioning())// learn
		{
			Clear_Learn_Flag();
			learn_switch(&SwitchInfo);
			if(0XFF == switch_index)
				{
					spk_beep(2, 100, 300);
				}else{
					spk_beep(1, 100, 300);
				}
			return;
		}

	if(Is_Clear_Actioning())// clear
		{
			Clear_Clear_Flag();
			if(0XFF == switch_index)
				{
					spk_beep(2, 200, 100);
				}else{
					clear_switch(&SwitchInfo);
					spk_beep(1, 200, 100);
				}
			return;
		}
	
	if(0XFF == switch_index)
		{
		}
	else
		{
			SetRelayStatus(SwitchSaveInfo.scene[switch_index].relay_status);
			if((windows_opend == SwitchSaveInfo.scene[switch_index].windows1_status)||(windows_opening == SwitchSaveInfo.scene[switch_index].windows1_status))
				{open_windows(0);}else{close_windows(0);}
			if((windows_opend == SwitchSaveInfo.scene[switch_index].windows2_status)||(windows_opening == SwitchSaveInfo.scene[switch_index].windows2_status))
				{open_windows(1);}else{close_windows(1);}
		}
}



void task_deal_tcm300(void)
{
	data_from_tcm300.recv_ok = 0;
	if(VerifyCRC8Sub(&data_from_tcm300.rx_data[6],(data_from_tcm300.rx_data[2]+data_from_tcm300.rx_data[3])) == 0XFF)
		{
			if(data_from_tcm300.rx_data[4] == 1)
				{
					switch(data_from_tcm300.rx_data[6])
						{
							case 0XF6:
						             Deal_RPS_RadioSub();
						             break;

						        case 0XA5:
						             //Deal_4BS_RadioSub();
						             break;

						        case 0XC5:
						             //Deal_SYSEX_RadioSub();
						             break;

						        case 0XD1:
	                       				//Deal_Gateway_RadioSub();
						             break;

	                	                        default:
	                	                             break;
						}
				}
			if(data_from_tcm300.rx_data[4] == 7)
				{
					data_from_tcm300.vBigRxMax =data_from_tcm300.rx_data[2]-4;
					//DealBigdata();
				}
			
		}
}

void test_tcm300(void)
{
	P_S_Switch_Info switch_value= NULL;
	switch_value =(P_S_Switch_Info)malloc(sizeof(S_Switch_Info));
	switch_value->tcm300_ID[0] = 0X00;
	switch_value->tcm300_ID[1] = 0X2C;
	switch_value->tcm300_ID[2] = 0X4C;
	switch_value->tcm300_ID[3] = 0X39;
	switch_value->key_value = cOperateCode_A;
	SetRelayStatus(0X5555);
	osDelay(1000);
	learn_switch(switch_value);

	
	switch_value->key_value = cOperateCode_B;
	SetRelayStatus(0XAAAA);
	osDelay(1000);
	learn_switch(switch_value);

	
	switch_value->key_value = cOperateCode_C;
	SetRelayStatus(0XFF00);
	osDelay(1000);
	learn_switch(switch_value);

	
	switch_value->key_value = cOperateCode_D;
	SetRelayStatus(0X00FF);
	osDelay(1000);
	learn_switch(switch_value);
	
	osDelay(1000);
	Save_Data(cell_Switch, (unsigned char*)&SwitchSaveInfo,sizeof(SwitchSaveInfo));
	free(switch_value);
}


