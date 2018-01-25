#include "serial_server.h"
void InitSerialServer(void)
{
	P_S_Socket_Task_Info server_socket;
	server_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
	
	server_socket->this_info = server_socket;
	

	server_socket->port = 5566;
	socket_creat_TCP_server_task("server_task_1",server_socket);

	osDelay(1000);
	P_S_Socket_Task_Info server_socket2;
	server_socket2 = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));

	server_socket2->port = 1111;
	socket_creat_TCP_server_task("server_task_2",server_socket2);
}




