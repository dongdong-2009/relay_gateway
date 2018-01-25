#include "machine_info_cfg.h"
#include "protocol.h"



S_Machine_Info_Cfg MachineInfoCfg;
unsigned char AutoUpLoadTimer;

unsigned int IPStrToInt(const char *ip)  
{  
    unsigned uResult = 0;  
    int nShift = 24;  
    int temp = 0;  
    const char *pStart = ip;  
    const char *pEnd = ip;  
      
    while (*pEnd != '\0')  
    {  
        while (*pEnd!='.' && *pEnd!='\0')  
        {  
            pEnd++;  
        }  
        temp = 0;  
        for (; pStart!=pEnd; ++pStart)  
        {  
            temp = temp * 10 + *pStart - '0';  
        }     
          
        uResult += temp<<nShift;  
        nShift -= 8;  
          
        if (*pEnd == '\0')  
            break;  
        pStart = pEnd + 1;  
        pEnd++;  
    }  
      
    return uResult;  
}   

char *IntToStr(const int ip, char *buf)  
{  
    sprintf(buf, "%u.%u.%u.%u",  
        (unsigned char )*((char *)&ip + 3),  
        (unsigned char )*((char *)&ip + 2),  
        (unsigned char )*((char *)&ip + 1),  
        (unsigned char )*((char *)&ip + 0));  
    return buf;  
} 

void TCPServer_Send_To_CtrlSide(unsigned char* data,unsigned int data_size)
{	
	P_S_Socket_Task_Info server_socket;
	server_socket = (P_S_Socket_Task_Info)(MachineInfoCfg.Ctrl_Side[0].socket_info);	
	if(server_socket->is_connected)
		{
			send((server_socket->client_id),data,data_size,0);
		}
}

void TCPClient_Send_To_CtrlSide(unsigned char* data,unsigned int data_size)
{	
	P_S_Socket_Task_Info client_socket;
	client_socket = (P_S_Socket_Task_Info)(MachineInfoCfg.Ctrl_Side[0].socket_info);	
	if(client_socket->is_connected)
		{
			send((client_socket->socket_num - SOCKET_OFFSET),data,data_size,0);
		}
}

void UDP_Send_To_CtrlSide(unsigned char* data,unsigned int data_size)
{	
	P_S_Socket_Task_Info UDP_socket;
	UDP_socket = (P_S_Socket_Task_Info)(MachineInfoCfg.Ctrl_Side[0].socket_info);	
	if(UDP_socket->is_connected)
		{
			sendto((UDP_socket->socket_num - SOCKET_OFFSET),data,data_size,0,(struct sockaddr *)&(UDP_socket->UDP_client_address),sizeof(UDP_socket->UDP_client_address));
		}
}

unsigned char Is_Ctrl_Side_Effect(void)
{
	if((MachineInfoCfg.Ctrl_Side[0].Ctrl_Protocol == TCP_CLIENT_PROTOCOL)&&(MachineInfoCfg.Ctrl_Side[0].Device_Port))
		{return 1;}
	if((MachineInfoCfg.Ctrl_Side[0].Ctrl_Protocol == TCP_SERVER_PROTOCOL)&&(MachineInfoCfg.Ctrl_Side[0].Ctrl_IP)&&(MachineInfoCfg.Ctrl_Side[0].Ctrl_Port))
		{return 1;}
	if((MachineInfoCfg.Ctrl_Side[0].Ctrl_Protocol == UDP_PROTOCOL)&&(MachineInfoCfg.Ctrl_Side[0].Ctrl_IP)&&(MachineInfoCfg.Ctrl_Side[0].Ctrl_Port)&&(MachineInfoCfg.Ctrl_Side[0].Device_Port))
		{return 1;}
	return 0;
}


void Init_TcpServer_Ctrl(void)
{
	P_S_Socket_Task_Info server_socket;
	unsigned char* server_buff = NULL;
	
	server_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
	server_buff = (unsigned char*)malloc(CTRL_SIDE_BUFF_SIZE);
	

	server_socket->port = MachineInfoCfg.Ctrl_Side[0].Device_Port;
	server_socket->buf_length = CTRL_SIDE_BUFF_SIZE;
	server_socket->recv_buf = server_buff;



	server_socket->socket_deal_function = Net_Ctrl_DealFunction;//协议处理函数
	socket_creat_TCP_server_task("tcpS_ctrl_task",server_socket);

	MachineInfoCfg.Ctrl_Side[0].socket_info = (void*)server_socket;
	MachineInfoCfg.Ctrl_Side[0].send_to_ctrl_side = TCPServer_Send_To_CtrlSide;

}

void Init_TcpClient_Ctrl(void)
{
	P_S_Socket_Task_Info client_socket;
	unsigned char* client_buff = NULL;
	client_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
	client_buff = (unsigned char*)malloc(CTRL_SIDE_BUFF_SIZE);

	IntToStr((MachineInfoCfg.Ctrl_Side[0].Ctrl_IP),(client_socket->target_server_ip));
	client_socket->target_server_port = MachineInfoCfg.Ctrl_Side[0].Ctrl_Port;
	client_socket->port = MachineInfoCfg.Ctrl_Side[0].Device_Port;
	client_socket->buf_length = CTRL_SIDE_BUFF_SIZE;
	client_socket->recv_buf = client_buff;

	client_socket->socket_deal_function = Net_Ctrl_DealFunction;//协议处理函数
	
	socket_creat_TCP_client_task("tcpC_ctrl_task",client_socket);
	
	MachineInfoCfg.Ctrl_Side[0].socket_info = (void*)client_socket;
	MachineInfoCfg.Ctrl_Side[0].send_to_ctrl_side = TCPClient_Send_To_CtrlSide;
}

void Init_UDP_Ctrl(void)
{
	P_S_Socket_Task_Info UDP_socket;
	unsigned char* UDP_buff = NULL;
	UDP_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
	UDP_buff = (unsigned char*)malloc(CTRL_SIDE_BUFF_SIZE);

	UDP_socket->port = MachineInfoCfg.Ctrl_Side[0].Device_Port;
	UDP_socket->buf_length = CTRL_SIDE_BUFF_SIZE;
	UDP_socket->recv_buf = UDP_buff;

	UDP_socket->socket_deal_function = Net_Ctrl_DealFunction;//协议处理函数	

	socket_creat_UDP_server_task("UDP_ctrl_task",UDP_socket);
	
	MachineInfoCfg.Ctrl_Side[0].socket_info = (void*)UDP_socket;
	MachineInfoCfg.Ctrl_Side[0].send_to_ctrl_side = UDP_Send_To_CtrlSide;
}



void Init_Ctrl_Server(void)
{
	if(CFG_EFFECT != MachineInfoCfg.Ctrl_Side[0].ctrl_effect)
		{return;}
	if(0 == Is_Ctrl_Side_Effect())
		{return;}
	switch (MachineInfoCfg.Ctrl_Side[0].Ctrl_Protocol)
		{
			case TCP_SERVER_PROTOCOL://控制端为TCPServer
				//Init_TcpServer_Ctrl();
				Init_TcpClient_Ctrl();
				break;

			case TCP_CLIENT_PROTOCOL://控制端为TCPClient
				//Init_TcpClient_Ctrl();
				Init_TcpServer_Ctrl();
				break;

			case UDP_PROTOCOL://控制端为UDP
				Init_UDP_Ctrl();
				break;

			default:
				break;
		}
}








void timer_autoupload_callback_function(void* arg)
{
	Timer_Send_Msg_To_Queue(AUTOUPLOAD_MSG,RelayQueueHandle);
}
void InitAutoUpLoad(void)
{
	unsigned short AutoUpLoadTime;
	AutoUpLoadTime = (MachineInfoCfg.SysConfig.auto_upload_time)>10? (MachineInfoCfg.SysConfig.auto_upload_time) : 60;
	AutoUpLoadTimer = register_value_into_timer(((AutoUpLoadTime)*1000), 1);
	register_callback_function_into_timer(AutoUpLoadTimer,timer_autoupload_callback_function);	
}

void Init_SysConfig(void)
{
	InitSaveData();
	init_windows();	
	init_relay_zero_cfg();
	deinit_connect_delay_time();
	
	InitTimerCounter();
	InitAutoUpLoad();
}

int BtoS(unsigned int data)//大端转小端
{
	/*unsigned int source = data;
	unsigned int res = 0;
	res |= (unsigned int)(*(unsigned char*)&source + 0);
	res <<= 8;	
	res |= (unsigned int)(*(unsigned char*)&source + 1);
	res <<= 8;	
	res |= (unsigned int)(*(unsigned char*)&source + 2);
	res <<= 8;	
	res |= (unsigned int)(*(unsigned char*)&source + 3);
	return res;*/
	return (data & 0x000000FFU) << 24 | (data & 0x0000FF00U) << 8 |   
        (data & 0x00FF0000U) >> 8 | (data & 0xFF000000U) >> 24;   
}

void SaveMachineConfig(unsigned char* recv_data)
{
	unsigned int device_ip,device_mask,device_gateway,ctrl_ip;
	U_IP res;
	unsigned char ctrl_protocol,rs232_a_baud_rate,rs232_b_baud_rate,rs485_a_baud_rate,rs485_b_baud_rate;
	unsigned short ctrl_side_port = 0;
	unsigned short ctrl_device_port = 0;
	unsigned int AutoUpLoadTime;
	device_ip = BtoS(*((unsigned int*)recv_data));	
	device_mask = BtoS(*((unsigned int*)recv_data + 1));	
	device_gateway =BtoS( *((unsigned int*)recv_data + 2));	
	ctrl_ip = BtoS(*((unsigned int*)recv_data + 3));

	AutoUpLoadTime = BtoS(*((unsigned int*)(recv_data+25)));

	ctrl_protocol = *(recv_data + 16);

	ctrl_side_port = *(recv_data + 17);
	ctrl_side_port <<= 8;
	ctrl_side_port += *(recv_data + 18);
	
	ctrl_device_port = *(recv_data + 19);
	ctrl_device_port <<= 8;
	ctrl_device_port += *(recv_data + 20);

	rs232_a_baud_rate = *(recv_data + 21);
	rs232_b_baud_rate = *(recv_data + 22);
	rs485_a_baud_rate = *(recv_data + 23);
	rs485_b_baud_rate = *(recv_data + 24);

	//res.ip_int = IPStrToInt(IntToStr(device_ip,buf));
	//res.ip_int = IPStrToInt(IntToStr(device_mask,buf));
	//res.ip_int = IPStrToInt(IntToStr(device_gateway,buf));
	//res.ip_int = IPStrToInt(IntToStr(ctrl_ip,buf));
	if(device_ip)
		{MachineInfoCfg.SysConfig.IPAddress.IP = device_ip;}
	if(device_mask)
		{MachineInfoCfg.SysConfig.IPAddress.NetMask= device_mask;}
	if(device_gateway)
		{MachineInfoCfg.SysConfig.IPAddress.GateWay= device_gateway;}
	if(ctrl_ip)
		{MachineInfoCfg.Ctrl_Side[0].Ctrl_IP= ctrl_ip;}
	if(ctrl_side_port)
		{MachineInfoCfg.Ctrl_Side[0].Ctrl_Port= ctrl_side_port;}
	if(ctrl_device_port)
		{MachineInfoCfg.Ctrl_Side[0].Device_Port= ctrl_device_port;}
	if(AutoUpLoadTime)
		{MachineInfoCfg.SysConfig.auto_upload_time = AutoUpLoadTime;}
	
	MachineInfoCfg.Ctrl_Side[0].Ctrl_Protocol = ctrl_protocol + 1;//ctrl_protocol值为0 1 2 这里加1，注意!!
	
	MachineInfoCfg.SysConfig.RS232_1_COM_cfg.BaudRate = rs232_a_baud_rate;
	MachineInfoCfg.SysConfig.RS232_2_COM_cfg.BaudRate = rs232_b_baud_rate;
	MachineInfoCfg.SysConfig.RS485_1_COM_cfg.BaudRate = rs485_a_baud_rate;
	MachineInfoCfg.SysConfig.RS485_2_COM_cfg.BaudRate = rs485_b_baud_rate;

	res.ip_int += 2;
	MachineInfoCfg.Ctrl_Side[0].ctrl_effect = CFG_EFFECT;
	Save_Data(cell_SYSConfig, (unsigned char*)&MachineInfoCfg,sizeof(MachineInfoCfg));
}

void UpLoadSys_Cfg(void)
{
	unsigned char arg[PROTOCOL_ARG_LENGTH] ={0};
	U_IP tmp;
	tmp.ip_int = MachineInfoCfg.SysConfig.IPAddress.IP;
	arg[0] = tmp.ip_array.ip_0;
	arg[1] = tmp.ip_array.ip_1;
	arg[2] = tmp.ip_array.ip_2;
	arg[3] = tmp.ip_array.ip_3;

	tmp.ip_int = MachineInfoCfg.SysConfig.IPAddress.NetMask;
	arg[4] = tmp.ip_array.ip_0;
	arg[5] = tmp.ip_array.ip_1;
	arg[6] = tmp.ip_array.ip_2;
	arg[7] = tmp.ip_array.ip_3;

	tmp.ip_int = MachineInfoCfg.SysConfig.IPAddress.GateWay;
	arg[8] = tmp.ip_array.ip_0;
	arg[9] = tmp.ip_array.ip_1;
	arg[10] = tmp.ip_array.ip_2;
	arg[11] = tmp.ip_array.ip_3;

	tmp.ip_int = MachineInfoCfg.Ctrl_Side[0].Ctrl_IP;
	arg[12] = tmp.ip_array.ip_0;
	arg[13] = tmp.ip_array.ip_1;
	arg[14] = tmp.ip_array.ip_2;
	arg[15] = tmp.ip_array.ip_3;
	
	arg[16] = MachineInfoCfg.Ctrl_Side[0].Ctrl_Protocol -1;
	arg[17] = MachineInfoCfg.Ctrl_Side[0].Ctrl_Port >> 8;
	arg[18] = MachineInfoCfg.Ctrl_Side[0].Ctrl_Port>>0;
	arg[19] = MachineInfoCfg.Ctrl_Side[0].Device_Port >> 8;
	arg[20] = MachineInfoCfg.Ctrl_Side[0].Device_Port >> 0;
	
	arg[21] = MachineInfoCfg.SysConfig.RS232_1_COM_cfg.BaudRate;
	arg[22] = MachineInfoCfg.SysConfig.RS232_2_COM_cfg.BaudRate; 
	arg[23] = MachineInfoCfg.SysConfig.RS485_1_COM_cfg.BaudRate; 
	arg[24] = MachineInfoCfg.SysConfig.RS485_2_COM_cfg.BaudRate;	
	
	arg[25] = MachineInfoCfg.SysConfig.auto_upload_time >>24;
	arg[26] = MachineInfoCfg.SysConfig.auto_upload_time >>16; 
	arg[27] = MachineInfoCfg.SysConfig.auto_upload_time >>8; 
	arg[28] = MachineInfoCfg.SysConfig.auto_upload_time >>0;	
	UploadData(UPLOAD_SYS_CFG,arg);
}



void SetUart(UART_HandleTypeDef* set_uart,unsigned char b_r)
{
	switch(b_r)
		{
			case BAUD_RATE_4800:
				set_uart->Init.BaudRate = 4800;
				break;
			case BAUD_RATE_9600:
				set_uart->Init.BaudRate = 9600;
				break;
			case BAUD_RATE_19200:
				set_uart->Init.BaudRate = 19200;
				break;
			case BAUD_RATE_57600:
				set_uart->Init.BaudRate = 57600;
				break;
			case BAUD_RATE_115200:
				set_uart->Init.BaudRate = 115200;
				break;
		}
	if (HAL_UART_Init(set_uart) != HAL_OK)
		{
		Error_Handler();
		}
}


