#ifndef __LEARN_CLEAR_H__
#define __LEARN_CLEAR_H__
#include "include.h"




#define MAX_SAVE_SCENE_NUM	16

typedef struct
{
	unsigned char tcm300_ID[4];
	unsigned char key_value;
}S_Switch_Info,*P_S_Switch_Info;

typedef struct
{
	S_Switch_Info switch_info;
	unsigned int relay_status;
	unsigned char windows_status;
	E_Windows_Status windows1_status;
	E_Windows_Status windows2_status;
}S_Scene_Info,*P_S_Scene_Info;

typedef struct
{
	unsigned char is_this_data_effect;
	unsigned int scene_save_position;
	S_Scene_Info scene[MAX_SAVE_SCENE_NUM];
}S_SceneSave_Data_Info,*P_S_SceneSave_Data_Info;
extern S_SceneSave_Data_Info SwitchSaveInfo;



unsigned char get_learn_space(P_S_Switch_Info SwitchInfo);
unsigned char learn_switch(P_S_Switch_Info SwitchInfo);
unsigned char clear_switch(P_S_Switch_Info SwitchInfo);

unsigned char learn_switch_with_relaystatus(P_S_Switch_Info SwitchInfo,unsigned int status);


unsigned char Is_Learn_Actioning(void);
unsigned char Is_Clear_Actioning(void);

void Start_Learn_Action(void);
void Start_Clear_Action(void);
void Clear_Learn_Flag(void);
void Clear_Clear_Flag(void);

#endif

