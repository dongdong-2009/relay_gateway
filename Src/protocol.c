#include "protocol.h"
#include "sockets.h"
#include "machine_info_cfg.h"
S_UDP_Ctrl_Server_Info udp_ctrl_server;
unsigned char CkeckFrameHead(unsigned char* source_data)
{
	if((FRAMEHEAD_0 == *source_data)&&(FRAMEHEAD_1 == *(source_data+1)))
		{
			return 1;
		}
	return 0;
}

unsigned char Shall_I_Do_It(P_S_ProtocolFrame protocol_frame)
{
	if(protocol_frame->machine_ID[0] == 0XFF&&\
	protocol_frame->machine_ID[1] ==  0XFF&&\
	protocol_frame->machine_ID[2] == 0XFF&&\
	protocol_frame->machine_ID[3] == 0XFF)
		{
			return 1;
		}

	if(protocol_frame->machine_ID[0] == id.TCM300_ID[0]&&\
	protocol_frame->machine_ID[1] ==  id.TCM300_ID[1]&&\
	protocol_frame->machine_ID[2] ==  id.TCM300_ID[2]&&\
	protocol_frame->machine_ID[3] ==  id.TCM300_ID[3])
		{
			return 1;
		}
	return 0;
}

void DecodeProtocolFrame(unsigned char* source_data,P_S_ProtocolFrame protocol_frame)
{
	if(0 == CkeckFrameHead(source_data))
		{return;}
	protocol_frame->source_data_length = *(source_data+2);
	protocol_frame->machine_ID[0] = *(source_data+3);
	protocol_frame->machine_ID[1] = *(source_data+4);
	protocol_frame->machine_ID[2] = *(source_data+5);
	protocol_frame->machine_ID[3] = *(source_data+6);
	protocol_frame->founction_code = *(source_data+7);
	protocol_frame->all_arg_num = *(source_data+8);
	protocol_frame->next_arg_length = *(source_data+9);
	protocol_frame->next_arg = (source_data+10);

	protocol_frame->crc8 = *(source_data+2+(protocol_frame->source_data_length));
}



void Net_Ctrl_DealFunction(void* arg)
{
	P_S_Socket_Task_Info task_info = NULL;
	task_info = (P_S_Socket_Task_Info)arg;

	S_ProtocolFrame protocol_frame ={0};
	DecodeProtocolFrame((task_info->recv_buf),&protocol_frame);
	ExecuteProtocolFrame(&protocol_frame);
}

void Ctrl_DealFunction(unsigned char* data)//解析控制协议
{
	S_ProtocolFrame protocol_frame ={0};
	DecodeProtocolFrame(data,&protocol_frame);
	ExecuteProtocolFrame(&protocol_frame);
}


void InitUDPCtrlServer(void)
{
	udp_ctrl_server.server.buf_length = UDP_CTRL_SERVER_BUFF_LENGTH;
	udp_ctrl_server.server.port = UDP_CTRL_SERVER_PORT;
	
	udp_ctrl_server.server.recv_buf = udp_ctrl_server.server_buff;

	udp_ctrl_server.server.socket_deal_function = Net_Ctrl_DealFunction;
	
	socket_creat_UDP_server_task("udp_ctrl_server",&(udp_ctrl_server.server));
}


void UploadData(unsigned char function_code,unsigned char* args)
{
	P_S_Socket_Task_Info task_info = NULL;
	task_info = &(udp_ctrl_server.server);

	unsigned char SendBuff[PROTOCOL_ARG_LENGTH + 10] = {0};
	unsigned char loopx = 0;
	SendBuff[0] = 0XAA;
	SendBuff[1] = 0X60;
	SendBuff[2] = (8+PROTOCOL_ARG_LENGTH);
	SendBuff[3] =  id.TCM300_ID[0];
	SendBuff[4] =  id.TCM300_ID[1];
	SendBuff[5] =  id.TCM300_ID[2];
	SendBuff[6] =  id.TCM300_ID[3];
	SendBuff[7] =  function_code;

	for(;loopx<PROTOCOL_ARG_LENGTH; loopx++)
		{
			SendBuff[8+loopx] = *(args + loopx);
		}
	SetCRC8Sub(SendBuff, (PROTOCOL_ARG_LENGTH + 9));
	sendto((task_info->socket_num - SOCKET_OFFSET),SendBuff,(PROTOCOL_ARG_LENGTH + 10),0,(struct sockaddr *)&(task_info->UDP_client_address),sizeof(task_info->UDP_client_address));
	if(Is_Ctrl_Side_Effect())
		{
			MachineInfoCfg.Ctrl_Side[0].send_to_ctrl_side(SendBuff,(PROTOCOL_ARG_LENGTH + 10));
		}
	
	HAL_UART_Transmit(&KEY_BOARD_COM, (unsigned char*)SendBuff, (PROTOCOL_ARG_LENGTH + 10), 500);
}



void UpLoadMachineStatus(void)
{
	unsigned char loopx = 0;
	unsigned char arg[PROTOCOL_ARG_LENGTH] ={0};
	/*继电器状态*/
	arg[0] = machine_relay_info.relay_status >> 24;
	arg[1] = machine_relay_info.relay_status >> 16;
	arg[2] = machine_relay_info.relay_status >> 8;
	arg[3] = machine_relay_info.relay_status >> 0;

	/*窗户状态*/
	arg[4] = (machine_windows_info.windows[0].status << 4)|(machine_windows_info.windows[1].status);


	/*开窗时间*/	
	arg[5] = MachineInfoCfg.SysConfig.windows_run_time >> 24;
	arg[6] = MachineInfoCfg.SysConfig.windows_run_time >> 16;
	arg[7] = MachineInfoCfg.SysConfig.windows_run_time >> 8;
	arg[8] = MachineInfoCfg.SysConfig.windows_run_time >> 0;

	/*温度湿度1*/
	arg[9] = MachineInfoCfg.Temp_Hum[0].temperature;
	arg[10] = MachineInfoCfg.Temp_Hum[0].humidity;

	/*温度湿度2*/
	arg[11] = MachineInfoCfg.Temp_Hum[1].temperature;
	arg[12] = MachineInfoCfg.Temp_Hum[1].humidity;

	/*温度湿度3*/
	arg[13] = MachineInfoCfg.Temp_Hum[2].temperature;
	arg[14] = MachineInfoCfg.Temp_Hum[2].humidity;


	/*继电器过零检测设置*/
	for(;loopx < MACHINE_REALY_NUMBER; loopx ++)
		{
			arg[15+loopx] = machine_relay_info.relay[loopx].zero_phase;
		}

	
	UploadData(UPLOAD_RELAY_STATUS,arg);	
}

void UpLoadSys_ID_IP(unsigned char mod)
{
	unsigned char arg[PROTOCOL_ARG_LENGTH] ={0};
	arg[0] = id.TCM300_ID[0];
	arg[1] = id.TCM300_ID[1];
	arg[2] = id.TCM300_ID[2];
	arg[3] = id.TCM300_ID[3];

	arg[4] = (MachineInfoCfg.SysConfig.IPAddress.IP) >> 24;
	arg[5] = (MachineInfoCfg.SysConfig.IPAddress.IP) >> 16;
	arg[6] = (MachineInfoCfg.SysConfig.IPAddress.IP) >> 8;
	arg[7] = (MachineInfoCfg.SysConfig.IPAddress.IP) >> 0;

	
	arg[8] = (MachineInfoCfg.SysConfig.IPAddress.GateWay) >> 24;
	arg[9] = (MachineInfoCfg.SysConfig.IPAddress.GateWay) >> 16;
	arg[10] = (MachineInfoCfg.SysConfig.IPAddress.GateWay) >> 8;
	arg[11] = (MachineInfoCfg.SysConfig.IPAddress.GateWay) >> 0;
	
	UploadData(mod,arg);
}


void SetWindowsRunTime(unsigned char* data)
{
	unsigned char loopx;
	unsigned int set_time = 0;
	set_time |=  *(data);
	set_time <<= 8;
	set_time |=  *(data+1);
	set_time <<= 8;
	set_time |=  *(data+2);
	set_time <<= 8;
	set_time |=  *(data+3);

	MachineInfoCfg.SysConfig.windows_run_time = set_time;

	for(loopx = 0 ; loopx < 2 ; loopx++)
		{
			machine_windows_info.windows[loopx].windows_protect_time = 500;
			machine_windows_info.windows[loopx].windows_set_run_time = (MachineInfoCfg.SysConfig.windows_run_time > 5 ? ((MachineInfoCfg.SysConfig.windows_run_time)*1000) : 5000); 
		}
	Save_Data(cell_SYSConfig, (unsigned char*)&MachineInfoCfg,sizeof(MachineInfoCfg));	
}


void SetRelayZeroCFG(unsigned char* data)
{
	unsigned char loopx = 0;
	for(;loopx<MACHINE_REALY_NUMBER ; loopx++)
		{
			machine_relay_info.relay[loopx].zero_phase = *(data + loopx);
			MachineInfoCfg.SysConfig.relay_zero_cfg[loopx] = *(data + loopx);
		}
	
	Save_Data(cell_SYSConfig, (unsigned char*)&MachineInfoCfg,sizeof(MachineInfoCfg));

}


unsigned char ExecuteProtocolFrame(P_S_ProtocolFrame protocol_frame)
{
	if(0 == Shall_I_Do_It(protocol_frame))
		{return 0;}
	switch(protocol_frame->founction_code)
		{
			case GET_SYS_ID_IP:
				UpLoadSys_ID_IP(UPLOAD_SYS_ID_IP);
				break;
			case PULL_SYS_ID_IP:
				UpLoadSys_ID_IP(PUSH_SYS_ID_IP);
				break;
				

			case SET_RELAY_TOG:
				TogRelay(*(protocol_frame->next_arg));
				break;
			case LEARN_SWITCH://学习开关
				Start_Learn_Action();
				break;

			case CLEAR_SWITCH://清除开关
				Start_Clear_Action();
				break;

			case SET_RELAY_ON://开继电器
				ConnectRelay(*(protocol_frame->next_arg));
				break;

			case SET_RELAY_OFF://关继电器
				DisConnectRelay(*(protocol_frame->next_arg));
				break;

			case SET_WINDOWS_ON://开窗户
				open_windows(*(protocol_frame->next_arg));
				break;

			case SET_WINDOWS_OFF://关窗户
				close_windows(*(protocol_frame->next_arg));
				break;

			case SET_CONFIG://配置机器
				SaveMachineConfig(protocol_frame->next_arg);
				break;

			case REBOOT_MACHINE://重启设备
				NVIC_SystemReset();
				break;

			case UPLOAD_SYS_CFG://上传配置信息
				UpLoadSys_Cfg();
				break;

			case SET_WINDOWS_RUN_TIME://设置窗户运行时间
				SetWindowsRunTime(protocol_frame->next_arg);
				break;

			case SET_RELAY_ZERO://设置过零检测
				SetRelayZeroCFG(protocol_frame->next_arg);
				break;
				
			default:
				break;
		}
	UpLoadMachineStatus();
	return 1;
}

