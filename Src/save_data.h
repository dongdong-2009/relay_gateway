#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__
#include "include.h"
//#include "relay.h"
//#include "m4_flash.h"

unsigned char my_str_cmp(unsigned char* target_data,unsigned char* source_data,unsigned short data_length);

/*

���浽flash�е����ݵı������洢����ȡ���ݵ�Ψһ��ʶ

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
�������ݵĲ��ұ�

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

flash ����

flash�������
flashʹ�õ�SAVE_FLASH_START_SECTOR(11)ҳ�������ݣ���һҳ��128kb 
����һҳ�ֳ�16��Сҳ��ÿ��Сҳ8kb ��ram����һ��8kb�Ļ��棬
�൱�ڵ�ǰʹ��Сҳ�ĸ��ơ�16��Сҳ���һ����״�洢�ṹ��
ÿ�α������ݵ�ʱ������һ��Сҳд���ݣ���Сҳʹ���꣬
�Ƚ�����ҳ������Ȼ��ӵ�һҳ��ʼ����


Сҳ�ṹ
��һ������ָʾ��ǰСҳ��flash��Ϣ  ʹ�õڼ���Сҳ  ���Сҳ�ĵ�ַ
�ڶ�������Сҳ�б��������Ŀ¼
�������־��Ǳ���ľ�������

*/
#define FLASHSAVEBUFFSIZE	(PAGE_SIZE-sizeof(S_Flash_Page_Info) - sizeof(S_FlashMap))

typedef struct//flash Сҳ
{
	S_Flash_Page_Info PageInfo;//ָʾ��ǰ��Ч��flashСҳ��Ϣ
	S_FlashMap flash_map;//flashСҳ�б����������Ϣ
	unsigned char FlashSaveBuff[FLASHSAVEBUFFSIZE];
}S_Flash_Buff,*P_S_Flash_Buff;
extern S_Flash_Buff FlashRamBuff;




/*

���ݱ�����غ���
�������¶�ȡ����ɾ��
����Ҫ����Ľṹ�巢���ı�֮���������flash

*/


void InitSaveData(void);
unsigned char Save_Data(cell_name cell,unsigned char* data,unsigned short size);
unsigned char Read_Data(cell_name cell,unsigned char* data,unsigned short size);





unsigned char my_str_cmp(unsigned char* target_data,unsigned char* source_data,unsigned short data_length);
void my_str_cpy(unsigned char* target_data,unsigned char* source_data,unsigned short data_length);




#endif
