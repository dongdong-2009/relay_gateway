#ifndef __TCM300_H__
#define __TCM300_H__
#include "include.h"
#define cOperateCode_A   0X50
#define cOperateCode_C   0X70
#define cOperateCode_B   0X10
#define cOperateCode_D   0X30


typedef struct
{
	unsigned char read_tcm300_id_flag;
	unsigned char TCM300_ID[4];
	unsigned char GATE_WAY_ID[4];
	unsigned char SET_LEARN_CLEAR_ID[4];
	unsigned char TARGAT_ID[4];
	unsigned char TARGAT_DATA[4];
	unsigned char NET_MAC_ID[6];
}S_ID,*P_S_ID;
extern S_ID id;


#define TCM300_MAX_RX 300

#define TCM300_RECV_BUFF_SIZE 300
#define TCM300_SEND_BUFF_SIZE 256

typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[TCM300_RECV_BUFF_SIZE];
	unsigned char tx_data[TCM300_SEND_BUFF_SIZE];
	
	unsigned char recv_ok;	
	unsigned short point;
	unsigned char enable_rxdata;
	unsigned char vBigRxMax;
	PVoid_Function_PVoid Action;
}S_Recv_From_Tcm300,*P_S_Recv_From_Tcm300;
extern S_Recv_From_Tcm300 data_from_tcm300;

void task_deal_tcm300(void);
void read_tcm300_id(void);
#endif

