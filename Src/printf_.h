
#ifndef __PRINTF_H__
#define __PRINTF_H__
#include "include.h"


#define DEBUG_NONE  	0
#define DEBUG_USER	1
#define DEBUG_SYS	2

#define DEBUG DEBUG_SYS
#if (DEBUG > 0)

#define  DEBUG_USR_ErrLog(...)   printf("USR_DEBUG: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\r\n");
#else
#endif

#if (DEBUG > 1)

#define  DEBUG_SYS_ErrLog(...)   printf("SYS_DEBUG: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\r\n");
#else


#endif


#endif
