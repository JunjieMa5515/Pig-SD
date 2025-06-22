#ifndef __COMMON_H__
#define __COMMON_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "sdio.h"
#include "stdio.h"
#include "usart.h"
#include "adc.h"
#include "CircleBuf.h"
#include "ff.h"
 
int Para_Save(void);
int Voltage_Pulling(void);
void common_loop(void);

void Voltage_Pull(void);
int Para_Save(void);
void common_loop(void);
void SD_Insert(void);
void	Para_init(void);
int Para_end(void);


void number_task(void);





#ifdef __cplusplus
}
#endif
#endif
