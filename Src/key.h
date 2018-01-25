#ifndef __KEY_H__
#define __KEY_H__
#include "include.h"
#define READ_KEY_STATUS(number)		HAL_GPIO_ReadPin(KEY_##number##_GPIO_Port, KEY_##number##_Pin)

#define key_press(x)  (0X20 +3*((x)))
#define key_long_press(x)  (0X21 +3*((x)))
#define key_long_long_press(x)	(0X22+3*((x)))




#define KEY_1_TRIGGER 	0X00
#define KEY_2_TRIGGER 	0X01
#define KEY_3_TRIGGER	0X02
#define KEY_4_TRIGGER 	0X03
#define KEY_5_TRIGGER 	0X04
#define KEY_6_TRIGGER 	0X05
#define KEY_7_TRIGGER 	0X06
#define KEY_8_TRIGGER 	0X07
#define KEY_9_TRIGGER 	0X08
#define KEY_10_TRIGGER 	0X09
#define KEY_11_TRIGGER 	0X0A
#define KEY_12_TRIGGER 	0X0B
#define KEY_13_TRIGGER	0X0C
#define KEY_14_TRIGGER 	0X0D
#define KEY_15_TRIGGER 	0X0E
#define KEY_16_TRIGGER 	0X0F
#define KEY_17_TRIGGER 	0X10
#define KEY_18_TRIGGER 	0X11
#define KEY_19_TRIGGER 	0X12
#define KEY_20_TRIGGER 	0X13
#define KEY_21_TRIGGER 	0X14
#define KEY_22_TRIGGER 	0X15
#define KEY_23_TRIGGER	0X16


#define KEY_LONG_BASE		0X70
#define KEY_LONG_TRIGGER(X)	(KEY_LONG_BASE+(X))	
#define KEY_1_LONG_TRIGGER 	(KEY_LONG_BASE+0)
#define KEY_2_LONG_TRIGGER 	(KEY_LONG_BASE+1)
#define KEY_3_LONG_TRIGGER	(KEY_LONG_BASE+2)
#define KEY_4_LONG_TRIGGER 	(KEY_LONG_BASE+3)
#define KEY_5_LONG_TRIGGER 	(KEY_LONG_BASE+4)
#define KEY_6_LONG_TRIGGER 	(KEY_LONG_BASE+5)
#define KEY_7_LONG_TRIGGER 	(KEY_LONG_BASE+6)
#define KEY_8_LONG_TRIGGER 	(KEY_LONG_BASE+7)
#define KEY_9_LONG_TRIGGER 	(KEY_LONG_BASE+8)
#define KEY_10_LONG_TRIGGER 	(KEY_LONG_BASE+9)
#define KEY_11_LONG_TRIGGER 	(KEY_LONG_BASE+10)
#define KEY_12_LONG_TRIGGER 	(KEY_LONG_BASE+11)
#define KEY_13_LONG_TRIGGER	(KEY_LONG_BASE+12)
#define KEY_14_LONG_TRIGGER 	(KEY_LONG_BASE+13)
#define KEY_15_LONG_TRIGGER 	(KEY_LONG_BASE+14)
#define KEY_16_LONG_TRIGGER 	(KEY_LONG_BASE+15)
#define KEY_17_LONG_TRIGGER 	(KEY_LONG_BASE+16)
#define KEY_18_LONG_TRIGGER 	(KEY_LONG_BASE+17)
#define KEY_19_LONG_TRIGGER 	(KEY_LONG_BASE+18)
#define KEY_20_LONG_TRIGGER 	(KEY_LONG_BASE+19)
#define KEY_21_LONG_TRIGGER 	(KEY_LONG_BASE+20)
#define KEY_22_LONG_TRIGGER 	(KEY_LONG_BASE+21)
#define KEY_23_LONG_TRIGGER	(KEY_LONG_BASE+22)

#define KEY_LONG_LONG_BASE		0X90
#define KEY_LONG_LONG_TRIGGER(X)	(KEY_LONG_LONG_BASE+(X))
#define KEY_1_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+0)
#define KEY_2_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+1)
#define KEY_3_LONG_LONG_TRIGGER	(KEY_LONG_LONG_BASE+2)
#define KEY_4_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+3)
#define KEY_5_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+4)
#define KEY_6_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+5)
#define KEY_7_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+6)
#define KEY_8_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+7)
#define KEY_9_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+8)
#define KEY_10_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+9)
#define KEY_11_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+10)
#define KEY_12_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+11)
#define KEY_13_LONG_LONG_TRIGGER	(KEY_LONG_LONG_BASE+12)
#define KEY_14_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+13)
#define KEY_15_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+14)
#define KEY_16_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+15)
#define KEY_17_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+16)
#define KEY_18_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+17)
#define KEY_19_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+18)
#define KEY_20_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+19)
#define KEY_21_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+20)
#define KEY_22_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+21)
#define KEY_23_LONG_LONG_TRIGGER	(KEY_LONG_LONG_BASE+22)



#define key1_press  (0X20 +3*((1)-1))
#define key1_long_press (key1_press+1)
#define key1_long_long_press	(key1_press+2)

#define key2_press  (0X20 +3*((2)-1))
#define key2_long_press (key2_press+1)
#define key2_long_long_press	(key2_press+2)

#define key3_press  (0X20 +3*((3)-1))
#define key3_long_press (key3_press+1)
#define key3_long_long_press	(key3_press+2)

#define key4_press  (0X20 +3*((4)-1))
#define key4_long_press (key4_press+1)
#define key4_long_long_press	(key4_press+2)

#define key5_press  (0X20 +3*((5)-1))
#define key5_long_press (key5_press+1)
#define key5_long_long_press	(key5_press+2)

#define key6_press  (0X20 +3*((6)-1))
#define key6_long_press (key6_press+1)
#define key6_long_long_press	(key6_press+2)

#define key7_press  (0X20 +3*((7)-1))
#define key7_long_press (key7_press+1)
#define key7_long_long_press	(key7_press+2)

#define key8_press  (0X20 +3*((8)-1))
#define key8_long_press (key8_press+1)
#define key8_long_long_press	(key8_press+2)

#define key9_press  (0X20 +3*((9)-1))
#define key9_long_press (key9_press+1)
#define key9_long_long_press	(key9_press+2)

#define key10_press  (0X20 +3*((10)-1))
#define key10_long_press (key10_press+1)
#define key10_long_long_press	(key10_press+2)

#define key11_press  (0X20 +3*((11)-1))
#define key11_long_press (key11_press+1)
#define key11_long_long_press	(key11_press+2)

#define key12_press  (0X20 +3*((12)-1))
#define key12_long_press (key12_press+1)
#define key12_long_long_press	(key12_press+2)

#define key13_press  (0X20 +3*((13)-1))
#define key13_long_press (key13_press+1)
#define key13_long_long_press	(key13_press+2)

#define key14_press  (0X20 +3*((14)-1))
#define key14_long_press (key14_press+1)
#define key14_long_long_press	(key14_press+2)

#define key15_press  (0X20 +3*((15)-1))
#define key15_long_press (key15_press+1)
#define key15_long_long_press	(key15_press+2)

#define key16_press  (0X20 +3*((16)-1))
#define key16_long_press (key16_press+1)
#define key16_long_long_press	(key16_press+2)

#define key17_press  (0X20 +3*((17)-1))
#define key17_long_press (key17_press+1)
#define key17_long_long_press	(key17_press+2)

#define key18_press  (0X20 +3*((18)-1))
#define key18_long_press (key18_press+1)
#define key18_long_long_press	(key18_press+2)

#define key19_press  (0X20 +3*((19)-1))
#define key19_long_press (key19_press+1)
#define key19_long_long_press	(key19_press+2)

#define key20_press  (0X20 +3*((20)-1))
#define key20_long_press (key20_press+1)
#define key20_long_long_press	(key20_press+2)

#define key21_press  (0X20 +3*((21)-1))
#define key21_long_press (key21_press+1)
#define key21_long_long_press	(key21_press+2)

#define key22_press  (0X20 +3*((22)-1))
#define key22_long_press (key22_press+1)
#define key22_long_long_press	(key22_press+2)

#define key23_press  (0X20 +3*((23)-1))
#define key23_long_press (key23_press+1)
#define key23_long_long_press	(key23_press+2)
///////////////////////////////0X64



#define KEY_NUMBER		1
typedef struct
{
	unsigned char key_point;
	unsigned char key_ok[KEY_NUMBER];
	unsigned char key_value[KEY_NUMBER];
	unsigned char  press_time[KEY_NUMBER];
}key_data_def;
extern key_data_def rtos_key;

extern osMessageQId KeyQueueHandle;

void read_key(void);
void deal_key_value(unsigned key_value);


#endif
