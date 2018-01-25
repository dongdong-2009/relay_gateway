#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__
#include "include.h"
//#include "relay.h"
//#include "m4_flash.h"

unsigned char my_str_cmp(unsigned char* target_data,unsigned char* source_data,unsigned short data_length);

/*

保存到flash中的数据的别名，存储、读取数据的唯一标识

*/

typedef enum{
	cell_Switch = 1,
	cell_IPaddress,
	cell_RelayStatus,
	cell_SerialConfig,
	cell_WindowsStatus,
	cell_SYSConfig,
}cell_name;



#define DATA_GAP	10

typedef struct
{
	unsigned char CurPageNum;
	unsigned int CurPageAddress;
	
}S_Flash_Page_Info,*P_S_Flash_Page_Info;


typedef struct
{
	cell_name name;
	unsigned short size;
	unsigned int p_offset;
	unsigned char* data;
}S_CellInfo,*P_S_CellInfo;



/*
保存数据的查找表

*/
#define MAX_MAP_NUM	16
typedef struct
{
	unsigned int map_shape;
	unsigned char* next_free_addr;
	unsigned int next_free_offset;
	S_CellInfo cell_info[MAX_MAP_NUM];	
}S_FlashMap,*P_S_FlashMap;
//extern S_FlashMap flash_map;



/*

flash 缓存

flash保存概述
flash使用第SAVE_FLASH_START_SECTOR(11)页保存数据，这一页共128kb 
将这一页分成16个小页，每个小页8kb ，ram中有一块8kb的缓存，
相当于当前使用小页的复制。16个小页组成一个环状存储结构，
每次保存数据的时候都往后一个小页写数据，当小页使用完，
先将整个页擦除，然后从第一页开始保存


小页结构
第一部分是指示当前小页的flash信息  使用第几个小页  这个小页的地址
第二部分是小页中保存的数据目录
第三部分就是保存的具体数据

*/
#define FLASHSAVEBUFFSIZE	(PAGE_SIZE-sizeof(S_Flash_Page_Info) - sizeof(S_FlashMap))

typedef struct//flash 小页
{
	S_Flash_Page_Info PageInfo;//指示当前有效的flash小页信息
	S_FlashMap flash_map;//flash小页中保存的数据信息
	unsigned char FlashSaveBuff[FLASHSAVEBUFFSIZE];
}S_Flash_Buff,*P_S_Flash_Buff;
extern S_Flash_Buff FlashRamBuff;




/*

数据保存相关函数
包括更新读取不能删除
在需要保存的结构体发生改变之后必须重置flash

*/


void InitSaveData(void);
unsigned char Save_Data(cell_name cell,unsigned char* data,unsigned short size);
unsigned char Read_Data(cell_name cell,unsigned char* data,unsigned short size);





unsigned char my_str_cmp(unsigned char* target_data,unsigned char* source_data,unsigned short data_length);
void my_str_cpy(unsigned char* target_data,unsigned char* source_data,unsigned short data_length);




#endif
