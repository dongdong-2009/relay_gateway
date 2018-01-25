#include "webserver.h"
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/ip_addr.h"
//#include "lwip/ip6_addr.h"
#include "lwip/sockets.h"
#include "include.h"

unsigned char WebRxBuff[WEBSERVERRXBUFFSIZE] = {0};
unsigned char WebTxBuff[WEBSERVERTXBUFFSIZE] = {0};
CONFIG_MSG  ConfigMsg;
int login_flag = 0;
int ip_address = 0;



static void make_basic_config_setting_json_callback(int8* buf, CONFIG_MSG config_msg)
{
  sprintf(buf,"settingsCallback({\"ver\":\"%d.%d\",\
                \"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\
                \"machine_ip\":\"%d.%d.%d.%d\",\
                \"machine_gateway\":\"%d.%d.%d.%d\",\
                \"MQTT_dy\":\"%d.%d.%d.%d\",\
								\"dns\":\"%d.%d.%d.%d\",\
                });",config_msg.sw_ver[0],config_msg.sw_ver[1],
                config_msg.mac[0],config_msg.mac[1],config_msg.mac[2],config_msg.mac[3],config_msg.mac[4],config_msg.mac[5],
                config_msg.lip[0],config_msg.lip[1],config_msg.lip[2],config_msg.lip[3],
                config_msg.gw[0],config_msg.gw[1],config_msg.gw[2],config_msg.gw[3],
                config_msg.sub[0],config_msg.sub[1],config_msg.sub[2],config_msg.sub[3],
								config_msg.dns[0],config_msg.dns[1],config_msg.dns[2],config_msg.dns[3]
                );
}

void make_cgi_response(uint16 delay, int8* url,int8* cgi_response_buf)
{
  sprintf(cgi_response_buf,"<html><head><title>iWeb - Configuration</title><script language=javascript>j=%d;function func(){document.getElementById('delay').innerText=' '+j + ' ';j--;setTimeout('func()',1000);if(j==0)location.href='http://%d.%d.%d.%d/';}</script></head><body onload='func()'>please wait for a while, the module will boot in<span style='color:red;' id='delay'></span> seconds.</body></html>",delay,url[0],url[1],url[2],url[3]);
  return;
}

void mid(int8* src, int8* s1, int8* s2, int8* sub)
{
	int8* sub1;
	int8* sub2;
	uint16 n;

  sub1=strstr(src,s1);
  sub1+=strlen(s1);
  sub2=strstr(sub1,s2);
  n=sub2-sub1;
  strncpy(sub,sub1,n);
  sub[n]=0;
}

void make_http_response_head(unsigned char * buf,char type,long len)
{
	char * head;
	char tmp[10];
	//20120504
	memset(buf,0x00,MAX_URI_SIZE); 
	//printf("make http response\r\n");
	//
	/* 文件类型*/
	if 	(type == PTYPE_HTML) head = RES_HTMLHEAD_OK;
	else if (type == PTYPE_GIF)	head = RES_GIFHEAD_OK;
	else if (type == PTYPE_TEXT)	head = RES_TEXTHEAD_OK;
	else if (type == PTYPE_JPEG)	head = RES_JPEGHEAD_OK;
	else if (type == PTYPE_FLASH)	head = RES_FLASHHEAD_OK;
	else if (type == PTYPE_MPEG)	head = RES_MPEGHEAD_OK;
	else if (type == PTYPE_PDF)	head = RES_PDFHEAD_OK;

	sprintf(tmp,"%ld", len);	
	strcpy((char*)buf, head);
	strcat((char*)buf, tmp);
	strcat((char*)buf, "\r\n\r\n");
	//printf("%s\r\n", buf);
}

void parse_http_request(st_http_request * request,unsigned char * buf)
{
	char * nexttok;
	nexttok = strtok((char*)buf," ");
	if(!nexttok)
		{
			request->METHOD = METHOD_ERR;
			return;
		}
	if(!strcmp(nexttok, "GET") || !strcmp(nexttok,"get"))
		{
			request->METHOD = METHOD_GET;
			nexttok = strtok(NULL," ");   			
		}
	else if (!strcmp(nexttok, "HEAD") || !strcmp(nexttok,"head"))	
		{
			request->METHOD = METHOD_HEAD;
			nexttok = strtok(NULL," ");  		
		}
	else if (!strcmp(nexttok, "POST") || !strcmp(nexttok,"post"))
		{
			nexttok = strtok(NULL,"\0");//20120316
			//nexttok = strtok(NULL," ");
			request->METHOD = METHOD_POST;		
		}
	else
		{
			request->METHOD = METHOD_ERR;
		}	 
	if(!nexttok)
		{
			request->METHOD = METHOD_ERR;  			
			return;
		}
	strcpy(request->URI,nexttok);
}


void SendHtml(P_S_Socket_Task_Info task_info,const char* html)
{
	unsigned int file_len;
	file_len = strlen(html);
	memset(WebRxBuff,0,WEBSERVERRXBUFFSIZE);
	memset(WebTxBuff,0,WEBSERVERRXBUFFSIZE);
	make_http_response_head((uint8*)WebRxBuff, PTYPE_HTML,file_len);
	send(task_info->client_id,WebRxBuff,strlen((char const*)WebRxBuff),0);
	send(task_info->client_id, (uint8 *)html, strlen(html),0);
}

void WebCGI(void* arg)
{	
	unsigned int usrname = 0;
	//unsigned int pwd = 0;
	unsigned char* str_pwd = NULL;
	P_S_Socket_Task_Info task_info = 0;
	task_info = (P_S_Socket_Task_Info)arg;
	char tmp[30] ={0};
	
	st_http_request *http_request;
	http_request = (st_http_request*)WebRxBuff;


	int8* name; //get method request file name
	int8 req_name[32]={0x00}; //post method request file name
	unsigned long file_len=0;
	//uint16 send_len=0;
	uint8* http_response;
	memset(WebTxBuff,0x00,WEBSERVERTXBUFFSIZE);
	http_response = (uint8*)WebRxBuff;
	http_request = (st_http_request*)WebTxBuff;
	parse_http_request(http_request, WebRxBuff);    // After analyze request, convert into http_request
	//method Analyze
	switch (http_request->METHOD)		
		{
		case METHOD_ERR :
			memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
			send(task_info->client_id, (uint8 *)http_response, strlen((int8 const*)http_response),0);
			break;
		case METHOD_HEAD:
		case METHOD_GET:
			//get file name from uri
			name = http_request->URI;

			if(strcmp(name,"/index.htm")==0 || strcmp(name,"/")==0 || (strcmp(name,"/index.html")==0))
				{
					file_len = strlen(LOGIN_ARRARY);
					//file_len = strlen(INDEX_HTML);
					make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
					send(task_info->client_id,http_response,strlen((char const*)http_response),0);
					send(task_info->client_id, (uint8 *)LOGIN_ARRARY, strlen(LOGIN_ARRARY),0);
					//send(task_info->client_id, (uint8 *)INDEX_HTML+send_len, strlen(INDEX_HTML),0);	
					task_info->needDISCONNECT = 0X36;				
				}

			if(strcmp(name,"/demo")==0)
				{
					//SendHtml(task_info,INDEX_HTML);				
				}
			else if(strcmp(name,"/w5500.js")==0)
				{
					memset(WebTxBuff,0,WEBSERVERTXBUFFSIZE);
					make_basic_config_setting_json_callback((int8*)WebTxBuff,ConfigMsg);
					sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen((const char*)WebTxBuff),WebTxBuff);
					send(task_info->client_id, (unsigned char *)http_response, strlen((char const*)http_response),0);
				}
			
			break;
			/*POST method*/
		case METHOD_POST:

			//get file name from uri
			mid(http_request->URI, "/", " ", req_name);

			if(strcmp(req_name,"config.cgi")==0)
			{
				//cgi_ipconfig((copy_st_http_request*)http_request);
				
				ip_address = GetIPV4AddressFromString("machine_ip",(char*)WebRxBuff);
				IntToStr(ip_address,tmp);
				ip_address = GetIPV4AddressFromString("machine_ip",tmp);
				make_cgi_response(5,(char*)ConfigMsg.lip,(char*)WebTxBuff);         
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen((const char*)WebTxBuff),WebTxBuff);
				send(task_info->client_id, (unsigned char *)http_response, strlen((char *)http_response),0);	
				//disconnect(s);	
				task_info->needDISCONNECT = 0X36;
				return;
			}

			if(strcmp(req_name,"sign_in.cgi")==0)
			{
				usrname = GetNumFromString("username",(char*)WebTxBuff);
				GetStringFromString("password",(char*)WebTxBuff,(char**)(&str_pwd));
				//if((usrname == 123)&&(str_pwd))
					if((usrname == 123)&&(my_str_cmp(str_pwd,"admin",sizeof("admin"))))
					{
						/*file_len = strlen(OUTDOOR_HTML);
						make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
						send(task_info->client_id,http_response,strlen((char const*)http_response),0);
						send_len=0;
						send(task_info->client_id, (uint8 *)OUTDOOR_HTML+send_len, strlen(OUTDOOR_HTML),0);*/

						SendHtml(task_info,OUTDOOR_ARRARY);
					}
				else
					{						
						/*file_len = strlen(LOGIN_HTML);
						make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
						send(task_info->client_id,http_response,strlen((char const*)http_response),0);
						send_len=0;
						send(task_info->client_id, (uint8 *)LOGIN_HTML+send_len, strlen(LOGIN_HTML),0);*/

						SendHtml(task_info,LOGIN_ARRARY);
					}
				task_info->needDISCONNECT = 0X36;
				return;
			}
		break;
		
		default :
		break;
		}

	
}

void InitHttpServer(void)
{	
  P_S_Socket_Task_Info server_socket;
  server_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
  server_socket->port = 80;
  //server_socket->task_signal = socket_queueHandle;//通知队列
  server_socket->recv_buf = WebRxBuff;
  server_socket->buf_length = WEBSERVERRXBUFFSIZE;
  server_socket->task.stacksize = 1024;
  server_socket->socket_deal_function = WebCGI;
  socket_creat_TCP_server_task("web_server",server_socket);
}






/**************************  DeCode   ******************************/
unsigned int string_length(char* string)
{
	char* tmp = string;
	unsigned int length = 0;
	while(*tmp++)
		{
			length ++; 
		}
	return length;
}

unsigned int pow10(unsigned char data)
{
	unsigned int return_num = 1;
	unsigned char loopx = data;
	while(loopx --)
		{
			return_num *= 10;
		}
	return return_num;
}


unsigned int str_to_int(char* str)
{
	unsigned char loopx,change_loop;
	unsigned char int_num[20] = {0};
	unsigned int return_num = 0;
	unsigned char str_num;
	char* tmp = str;
	str_num = string_length(tmp);
	for(loopx = 0; loopx < str_num ; loopx++)
		{
			if((*tmp >= '0')&&(*tmp <='9'))
				{
					int_num[loopx] = *(tmp++) - '0';
				}
			else
				{
					break;
				}
		}
	for(change_loop = 0; change_loop < loopx ;change_loop ++)
		{
			return_num += int_num[change_loop]*pow10(loopx - change_loop -1);
		}
	return return_num;
}

unsigned int GetStringOffset(char* name,char* SourceString)
{
	unsigned int name_length = 0;
	unsigned int SourceString_length = 0;
	unsigned int loopinside = 0;
	unsigned int loopoutside = 0;
	name_length = string_length(name);
	//SourceString_length = string_length(SourceString);
	SourceString_length = WEBSERVERRXBUFFSIZE;
	for(loopoutside = 0;loopoutside < SourceString_length; loopoutside ++)
		{
			for(loopinside = 0;loopinside < name_length; loopinside++)
				{
					if(*(name+loopinside) != *(SourceString+loopoutside+loopinside))
						{
							break;
						}
					else
						{
							continue;
						}
				}
			if(loopinside == name_length)
				{
					return loopoutside;
				}
		}
	return 0XFFFFFFFF;	
}

unsigned int GetNumFromString(char* name,char* String)
{
	//char* target_value = NULL;
	unsigned int name_length = 0;
	name_length = string_length(name);
	unsigned int string_offset = 0;
	string_offset = GetStringOffset(name,String);
	if(0XFFFFFFFF == string_offset)
		{
			return 0XFFFFFFFF;
		}
	return str_to_int(String+string_offset+name_length +1);
}


unsigned int GetIPV4AddressFromString(char* name,char* String)
{
	char ip_tmp[20] = {0};
	unsigned char loopx = 0;
	char* IP_String = NULL;
	GetStringFromString(name,String,(char**)&(IP_String));
	for(;loopx<20;loopx++)
		{
			if(*(IP_String + loopx) == '&')
				{
					break;
				}
			ip_tmp[loopx] = *(IP_String + loopx);
		}
	return IPStrToInt(ip_tmp);
}
unsigned char GetStringFromString(char* name,char* String,char** StringValue)
{	
	unsigned int name_length = 0;
	name_length = string_length(name);
	unsigned int string_offset = 0;
	string_offset = GetStringOffset(name,String);
	if(0XFFFFFFFF == string_offset)
		{
			return 0;
		}
	*StringValue =(String+string_offset+name_length+1);
	return 1;
}


