#ifndef __M4_FLASH_H__
#define __M4_FLASH_H__

#include "include.h"


#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define KB_		1024
#define FLASH_SECTION_SIZE		(128*KB_)
#define PAGE_SIZE	(8*KB_)

/*!!!!!     FlasSectorNum 取值范围为5-11      !!!!!!*/
#define FLASH_ADDRESS(FlashSectorNum)	(((FlashSectorNum - 5)*FLASH_SECTION_SIZE)+ADDR_FLASH_SECTOR_5)


#define SAVE_FLASH_START_SECTOR	11
#define SAVE_FLASH_ADDRESS	FLASH_ADDRESS(SAVE_FLASH_START_SECTOR)


#define PAGE_NUM_PER_SECTION	(FLASH_SECTION_SIZE/PAGE_SIZE)
#define PAGE_ADDRESS_IN_SECTOR(FlashSectorNum,PageNum)	(FLASH_ADDRESS(FlashSectorNum)+((PageNum)*PAGE_SIZE))
#define LAST_PAGE_ADDRESS_IN_SECTOR(FlashSectorNum)	(FLASH_ADDRESS(FlashSectorNum)+((PAGE_NUM_PER_SECTION-1)*PAGE_SIZE))

void memcopy(char* from_addr,char* to_addr,unsigned short length);


void FORCE_RESET_DATA_SPACE(void);

/*
以下三个函数用于保存数据，数据保存在SAVE_FLASH_START_SECTOR(11) sector中，这个sector分成
若干(PAGE_NUM_PER_SECTION)个页来保存数据，每次调用SAVE_UPDATED_DATA()函数时，会在下一个
页中写入数据，当整个SECTOR被写完后，会擦除这个SECTOR从头开始写，修改读取数据
都在FlashRamBuff中进行

*/
void RESET_DATA_SPACE(void);
void GET_SAVEED_DATA(void);
void SAVE_UPDATED_DATA(void);








#endif

