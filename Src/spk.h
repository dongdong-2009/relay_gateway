#ifndef __SPK_H__
#define __SPK_H__
#include "include.h"

#define  SPK_1	HAL_GPIO_WritePin(SPK_GPIO_Port,SPK_Pin,GPIO_PIN_SET)
#define  SPK_0	HAL_GPIO_WritePin(SPK_GPIO_Port,SPK_Pin,GPIO_PIN_RESET)

#define SPK_STATUS_FREE		0
#define SPK_STATUS_BEEPING_1	1
#define SPK_STATUS_BEEPING_0	2

typedef struct 
{
	unsigned char spk_status;
	unsigned char timer;
	unsigned char beep_times;
	unsigned short keep_time;
	unsigned short interval_time;
}S_SPK_Info,*P_S_SPK_Info;
void spk_beep(unsigned char beep_times,unsigned short keep_time,unsigned short interval_time);


#endif

