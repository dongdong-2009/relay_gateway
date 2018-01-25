#include "spk.h"
S_SPK_Info SPK;
void timer_spk_callback_function(void* arg)
{
	switch(SPK.spk_status)
		{
			case SPK_STATUS_BEEPING_1:
				SPK_0;
				SPK.spk_status = SPK_STATUS_BEEPING_0;
				SPK.timer = register_value_into_timer(SPK.interval_time,0);
				register_callback_function_into_timer(SPK.timer,timer_spk_callback_function);
				break;

			case SPK_STATUS_BEEPING_0:
				if(1 == SPK.beep_times)
					{						
						SPK.spk_status = SPK_STATUS_FREE;
					}
				else
					{
						SPK_1;
						SPK.spk_status = SPK_STATUS_BEEPING_1;
						SPK.beep_times --;
						SPK.timer = register_value_into_timer(SPK.keep_time,0);
						register_callback_function_into_timer(SPK.timer,timer_spk_callback_function);
					}
				break;

			default:
				break;
		}
}

void spk_beep(unsigned char beep_times,unsigned short keep_time,unsigned short interval_time)
{
	SPK.beep_times = beep_times;
	SPK.keep_time = keep_time;
	SPK.interval_time = interval_time;
	
	if(SPK.spk_status != SPK_STATUS_FREE)
		{
			return;
		}
	else
		{
			SPK.spk_status = SPK_STATUS_BEEPING_1;
		}
	SPK.timer = register_value_into_timer(SPK.keep_time,0);
	register_callback_function_into_timer(SPK.timer,timer_spk_callback_function);
	SPK_1;
}


