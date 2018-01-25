#include "learn_clear.h"



unsigned char is_switch_space_used(unsigned char index)//判断空间是否被占用
{
	return(get_bit((SwitchSaveInfo.scene_save_position),index));
}


unsigned char compare_switch_info(P_S_Switch_Info target_SwitchInfo,P_S_Switch_Info source_SwitchInfo)
{
	return my_str_cmp((unsigned char*)target_SwitchInfo,(unsigned char*)source_SwitchInfo,sizeof(S_Switch_Info));
}

unsigned char get_first_learn_empty_space(void)
{
	unsigned char loopx = 0;
	for(;loopx < MAX_SAVE_SCENE_NUM; loopx++)
		{
			if(0 == is_switch_space_used(loopx))
				{
					return loopx;
				}
		}
	return 0XFF;
}


unsigned char get_learn_space(P_S_Switch_Info SwitchInfo)//获取学习开关的保存位置
{
	unsigned char loopx = 0;
	for(;loopx < MAX_SAVE_SCENE_NUM; loopx++)
		{
			if(1 == is_switch_space_used(loopx))
				{
					if(1 == compare_switch_info(SwitchInfo,&(SwitchSaveInfo.scene[loopx].switch_info)))
						{
							return loopx;
						}
				}
		}
	return 0XFF;
}


unsigned char learn_switch_with_relaystatus(P_S_Switch_Info SwitchInfo,unsigned int status)
{
	unsigned char learn_point;
	unsigned char res = 2;//修改
	learn_point = get_learn_space(SwitchInfo);
	if(0XFF == learn_point)
		{
			res = 1;//新学习
			learn_point = get_first_learn_empty_space();
			if(0XFF == learn_point)
				{
					res =0;//学习失败
					return res;
				}
		}
	value_set_bit(&(SwitchSaveInfo.scene_save_position),learn_point);
	my_str_cpy((unsigned char*)(&(SwitchSaveInfo.scene[learn_point].switch_info)),(unsigned char*)SwitchInfo,sizeof(S_Switch_Info));
	SwitchSaveInfo.scene[learn_point].relay_status = status;
	SwitchSaveInfo.scene[learn_point].windows1_status = machine_windows_info.windows[0].status;
	SwitchSaveInfo.scene[learn_point].windows2_status = machine_windows_info.windows[1].status;
	return res;
}

unsigned char learn_switch(P_S_Switch_Info SwitchInfo)
{
	unsigned char learn_point;
	unsigned char res = 2;//修改
	learn_point = get_learn_space(SwitchInfo);
	if(0XFF == learn_point)
		{
			res = 1;//新学习
			learn_point = get_first_learn_empty_space();
			if(0XFF == learn_point)
				{
					res =0;//学习失败
					return res;
				}
		}
	value_set_bit(&(SwitchSaveInfo.scene_save_position),learn_point);
	my_str_cpy((unsigned char*)(&(SwitchSaveInfo.scene[learn_point].switch_info)),(unsigned char*)SwitchInfo,sizeof(S_Switch_Info));
	SwitchSaveInfo.scene[learn_point].relay_status = machine_relay_info.relay_status;
	SwitchSaveInfo.scene[learn_point].windows1_status = machine_windows_info.windows[0].status;
	SwitchSaveInfo.scene[learn_point].windows2_status = machine_windows_info.windows[1].status;
	
	Save_Data(cell_Switch, (unsigned char*)&SwitchSaveInfo,sizeof(SwitchSaveInfo));
	
	return res;
}


unsigned char clear_switch(P_S_Switch_Info SwitchInfo)
{
	unsigned char learn_point;
	unsigned char res = 1;
	learn_point =  get_learn_space(SwitchInfo);
	if(0XFF == learn_point)
		{
			return res;
		}
	value_clear_bit(&(SwitchSaveInfo.scene_save_position),learn_point);
	
	Save_Data(cell_Switch, (unsigned char*)&SwitchSaveInfo,sizeof(SwitchSaveInfo));
	
	return res;
}















unsigned char Is_Learn_Actioning(void)
{
	return MachineInfoCfg.learn_clear.learn_flag;
}

void Clear_Learn_Flag(void)
{
	MachineInfoCfg.learn_clear.learn_flag = 0;
}

unsigned char Is_Clear_Actioning(void)

{
	return MachineInfoCfg.learn_clear.clear_flag;
}

void Clear_Clear_Flag(void)
{
	MachineInfoCfg.learn_clear.clear_flag= 0;
}


void End_Learn_Action(void* arg)
{
	if(MachineInfoCfg.learn_clear.learn_flag)
		{
			//学习失败
		}
	MachineInfoCfg.learn_clear.learn_flag = 0;
}

void End_Clear_Action(void* arg)
{
	if(MachineInfoCfg.learn_clear.clear_flag)
		{
			//清除失败
		}
	MachineInfoCfg.learn_clear.clear_flag = 0;
}

void Start_Learn_Action(void)
{
	MachineInfoCfg.learn_clear.learn_flag = 1;
	MachineInfoCfg.learn_clear.clear_flag= 0;
	register_callback_function_into_timer(register_value_into_timer(5000, 0),End_Learn_Action);
}

void Start_Clear_Action(void)
{
	MachineInfoCfg.learn_clear.clear_flag= 1;
	MachineInfoCfg.learn_clear.learn_flag = 0;
	register_callback_function_into_timer(register_value_into_timer(5000, 0),End_Clear_Action);
}



















