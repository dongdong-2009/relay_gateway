#include "socket_lib.h"
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/ip_addr.h"
//#include "lwip/ip6_addr.h"
#include "lwip/sockets.h"
//#include "include.h"

/*
  osThreadDef(system, systrm_task, osPriorityNormal, 0, 200);
  systemHandle = osThreadCreate(osThread(system), NULL);

  
  osMessageQDef(serial_queue, 8, uint8_t);
  serial_queueHandle = osMessageCreate(osMessageQ(serial_queue), NULL);

  */
unsigned int g_socket_num;


void socket_tcp_client_task(void const * argument)
{
  /* USER CODE BEGIN socket_server_task */
  	save_task_info();
	//save_task_info();
  	P_S_Socket_Task_Info task_info = (P_S_Socket_Task_Info)argument;
	unsigned char TCP_Client_Default_RecvBuf[100]; //TCP客户端接收数据缓冲区
	if(task_info->recv_buf == NULL)
		{
			task_info->recv_buf = TCP_Client_Default_RecvBuf;
			task_info->buf_length = 100;
		}

	
	start_client:		
	task_info->is_connected = 0;
	osDelay(1000);	
	int ret;
	struct sockaddr_in local_addr;
	int s;  
	local_addr.sin_addr.s_addr = inet_addr(task_info->target_server_ip);
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(task_info->target_server_port);
	s=socket(AF_INET,SOCK_STREAM,0); 
	//printf("client socket number = %d\r\n",s);
	if(-1 == s)
		{
			//printf("creat socket error!\r\n");
		}
	else
		{
			//printf("socket number = %d\r\n",s);
			task_info->socket_num = s+SOCKET_OFFSET;
		}
	ret=connect(s,(struct sockaddr*)&local_addr,sizeof(local_addr));
	task_info->is_connected = 1;
  /* Infinite loop */
  for(;;)
  {
	
	ret=recv(s,task_info->recv_buf,task_info->buf_length,0);
	task_info->recv_length = ret;
	if(ret > 0)
		{
			if(task_info->socket_deal_function)
				{
					task_info->socket_deal_function((void*)(task_info));
				}
			else
				{
					send(s,task_info->recv_buf,ret,0);
				}
		}
	if(ret==-1)
		{
			close(s);
			osDelay(100);
			goto start_client;
		}
	if(ret == 0)
		{
			close(s);
			goto start_client;
		}
  }
}


void socket_tcp_server_task(void const * argument)
{
	save_task_info();
  	P_S_Socket_Task_Info task_info = (P_S_Socket_Task_Info)argument;
	int s;
	int ret;
	int server_len,socklen;
	int client_fd;
	struct sockaddr_in cli_sockaddr; 
	unsigned char TCP_Server_Default_RecvBuf[100] = {0};
	
	if(task_info->recv_buf == NULL)
		{
			task_info->recv_buf = TCP_Server_Default_RecvBuf;
			task_info->buf_length = 100;
		}
	if(task_info->port == 0)
		{
			task_info->port = 3630;
		}
	
	struct sockaddr_in server_sockaddr;  

	
	start_server:	
	task_info->is_connected = 0;
	osDelay(1000);
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));  
	server_sockaddr.sin_family = AF_INET;  
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sockaddr.sin_port = htons(task_info->port);  
	server_len = sizeof(server_sockaddr);
	
	s = socket(AF_INET, SOCK_STREAM, 0); 
	if(-1 == ret)
		{
			//printf("creat socket error!\r\n");
		}
	else
		{
			//printf("socket number = %d\r\n",s);
			task_info->socket_num = s+SOCKET_OFFSET;
		}
  
	
	ret = bind(s, (struct sockaddr *) &server_sockaddr, server_len);
	if(-1 == ret)
		{
			//printf("bind error!\r\n");
			close(s);		
			goto start_server;
		}
	ret = listen(s, 2); 
	if(-1 == ret)
		{
			//printf("listen error!\r\n");
			close(s);	
			goto start_server;	
		}
	accept_client:
	socklen = sizeof(cli_sockaddr);	
	task_info->is_connected = 0;
	client_fd = accept(s, (struct sockaddr *) &cli_sockaddr, (socklen_t*)&socklen);	
	task_info->is_connected = 1;
	if(-1 == client_fd)
		{
			//printf("accept error!\r\n");
			close(s);		
			goto start_server;;		
		}
	else
		{
			//printf("client_id = %d\r\n",client_fd);
			task_info->client_id = client_fd;
		}
	
	 for(;;)
	{	
		ret=read(client_fd,task_info->recv_buf,task_info->buf_length);
		task_info->recv_length = ret;
		if(ret > 0)
			{
				//g_socket_num = task_info->socket_num;
				if(task_info->socket_deal_function)
					{
						task_info->socket_deal_function((void*)(task_info));
						if(task_info->needDISCONNECT == 0X36)
							{
								//shutdown(s,2);					
								//shutdown(client_fd,2);
								//close(s);					
								close(client_fd);
								goto accept_client;
								
							}
					}
				else
					{
						write(client_fd,task_info->recv_buf,ret);
					}
				task_info->recv_ok = 1;
			}
		if(ret==-1)
			{
				close(s);					
				close(client_fd);
				goto start_server;
			}
		if(ret == 0)
			{
			
				close(s);	
				//shutdown(client_fd,2);
				close(client_fd);		
				goto start_server;
			}
	 	}

}

unsigned char malloc_client_space(int client_fd,P_S_Socket_Task_Info task_info)
{
	unsigned char loopx = 0;
	for(;loopx < MAX_CLIENT_NUM; loopx ++)
		{
			if((task_info->client.connct_flag) & (1<<loopx))
				{
					continue;
				}
			else
				{
					task_info->client.connct_flag |= (1<<loopx);
					task_info->client.cur_operat_client_fd = client_fd;
					task_info->client.client_fd[loopx] = client_fd;
					task_info->client.recv_buf[loopx] = (unsigned char*)malloc(CLIENT_RECV_BUFF_LENGTH);
					task_info->client.task_info[loopx] = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
					return loopx;
				}
		}
	return 0XFF;
}

unsigned char free_client_space(int client_fd,P_S_Socket_Task_Info task_info)
{
	unsigned char loopx = 0;
	for(;loopx < MAX_CLIENT_NUM; loopx ++)
		{
			if((task_info->client.connct_flag) & (1<<loopx))
				{
					if(task_info->client.client_fd[loopx] == client_fd)
						{
							task_info->client.connct_flag &= (~(1<<loopx));
							free(task_info->client.recv_buf[loopx]);
							free(task_info->client.task_info[loopx]);
							return loopx;
						}
				}
			else
				{

				}
		}
	return 0XFF;
}
unsigned char get_client_fd_space(unsigned int client_fd,P_S_Socket_Task_Info task_info)
{
	unsigned char loopx = 0;
	for(;loopx < MAX_CLIENT_NUM; loopx ++)
		{
			if((task_info->client.connct_flag) & (1<<loopx))
				{
					if(task_info->client.client_fd[loopx] == client_fd)
						{
							return loopx;
						}
				}
			else
				{

				}
		}
	return 0XFF;
}

void client_thread(void const * arg)
{
	unsigned char client_speace;
	int client_fd;
	int ret = 0;
	P_S_Socket_Task_Info task_info = (P_S_Socket_Task_Info)arg;
	client_speace = get_client_fd_space(task_info->client.cur_operat_client_fd,task_info);
	if(0XFF == client_speace)
		{
			//error
		}
	client_fd = task_info->client.client_fd[client_speace];
	while(1)
		{	
		ret=read(client_fd,task_info->client.recv_buf[client_speace],CLIENT_RECV_BUFF_LENGTH);
		task_info->client.client_recv_length[client_speace]= ret;
		if(ret > 0)
			{
				//g_socket_num = task_info->socket_num;
				if(task_info->socket_deal_function)
					{
						task_info->socket_deal_function((void*)(task_info));
						if(task_info->needDISCONNECT == 0X36)
							{
								//shutdown(s,2);					
								//shutdown(client_fd,2);
								close(client_fd);	
								free_client_space(client_fd, task_info);
								vTaskDelete(task_info->client.client_task_handle[client_speace]);								
							}
					}
				else
					{
						write(client_fd,task_info->client.recv_buf[client_speace],ret);
					}
				task_info->recv_ok = 1;
			}
		if(ret==-1)
			{
				close(client_fd);
				free_client_space(client_fd, task_info);			
				vTaskDelete(task_info->client.client_task_handle[client_speace]);	
			}
		if(ret == 0)
			{			
				close(client_fd);		
				free_client_space(client_fd, task_info);		
				vTaskDelete(task_info->client.client_task_handle[client_speace]);
			}
	 	}
	
}

/*void client_prethread(int client_fd,P_S_Socket_Task_Info task_info)
{
	int ret = 0;
	unsigned char client_speace;
	if(-1 == client_fd)
		{
			return;
		}
	else
		{
			client_speace = malloc_client_space(client_fd,task_info);
			if(0XFF == client_speace)
				{
					return;
				}
		}
	
	 for(;;)
	{	
		ret=read(client_fd,task_info->client.recv_buf[client_speace],CLIENT_RECV_BUFF_LENGTH);
		task_info->client.client_recv_length[client_speace]= ret;
		if(ret > 0)
			{
				//g_socket_num = task_info->socket_num;
				if(task_info->socket_deal_function)
					{
						task_info->socket_deal_function((void*)(task_info));
						if(task_info->needDISCONNECT == 0X36)
							{
								//shutdown(s,2);					
								//shutdown(client_fd,2);
								close(client_fd);			
								vTaskDelete(task_info->client.client_task_handle[client_speace]);								
							}
					}
				else
					{
						write(client_fd,task_info->client.recv_buf[client_speace],ret);
					}
				task_info->recv_ok = 1;
			}
		if(ret==-1)
			{
				close(client_fd);			
				vTaskDelete(task_info->client.client_task_handle[client_speace]);	
			}
		if(ret == 0)
			{			
				close(client_fd);			
				vTaskDelete(task_info->client.client_task_handle[client_speace]);
			}
	 	}
}*/

osThreadId start_client_prethread(int client_fd,P_S_Socket_Task_Info task_info)
{
	unsigned char client_speace;
	P_S_Socket_Task_Info tmp_task_info = NULL;
	char client_thread_name[30] = {0};
	sprintf(client_thread_name,"ClientTsak%d",client_fd);
	if(-1 == client_fd)
		{
			return NULL;
		}
	else
		{
			client_speace = malloc_client_space(client_fd,task_info);
			if(0XFF == client_speace)
				{
					return NULL;
				}
		}

	//task_info->task.pthread = socket_tcp_client_task;
	task_info->client.p_task_info = task_info;
	tmp_task_info =  (P_S_Socket_Task_Info)(task_info->client.task_info);

	tmp_task_info->task.pthread = client_thread;
	tmp_task_info->task.stacksize = 200;
	tmp_task_info->task.instances = 0;
	tmp_task_info->task.name = client_thread_name;
	tmp_task_info->task.tpriority = osPriorityAboveNormal;	
	//tmp_task_info->task_handle = osThreadCreate(&(task_info->task),(void*)task_info);
	task_info->client.client_task_handle[client_speace]= osThreadCreate(&(tmp_task_info->task),(void*)task_info);
	return task_info->client.client_task_handle[client_speace];
	
}

void socket_tcp_Concurrency_server_task(void const * argument)//并发
{
	save_task_info();
  	P_S_Socket_Task_Info task_info = (P_S_Socket_Task_Info)argument;
	int s;
	int ret;
	int server_len,socklen;
	int client_fd;
	struct sockaddr_in cli_sockaddr; 
	unsigned char TCP_Server_Default_RecvBuf[100] = {0};
	
	if(task_info->recv_buf == NULL)
		{
			task_info->recv_buf = TCP_Server_Default_RecvBuf;
			task_info->buf_length = 100;
		}
	if(task_info->port == 0)
		{
			task_info->port = 3630;
		}
	
	struct sockaddr_in server_sockaddr;  

	
	start_server:	
	task_info->is_connected = 0;
	osDelay(1000);
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));  
	server_sockaddr.sin_family = AF_INET;  
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sockaddr.sin_port = htons(task_info->port);  
	server_len = sizeof(server_sockaddr);
	
	s = socket(AF_INET, SOCK_STREAM, 0); 
	if(-1 == ret)
		{
			//printf("creat socket error!\r\n");
		}
	else
		{
			//printf("socket number = %d\r\n",s);
			task_info->socket_num = s+SOCKET_OFFSET;
		}
  
	
	ret = bind(s, (struct sockaddr *) &server_sockaddr, server_len);
	if(-1 == ret)
		{
			//printf("bind error!\r\n");
			close(s);		
			goto start_server;
		}
	ret = listen(s, 2); 
	if(-1 == ret)
		{
			//printf("listen error!\r\n");
			close(s);	
			goto start_server;	
		}
	//accept_client:
		while(1)
			{
				socklen = sizeof(cli_sockaddr);	
				client_fd = accept(s, (struct sockaddr *) &cli_sockaddr, (socklen_t*)&socklen);	
				start_client_prethread(client_fd,task_info);
			}
}



void socket_udp_server_task(void const * argument)
{	
  	save_task_info();
  	P_S_Socket_Task_Info task_info = (P_S_Socket_Task_Info)argument;
	
	int s;
	int ret;
	int server_len;
	struct sockaddr_in cli_sockaddr = {0}; 
	struct sockaddr_in server_sockaddr;  
	socklen_t client_length = sizeof(cli_sockaddr);
	unsigned char UDP_Server_Default_RecvBuf[100] = {0};
	
	if(task_info->recv_buf == NULL)
		{
			task_info->recv_buf = UDP_Server_Default_RecvBuf;
			task_info->buf_length = 100;
		}
	if(task_info->port == 0)
		{
			task_info->port = 3630;
		}

	Start_Udp:		
	task_info->is_connected = 0;		
	osDelay(1000);
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));  
	server_sockaddr.sin_family = AF_INET;  
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sockaddr.sin_port = htons(task_info->port);  
	server_len = sizeof(server_sockaddr);
	
	s = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == s)
		{
			//printf("creat socket error\r\n");
			goto Start_Udp;
		}
	else
		{
			//printf("socket number = %d\r\n",s);
			task_info->socket_num = s+SOCKET_OFFSET;
		}
	if(-1 == bind(s,(struct sockaddr *)&server_sockaddr,server_len))
		{
			//printf("bind IP error \r\n");
			close(s);		
			goto Start_Udp;
		}	
	task_info->is_connected = 1;

	while(1)
		{
			memset(&cli_sockaddr,0,sizeof(cli_sockaddr));
			//ret = recvfrom(s,(task_info->recv_buf),(task_info->buf_length),0,(struct sockaddr *)&cli_sockaddr,&client_length);
			ret = recvfrom(s,(task_info->recv_buf),(task_info->buf_length),0,(struct sockaddr *)&(task_info->UDP_client_address),&client_length);
			task_info->recv_length = ret;
			if(ret > 0)
				{
					if(task_info->socket_deal_function)
						{
							task_info->socket_deal_function((void*)(task_info));
						}
					else
						{
							//write(client_fd,task_info->recv_buf,ret);							  
							server_len = sizeof(server_sockaddr);
							Set_UDP_Target_Address("192.168.1.255",8877,&cli_sockaddr);
							sendto(s,(task_info->recv_buf),(task_info->recv_length),0,(struct sockaddr *)&cli_sockaddr,client_length);
						}
					task_info->recv_ok = 1;
				}
			if(ret==-1)
				{
					close(s);	
					goto Start_Udp;
				}
			if(ret == 0)
				{
				
					close(s);		
					goto Start_Udp;
				}
			
		}
	
}

osThreadId socket_creat_TCP_client_task(char* task_name,P_S_Socket_Task_Info task_info)
{
	if(task_info->task_signal == NULL)//任务消息队列不存在时新建一个队列，保存在任务信息里
		{
			/*如果需要通知队列就在这里加上*/
			//task_info->task_queue.item_sz = 4;
			//task_info->task_queue.queue_sz =5;	
			//task_info->task_signal= osMessageCreate(&(task_info->task_queue), NULL);		
		}
	
	//task_info->recv_buf = recv_buf;//务必确定接受缓存合法!!
	task_info->task.pthread = socket_tcp_client_task;
	if(0 == task_info->task.stacksize)
		{
			task_info->task.stacksize = 200;
		}
	task_info->task.instances = 0;
	task_info->task.name = task_name;
	task_info->task.tpriority = osPriorityAboveNormal;	
	task_info->task_handle = osThreadCreate(&(task_info->task),(void*)task_info);
	return task_info->task_handle;
}


osThreadId socket_creat_TCP_server_task(char* task_name,P_S_Socket_Task_Info task_info)
{
	if(task_info->task_signal == NULL)//任务消息队列不存在时新建一个队列，保存在任务信息里
		{
			/*如果需要通知队列就在这里加上*/
			//task_info->task_queue.item_sz = 4;
			//task_info->task_queue.queue_sz =5;	
			//task_info->task_signal= osMessageCreate(&(task_info->task_queue), NULL);		
		}
	
	//task_info->recv_buf = recv_buf;//务必确定接受缓存合法!!
	task_info->task.pthread = socket_tcp_server_task;
	if(0 == task_info->task.stacksize)
		{
			task_info->task.stacksize = 200;
		}
	task_info->task.instances = 0;
	task_info->task.name = task_name;
	task_info->task.tpriority = osPriorityAboveNormal;	
	task_info->task_handle = osThreadCreate(&(task_info->task),(void*)task_info);
	return task_info->task_handle;
}


osThreadId socket_creat_Concurrency_TCP_server_task(char* task_name,P_S_Socket_Task_Info task_info)
{
	if(task_info->task_signal == NULL)//任务消息队列不存在时新建一个队列，保存在任务信息里
		{
			/*如果需要通知队列就在这里加上*/
			//task_info->task_queue.item_sz = 4;
			//task_info->task_queue.queue_sz =5;	
			//task_info->task_signal= osMessageCreate(&(task_info->task_queue), NULL);		
		}
	
	//task_info->recv_buf = recv_buf;//务必确定接受缓存合法!!
	task_info->task.pthread = socket_tcp_Concurrency_server_task;
	if(0 == task_info->task.stacksize)
		{
			task_info->task.stacksize = 200;
		}
	task_info->task.instances = 0;
	task_info->task.name = task_name;
	task_info->task.tpriority = osPriorityAboveNormal;	
	task_info->task_handle = osThreadCreate(&(task_info->task),(void*)task_info);
	return task_info->task_handle;
}



osThreadId socket_creat_UDP_server_task(char* task_name,P_S_Socket_Task_Info task_info)
{
	if(task_info->task_signal == NULL)//任务消息队列不存在时新建一个队列，保存在任务信息里
		{
			/*如果需要通知队列就在这里加上*/
			//task_info->task_queue.item_sz = 4;
			//task_info->task_queue.queue_sz =5;	
			//task_info->task_signal= osMessageCreate(&(task_info->task_queue), NULL);		
		}
	
	//task_info->recv_buf = recv_buf;//务必确定接受缓存合法!!
	task_info->task.pthread = socket_udp_server_task;
	if(0 == task_info->task.stacksize)
		{
			task_info->task.stacksize = 200;
		}
	task_info->task.instances = 0;
	task_info->task.name = task_name;
	task_info->task.tpriority = osPriorityAboveNormal;	
	task_info->task_handle = osThreadCreate(&(task_info->task),(void*)task_info);
	return task_info->task_handle;
}

typedef struct sockaddr_in* P_sockaddr_in;

void Set_UDP_Target_Address(char* ip,unsigned short port,P_sockaddr_in TargetAddress)
{
	TargetAddress->sin_len = 0X10;
	TargetAddress->sin_family = AF_INET;
	TargetAddress->sin_port = htons(port);
	TargetAddress->sin_addr.s_addr = inet_addr(ip);
}


void socket_deal_function_demo(void* arg)
{
	P_S_Socket_Task_Info task_info = 0;
	task_info = (P_S_Socket_Task_Info)arg;
	char tmp_buf[200] = {0};
	sprintf(tmp_buf,"recv data:");
	send(task_info->socket_num - SOCKET_OFFSET,tmp_buf,10,0);
	
	send(task_info->socket_num - SOCKET_OFFSET,task_info->recv_buf,task_info->recv_length,0);
	
	sprintf(tmp_buf,"\r\n");
	send(task_info->socket_num - SOCKET_OFFSET,tmp_buf,2,0);
	return;
}


