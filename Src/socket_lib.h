#ifndef __SOCKET_LIB_H__
#define __SOCKET_LIB_H__
////MEMP_NUM_NETCONN

#include "include.h"
#include "sockets.h"

#define SOCKET_OFFSET 0X10

typedef struct
{
	unsigned char* link_buf;
	unsigned short link_buf_length;
	unsigned short recv_length;
}S_Socket_Link_Info,*P_S_Socket_Link_Info;


typedef void (*P_Socket_Deal_Function)(void*);



#define MAX_CLIENT_NUM		8
#define CLIENT_RECV_BUFF_LENGTH	400
typedef struct
{
	unsigned int connct_flag;
	unsigned int cur_operat_client_fd;

	osThreadId client_task_handle[MAX_CLIENT_NUM];//������
	
	void* p_task_info;//������
	void* task_info[MAX_CLIENT_NUM];//������Ϣ	
	
	unsigned int client_fd[MAX_CLIENT_NUM];
	
	unsigned char* recv_buf[MAX_CLIENT_NUM];
	unsigned int client_recv_length[MAX_CLIENT_NUM];
	//unsigned int recv_buf_length;
	//P_Socket_Deal_Function client_deal_function[MAX_CLIENT_NUM];//���մ�����
}S_ConnectClient,*P_S_ConnectClient;


typedef struct
{
	unsigned char recv_ok;
	unsigned char is_connected;
	unsigned char needDISCONNECT;
	unsigned int socket_num;//������socket            clientģʽʱ�����socket�ظ���Ϣ
	/*����socketʱ��0��ʼ��������ֹ����ʧ��ʱ���ֵ����0�п��ܷ������벻�������
	�����ڱ���socket��ֵʱ  ������SOCKET_OFFSET ��ʹ�õ�ʱ���ȥ���ֵ*/


	
	unsigned int client_id;//accept�ͻ��˵�socket          serverģʽʱ�����socket�ظ���Ϣ
	S_ConnectClient client;

	
	char target_server_ip[18];//Ŀ��
	unsigned int target_server_port;

	unsigned int port;
	
	unsigned int recv_length;
	
	unsigned int buf_length;
	unsigned char* recv_buf;

	P_Socket_Deal_Function socket_deal_function;//���մ�����
		
	osMessageQId task_signal;//�յ����ݺ�֪ͨ�Ķ��о��
	osThreadId task_handle;//������������	

	struct sockaddr_in UDP_client_address;

	
	osMessageQDef_t task_queue;
	osThreadDef_t task;
	struct S_Socket_Task_Info* this_info;
}S_Socket_Task_Info,*P_S_Socket_Task_Info;

typedef struct sockaddr_in* P_sockaddr_in;

/*void socket_creat_client(char* task_name,P_S_Socket_Task_Info task_info);
void socket_tcp_client_task(void const * argument);
*/

void socket_tcp_server_task(void const * argument);
osThreadId socket_creat_TCP_client_task(char* task_name,P_S_Socket_Task_Info task_info);
osThreadId socket_creat_TCP_server_task(char* task_name,P_S_Socket_Task_Info task_info);
osThreadId socket_creat_UDP_server_task(char* task_name,P_S_Socket_Task_Info task_info);
void Set_UDP_Target_Address(char* ip,unsigned short port,P_sockaddr_in TargetAddress);


osThreadId socket_creat_TCP_server_task(char* task_name,P_S_Socket_Task_Info task_info);

osThreadId socket_creat_Concurrency_TCP_server_task(char* task_name,P_S_Socket_Task_Info task_info);
#endif

