/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "include.h"
#include "protocol.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId SysHandle;
osThreadId ReadKeyHandle;
osThreadId DealKeyHandle;
osThreadId DealSerialHandle;
osThreadId DealSaveDataHandle;
osThreadId DelayStartUpHandle;
osThreadId DealRelayHandle;
osMessageQId KeyQueueHandle;
osMessageQId SerialQueueHandle;
osMessageQId SaveDataQueueHandle;
osMessageQId RelayQueueHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void SysTask(void const * argument);
void ReadKeyTask(void const * argument);
void DealKeyTask(void const * argument);
void DealSerialTask(void const * argument);
void DealSaveDataTask(void const * argument);
void DelayStartUpTask(void const * argument);
void DealRelayTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of Sys */
  osThreadDef(Sys, SysTask, osPriorityNormal, 0, 128);
  SysHandle = osThreadCreate(osThread(Sys), NULL);

  /* definition and creation of ReadKey */
  osThreadDef(ReadKey, ReadKeyTask, osPriorityNormal, 0, 128);
  ReadKeyHandle = osThreadCreate(osThread(ReadKey), NULL);

  /* definition and creation of DealKey */
  osThreadDef(DealKey, DealKeyTask, osPriorityHigh, 0, 128);
  DealKeyHandle = osThreadCreate(osThread(DealKey), NULL);

  /* definition and creation of DealSerial */
  osThreadDef(DealSerial, DealSerialTask, osPriorityHigh, 0, 500);
  DealSerialHandle = osThreadCreate(osThread(DealSerial), NULL);

  /* definition and creation of DealSaveData */
  osThreadDef(DealSaveData, DealSaveDataTask, osPriorityAboveNormal, 0, 400);
  DealSaveDataHandle = osThreadCreate(osThread(DealSaveData), NULL);

  /* definition and creation of DelayStartUp */
  osThreadDef(DelayStartUp, DelayStartUpTask, osPriorityNormal, 0, 400);
  DelayStartUpHandle = osThreadCreate(osThread(DelayStartUp), NULL);

  /* definition and creation of DealRelay */
  osThreadDef(DealRelay, DealRelayTask, osPriorityRealtime, 0, 400);
  DealRelayHandle = osThreadCreate(osThread(DealRelay), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of KeyQueue */
  osMessageQDef(KeyQueue, 4, uint8_t);
  KeyQueueHandle = osMessageCreate(osMessageQ(KeyQueue), NULL);

  /* definition and creation of SerialQueue */
  osMessageQDef(SerialQueue, 12, uint8_t);
  SerialQueueHandle = osMessageCreate(osMessageQ(SerialQueue), NULL);

  /* definition and creation of SaveDataQueue */
  osMessageQDef(SaveDataQueue, 6, uint32_t);
  SaveDataQueueHandle = osMessageCreate(osMessageQ(SaveDataQueue), NULL);

  /* definition and creation of RelayQueue */
  osMessageQDef(RelayQueue, 32, uint32_t);
  RelayQueueHandle = osMessageCreate(osMessageQ(RelayQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* SysTask function */
void SysTask(void const * argument)
{
  /* init code for LWIP */
  
  osDelay(1000);
  MX_LWIP_Init();

  /* USER CODE BEGIN SysTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SysTask */
}

/* ReadKeyTask function */
void ReadKeyTask(void const * argument)
{
  /* USER CODE BEGIN ReadKeyTask */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ReadKeyTask */
}

/* DealKeyTask function */
void DealKeyTask(void const * argument)
{
  /* USER CODE BEGIN DealKeyTask */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DealKeyTask */
}

/* DealSerialTask function */
void DealSerialTask(void const * argument)
{
  /* USER CODE BEGIN DealSerialTask */
  save_task_info();  
  unsigned char serial_value;
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(SerialQueueHandle, &serial_value, portMAX_DELAY);
	switch(serial_value)
		{
			case RS232_1_MSG:						
				break;
			case RS232_2_MSG:
				break;

			case RS485_1_MSG:
				break;

			case RS485_2_MSG:
				break;

			case TCM300_MSG:
				task_deal_tcm300();	
				break;

			case KEY_BOARD_MSG:
				Deal_KeyBoard();
				break;

			default:
				break;
		}
  }
  /* USER CODE END DealSerialTask */
}

/* DealSaveDataTask function */
void DealSaveDataTask(void const * argument)
{
  /* USER CODE BEGIN DealSaveDataTask */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DealSaveDataTask */
}

/* DelayStartUpTask function */
void DelayStartUpTask(void const * argument)
{
  /* USER CODE BEGIN DelayStartUpTask */
  save_task_info();
  //unsigned char test_buf[50] = {0};
  double tmp[3] = {0.0};
  Init_SysConfig();
  //InitSaveData();

  //init_windows();

  InitIICBus();
  InitSerialBus();
  osDelay(50);
  read_tcm300_id();

  //IIC24C0XWrite(&IIC_24C0X,0,"123456789987654321",18);
  
  osDelay(2000);
  //IIC24C0XRead(&IIC_24C0X,0,test_buf,50);  	

  
  spk_beep(1,10,500);
  
	Init_Ctrl_Server();
	osDelay(1000);

	//
	P_S_Socket_Task_Info server_socket;
	server_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));
  	server_socket->port = 4477;
	socket_creat_Concurrency_TCP_server_task("server_task_1",server_socket);
  /* P_S_Socket_Task_Info server_socket;
  server_socket = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));

  server_socket->port = 5566;
  socket_creat_TCP_server_task("server_task_1",server_socket);

  osDelay(1000);
  P_S_Socket_Task_Info server_socket2;
  server_socket2 = (P_S_Socket_Task_Info)malloc(sizeof(S_Socket_Task_Info));

  server_socket2->port = 1111;
  socket_creat_TCP_server_task("server_task_2",server_socket2);

  osDelay(1000);*/
  
  //InitHttpServer();//webpage ·þÎñÆ÷
  //osDelay(1000);
  
  InitUDPCtrlServer();
  osDelay(1000);
  UpLoadMachineStatus();
  
  //init_flash_map();
  
  //osDelay(500);
  //test_save_data();
  //test_tcm300();

  
  
  /* Infinite loop */
  for(;;)
  {
	//test_serial();
  osDelay(1000);
	
	tmp[0] = ChangeTem(GetTem(&IIC_SHT20_1));
	
	tmp[1] = ChangeTem(GetTem(&IIC_SHT20_2));
	
	tmp[2] = ChangeTem(GetTem(&IIC_SHT20_3));

	MachineInfoCfg.Temp_Hum[0].temperature = (unsigned char)(tmp[0] + 100);
	
	MachineInfoCfg.Temp_Hum[1].temperature = (unsigned char)(tmp[1] + 100);
	
	MachineInfoCfg.Temp_Hum[2].temperature = (unsigned char)(tmp[2] + 100);

	MachineInfoCfg.Temp_Hum[0].humidity = (unsigned char)ChangeHum(GetHum(&IIC_SHT20_1));
	MachineInfoCfg.Temp_Hum[1].humidity = (unsigned char)ChangeHum(GetHum(&IIC_SHT20_2));
	MachineInfoCfg.Temp_Hum[2].humidity = (unsigned char)ChangeHum(GetHum(&IIC_SHT20_3));

	//SAVE_UPDATED_DATA();

	}
  /* USER CODE END DelayStartUpTask */
}

/* DealRelayTask function */
void DealRelayTask(void const * argument)
{
  /* USER CODE BEGIN DealRelayTask */
	save_task_info();
	unsigned int recv_relay_value;	
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(RelayQueueHandle, &recv_relay_value, portMAX_DELAY);
	task_deal_relay(recv_relay_value);
   }
  /* USER CODE END DealRelayTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
