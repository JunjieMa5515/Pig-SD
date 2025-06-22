#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H
#include "main.h"


#define Jetson_CAN hcan1



typedef enum
{
    CAN_Jetson_ALL_ID = 0x200,//底盘can2


    CAN_Stm32_ALL_ID = 0x1FF,

	capid = 0x211,//超级电容回传
} can_msg_id_e;


typedef struct
{
    uint16_t speed_set;
    int16_t speed_get;
    int16_t ang;
    uint16_t temperate;

} Jetson_measure_t;


void CAN_cmd(int16_t acc0, int16_t acc1, int16_t vol, int16_t pull);





























#endif
