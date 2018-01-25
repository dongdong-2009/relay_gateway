#include "timer_operat.h"
#include "queue.h"

S_Timer_Counter timer_counter;

unsigned int convert_to_timer_msg(unsigned int value)
{
	unsigned int data = 0;
	data = value&0XFFFFFF;
	return (data|0X1000000);
}


unsigned char timer_alloc(void)
{
	unsigned char loopx;
	for(loopx = 0; loopx < MAX_COUNTVALUE_NUMBER ; loopx ++)
		{
			if(!(timer_counter.space & (1<<loopx)))
				{
					timer_counter.space|= (1<<loopx);
					memset((void*)&(timer_counter.timer[loopx]),0,sizeof(S_Value_In_Timer));
					return loopx;
				}
		}
	return 0XFF;
}

unsigned char timer_free(unsigned char space)
{
	if(timer_counter.space & (1 << space))
		{
			timer_counter.space &= (~(1<<space));
			return 1;
		}
	return 0;
}
void timer_reload(unsigned char space)
{
	//timer_counter.timer[space].count_value = timer_counter.timer[space].init_value;
}

unsigned char timer_effective(unsigned char space)
{
	if(timer_counter.space & (1 << space))
	{
		return 1;
	}
	return 0;
}
unsigned char register_value_into_timer(unsigned int count_value,unsigned char mod)//mod=1 ÖØ¸´¼ÆÊ±Æ÷
{
	unsigned char space;
	space = timer_alloc();
	if(space > MAX_COUNTVALUE_NUMBER)
		{
			return 0XFF;
		}
	timer_counter.timer[space].target_value = count_value;
	timer_counter.timer[space].repeatability = mod;
	return space;
}

void register_queue_into_timer(unsigned char space,osMessageQId queue)
{
	timer_counter.timer[space].signal_queue = queue;
}

void register_callback_function_into_timer(unsigned char space,P_TimerCounter_Deal_Function function)
{
	timer_counter.timer[space].timer_counter_callback_function = function;
}

void timer_overflow_function(unsigned char space)
{
	unsigned int msg;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	
	timer_counter.timer[space].count_value ++;
	
	if(timer_counter.timer[space].count_value >timer_counter.timer[space].target_value)
		{
			timer_counter.timer[space].overflow_flag = 1;
			if(timer_counter.timer[space].signal_queue)
				{
					//xQueueSendToBackFromISR(timer_counter.timer[space].signal_queue,(void*)space,100);
					msg = convert_to_timer_msg(space);
					xQueueSendFromISR(timer_counter.timer[space].signal_queue,&msg,&xHigherPriorityTaskWoken);
				}
			if(timer_counter.timer[space].timer_counter_callback_function)
				{
					//timer_counter.timer[space].timer_counter_callback_function((void*)&(timer_counter.timer[space]));
					timer_counter.timer[space].timer_counter_callback_function((void*)space);
				}

			
			if(timer_counter.timer[space].repeatability)
				{
					//timer_reload(space);
					timer_counter.timer[space].count_value = 0;
				}
			else
				{
					timer_free(space);
				}
		}
}

void INC_COUNTER_TIMER(void)
{
	unsigned char loopx;
	timer_counter.timer_tick ++;
	if(timer_counter.timer_tick > 1000)
		{
			timer_counter.timer_tick = 0;
			timer_counter.second_tick ++;
		}
	for(loopx = 0 ;loopx < MAX_COUNTVALUE_NUMBER; loopx ++)
		{
			if(timer_effective(loopx))
				{
					timer_overflow_function(loopx);
				}
		}
}


unsigned int get_cur_second_tick(void)
{
	return timer_counter.second_tick;
}

unsigned char is_timer_counter_overflow(unsigned char space)
{
	return timer_counter.timer[space].overflow_flag;
}

void clear_timer_counter_overflow_flag(unsigned char space)
{
	timer_counter.timer[space].overflow_flag = 0;
}

void InitTimerCounter(void)
{
	HAL_TIM_Base_Start_IT(&COUNTER_TIMER);
}


//////////////////////////////////////////counter_timer

void InitDelayTimer(void)
{
	HAL_TIM_Base_Start(&US_COUNTER_TIMER);
}

void TimerDelay_us(unsigned int n)
{
	unsigned short delay_start_time;
	unsigned short loop_num = 0;
	unsigned char loop_flag = 1;	
	while(1)
		{
			if(((delay_start_time+loop_num*999) - US_COUNTER_TIMER.Instance->CNT)>n)
				{
					if((US_COUNTER_TIMER.Instance->CNT == delay_start_time)&&(loop_flag == 0))
						{
							loop_num++;
							loop_flag = 1;
						}	
					else
						{
							loop_flag = 0;
						}
				}
			else
				{
					return;
				}
		}

}

unsigned char test_runing = 1;
void StopRun(void* arg)
{
	test_runing = 0;
}

void TestUsDelay(void)
{	
	unsigned char delay_count = 0;
	register_callback_function_into_timer(register_value_into_timer(1000,0), StopRun);
	while(test_runing)
		{
			TimerDelay_us(10);
			delay_count ++;
		}
	printf("delay_count :%d\n\r",delay_count);
}





