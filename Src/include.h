#ifndef __INCLUDE_H__
#define __INCLUDE_H__
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;


typedef int (*Int_Function_Int) (int arg);
typedef void (*Void_Function_Int) (int arg);
typedef void (*Void_Function_Void) (void);
typedef void* (*PVoid_Function_PVoid) (void* arg);

typedef unsigned char (*UChar_Function_Void) (void);



#define EFFECT 	0X55
#define INVALID 	0XAA

#define WATCH_DOG_	0

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>
#include <stddef.h>
//#include "core_cmFunc.h"

//#include "event_groups.h"
//#include "StackMacros.h"

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "dma.h"
#include "lwip.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "uart_recv.h"


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "m4_flash.h"

#include "cpu_.h"
#include "CRC.h"
#include "socket_lib.h"
#include "key.h"
#include "relay.h"
#include "timer_operat.h"
#include "tcm300.h"
//#include "machine_info.h"
#include "save_data.h"
#include "WEBPAGES/web.h"
#include "iic.h"
#include "my_it.h"
#include "transmit.h"
#include "serial_server.h"
#include "webserver.h"
#include "spk.h"
#include "m4_flash.h"
//#include "protocol.h"
#include "machine_info_cfg.h"
#include "learn_clear.h"

extern osMessageQId KeyQueueHandle;
extern osMessageQId SerialQueueHandle;
extern osMessageQId SaveDataQueueHandle;
extern osMessageQId RelayQueueHandle;


#endif



