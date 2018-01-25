#include "include.h"



void memcopy(char* from_addr,char* to_addr,unsigned short length)
{
	char* tmp_from_addr;
	char* tmp_to_addr;
	tmp_from_addr = from_addr;
	tmp_to_addr = to_addr;
	while(length --)
		{
			*(tmp_to_addr ++) = *(tmp_from_addr ++);
		}
}


void erase_flash(unsigned int SectorNum)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	unsigned int SectorError;
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector= SectorNum;
	EraseInitStruct.NbSectors = 1;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	  {
	    /*
	      Error occurred while page erase.
	      User can add here some code to deal with this error.
	      PAGEError will contain the faulty page and then to know the code error on this page,
	      user can call function 'HAL_FLASH_GetError()'
	    */
	    /* Infinite loop */	   
	  }
	HAL_FLASH_Lock();
}


unsigned char write_flash(unsigned address,char* data,unsigned short len)
{
	//erase_flash(address);
	unsigned char MemoryProgramStatus = 1;
	unsigned short count = 0;
	unsigned int Address = 0;
	char* data_tmp;
	data_tmp = data;
	
	Address = address;
	HAL_FLASH_Unlock();
	while (count < len)
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *(int*)data_tmp) == HAL_OK)
				{
					Address = Address + 4;
					data_tmp= data_tmp+ 4;
					count = count +4;
				}
			else
				{
					/* Error occurred while writing data in Flash memory. 
					User can add here some code to deal with this error */	        
				}
		}
	HAL_FLASH_Lock();
	/* Check the correctness of written data */
	Address = address;
	data_tmp = data;
	count = 0;
	while (count < len)				
		{
			if((*(__IO unsigned int*) Address) != *(unsigned int*)data_tmp)
				{
					MemoryProgramStatus = 0;
					return MemoryProgramStatus;
				}
			Address += 4;			  
			data_tmp= data_tmp+4;
			count = count+4;
		}

	HAL_FLASH_Lock();
	return  MemoryProgramStatus;
} 


unsigned char read_flash(unsigned address,char* data,unsigned short len)
{
	char* data_tmp;
	data_tmp = data;
	unsigned short count = 0;
	unsigned char MemoryProgramStatus = 1;
	unsigned int Address = address;
	while (count < len)				
		{
			*data_tmp = (*(__IO unsigned char*) Address);
			Address += 1;			  
			data_tmp= data_tmp+ 1;
			count = count +1;
		}

	/* Check the correctness of read data */
	Address = address;
	data_tmp = data;
	count = 0;
	while (count < len)				
		{
			if((*(__IO unsigned char*) Address) != *data_tmp)
				{
					MemoryProgramStatus = 0;
					return MemoryProgramStatus;
				}
			Address += 1;			  
			data_tmp= data_tmp+ 1;
			count = count +1;
		}
	return MemoryProgramStatus;
}


unsigned char GetFlashSavePage(void)
{
	unsigned char loopx = 0;
	unsigned char page_num = 0XFF;
	for(;loopx < PAGE_NUM_PER_SECTION; loopx++)
		{
			read_flash((FLASH_ADDRESS(SAVE_FLASH_START_SECTOR)+((loopx)*PAGE_SIZE)),(char*)&FlashRamBuff,sizeof(S_Flash_Page_Info));
			if((FlashRamBuff.PageInfo.CurPageNum != 0XFF)&&(FlashRamBuff.PageInfo.CurPageAddress == (FLASH_ADDRESS(SAVE_FLASH_START_SECTOR)+((FlashRamBuff.PageInfo.CurPageNum)*PAGE_SIZE))))
				{
					page_num = FlashRamBuff.PageInfo.CurPageNum;
				}
		}
	return page_num;
}

unsigned char GetNextSavePage(void)
{
	unsigned char NextPage;
	NextPage = GetFlashSavePage();
	if(NextPage > (PAGE_NUM_PER_SECTION-2))
		{
			return 0;
		}
	else
		{
			return NextPage+1;
		}
}

void GetFlashData(unsigned char PageNum)
{
	if(PageNum > (PAGE_NUM_PER_SECTION-1))
		{
			return;
		}
	read_flash((FLASH_ADDRESS(SAVE_FLASH_START_SECTOR)+((PageNum)*PAGE_SIZE)),(char*)&FlashRamBuff,sizeof(S_Flash_Buff));
}
//GetFlashData(GetFlashSavePage())

void SaveFlashData(unsigned char PageNum)
{	
	if(PageNum > (PAGE_NUM_PER_SECTION-1))
		{
			return;
		}
	write_flash((FLASH_ADDRESS(SAVE_FLASH_START_SECTOR)+((PageNum)*PAGE_SIZE)),(char*)&FlashRamBuff,sizeof(S_Flash_Buff));
}
//SaveFlashData(GetNextSavePage)

unsigned char IsSaveFlashInit(void)
{
	unsigned int loopx = FLASH_SECTION_SIZE/4;
	int* CheckPoint = NULL;
	CheckPoint = (int*)(FLASH_ADDRESS(SAVE_FLASH_START_SECTOR));
	while(loopx --)
		{
			if(0XFFFFFFFF != *(CheckPoint++))
				{
					return 0;
				}
		}
	return 1;
}

void FORCE_RESET_DATA_SPACE(void)
{
	erase_flash(SAVE_FLASH_START_SECTOR);
}

void RESET_DATA_SPACE(void)
{
	if((0XFF == GetFlashSavePage()) && (IsSaveFlashInit()))
		{
			erase_flash(SAVE_FLASH_START_SECTOR);
		}
}

void GET_SAVEED_DATA(void)
{
	unsigned char page_num = 0;
	page_num = GetFlashSavePage();
	GetFlashData(page_num);
}

void SAVE_UPDATED_DATA(void)
{
	unsigned char page_num = 0;
	page_num = GetNextSavePage();
	FlashRamBuff.PageInfo.CurPageNum = page_num;
	FlashRamBuff.PageInfo.CurPageAddress = (FLASH_ADDRESS(SAVE_FLASH_START_SECTOR)+((page_num)*PAGE_SIZE));
	if(0 == page_num)
		{
			erase_flash(SAVE_FLASH_START_SECTOR);
		}
	SaveFlashData(page_num);
}


void TEST_SAVE_DATA(void)
{
	
}


