#include "save_data.h"

S_Flash_Buff FlashRamBuff;
//S_FlashMap flash_map;


S_SceneSave_Data_Info SwitchSaveInfo;

unsigned char my_str_cmp(unsigned char* target_data,unsigned char* source_data,unsigned short data_length)
{
	unsigned short loopx = data_length;
	unsigned char* tmp_target_data = target_data;
	unsigned char* tmp_source_data = source_data;
	while(loopx--)
		{
			if(*(tmp_target_data++) != *(tmp_source_data++))
				{
					return 0;
				}
		}
	return 1;
}

void my_str_cpy(unsigned char* target_data,unsigned char* source_data,unsigned short data_length)
{
	unsigned short loopx = data_length;
	unsigned char* tmp_target_data = target_data;
	unsigned char* tmp_source_data = source_data;
	while(loopx--)
		{
			*(tmp_target_data++) = *(tmp_source_data++);
		}
}







void update_mapinfo(void)
{
	//memcopy((char*)&flash_map, (char *)&(FlashRamBuff.FlashSaveBuff), sizeof(flash_map));	
}

void reset_map_info(void)
{
	//flash_map.map_shape= 0;
	//update_mapinfo();
	//flash_map.next_free_addr = (FlashRamBuff.FlashSaveBuff + sizeof(flash_map));

	FlashRamBuff.flash_map.map_shape = 0;
	FlashRamBuff.flash_map.next_free_offset = 0;
}

unsigned char* Get_Cell_Adress(cell_name cell)
{
	/*unsigned char loopx = 0;
	for(;loopx < MAX_MAP_NUM ; loopx++)
		{
			if(0 == get_bit(flash_map.map_shape, loopx))
				{
					continue;
				}
			else
				{
					if(cell == flash_map.cell_info[loopx].name)
						{
							//return (flash_map.cell_info[loopx].data);
							return (FlashRamBuff.FlashSaveBuff + sizeof(flash_map)+(flash_map.cell_info[loopx].p_offset));
						}
				}
		}
	return NULL;*/
	unsigned char loopx = 0;
	for(; loopx < MAX_MAP_NUM ; loopx ++)
		{
			if(0 == get_bit((FlashRamBuff.flash_map.map_shape) , loopx))
				{
					continue;
				}
			else
				{
					if(cell == FlashRamBuff.flash_map.cell_info[loopx].name)
						{
							return (FlashRamBuff.FlashSaveBuff + (FlashRamBuff.flash_map.cell_info[loopx].p_offset));
						}
				}
		}
	return NULL;
}

unsigned char Get_Last_MapShape(void)
{
	/*unsigned char loopx = (MAX_MAP_NUM);
	while(loopx--)
		{
			if(1 == get_bit(flash_map.map_shape, loopx))
				{
					return (loopx);
				}
		}
	return 0XFF;//还没保存过数据，保存在第0个cell中*/
	
	unsigned char loopx = (MAX_MAP_NUM);
	while(loopx--)
		{
			if(1 == get_bit((FlashRamBuff.flash_map.map_shape), loopx))
				{
					return (loopx);
				}
		}
	return 0XFF;//还没保存过数据，保存在第0个cell中
}

unsigned char Save_Data(cell_name cell,unsigned char* data,unsigned short size)
{
	unsigned char* target_address = NULL;
	unsigned char last_cell_shape = 0;
	target_address = Get_Cell_Adress(cell);
	if(target_address)
		{
			memcopy((char*)data, (char*)target_address, size);
			//update_mapinfo();
		}
	else
		{
			last_cell_shape = Get_Last_MapShape();
			if((MAX_MAP_NUM -1) == last_cell_shape)
				{
					return 0XFF;//空间满
				}
			else if(0XFF == last_cell_shape)
				{
					reset_map_info();//第一个保存的数据
					last_cell_shape = 0;
					//flash_map.next_free_offset = 0;
				}
			else
				{
					last_cell_shape += 1;
				}
			/*value_set_bit(&(flash_map.map_shape), last_cell_shape);
			flash_map.cell_info[last_cell_shape].name = cell;
			flash_map.cell_info[last_cell_shape].data = FlashRamBuff.FlashSaveBuff + sizeof(flash_map)+flash_map.next_free_offset;
			flash_map.cell_info[last_cell_shape].p_offset = flash_map.next_free_offset;			
			target_address = flash_map.cell_info[last_cell_shape].data;//初始化前不能用
			flash_map.cell_info[last_cell_shape].size = size;
			flash_map.next_free_offset += (size+DATA_GAP);
			memcopy((char*)data, (char*)target_address, size);			
			update_mapinfo();*/


			value_set_bit(&(FlashRamBuff.flash_map.map_shape), last_cell_shape);
			FlashRamBuff.flash_map.cell_info[last_cell_shape].name = cell;
			FlashRamBuff.flash_map.cell_info[last_cell_shape].data = FlashRamBuff.FlashSaveBuff + FlashRamBuff.flash_map.next_free_offset;
			FlashRamBuff.flash_map.cell_info[last_cell_shape].p_offset = FlashRamBuff.flash_map.next_free_offset;			
			target_address = FlashRamBuff.flash_map.cell_info[last_cell_shape].data;//初始化前不能用
			FlashRamBuff.flash_map.cell_info[last_cell_shape].size = size;
			FlashRamBuff.flash_map.next_free_offset += (size+DATA_GAP);
			memcopy((char*)data, (char*)target_address, size);		
			
		}
	SAVE_UPDATED_DATA();
	return 1;
}


unsigned char Read_Data(cell_name cell,unsigned char* data,unsigned short size)
{
	unsigned char* target_address = NULL;
	target_address = Get_Cell_Adress(cell);
	if(target_address)
		{
			memcopy((char*)target_address,(char*)data, size);
			return 1;
		}
	else
		{
			return 0XFF;//未保存的数据
		}
}

void init_flash_map(void)
{
	//FORCE_RESET_DATA_SPACE();//强制全清flash
	GET_SAVEED_DATA();
}


void test_save_data(void)
{
	Save_Data(cell_RelayStatus,(unsigned char*)&(machine_relay_info),sizeof(machine_relay_info));

	Save_Data(cell_WindowsStatus,(unsigned char*)&(machine_windows_info),sizeof(machine_windows_info));

	Save_Data(cell_RelayStatus,(unsigned char*)&(machine_relay_info),sizeof(machine_relay_info));
	
	Save_Data(cell_WindowsStatus,(unsigned char*)&(machine_windows_info),sizeof(machine_windows_info));
	
}


void InitSaveData(void)
{
	init_flash_map();
	Read_Data(cell_Switch, (unsigned char*)&SwitchSaveInfo,sizeof(SwitchSaveInfo));
	//Read_Data(cell_SYSConfig, (unsigned char*)&SysConfig,sizeof(SysConfig));
	Read_Data(cell_SYSConfig, (unsigned char*)&MachineInfoCfg,sizeof(MachineInfoCfg));
}








