#include "CAN_receive.h"
#include "cmsis_os.h"
#include "main.h"



extern CAN_HandleTypeDef hcan1;


static CAN_TxHeaderTypeDef  Jetson_tx_message;
static uint8_t              Jetson_can_send_data[8];

 Jetson_measure_t Motor_z;


//jetson data read
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->speed_set = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_get = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->ang = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (uint16_t)((data)[6] << 8 | (data)[7]);                                  \
    }


//发送数据
void CAN_cmd(int16_t acc0, int16_t acc1, int16_t vol, int16_t pull)
{
    uint32_t send_mail_box;
    Jetson_tx_message.StdId = CAN_Jetson_ALL_ID;
    Jetson_tx_message.IDE = CAN_ID_STD;
    Jetson_tx_message.RTR = CAN_RTR_DATA;
    Jetson_tx_message.DLC = 0x08;
    Jetson_can_send_data[0] = (acc0 >> 8);
    Jetson_can_send_data[1] = acc0;
    Jetson_can_send_data[2] = (acc1 >> 8);
    Jetson_can_send_data[3] = acc1;
    Jetson_can_send_data[4] = (vol >> 8);
    Jetson_can_send_data[5] = vol;
    Jetson_can_send_data[6] = (pull >> 8);
    Jetson_can_send_data[7] = pull;
    HAL_CAN_AddTxMessage(&Jetson_CAN, &Jetson_tx_message, Jetson_can_send_data, &send_mail_box);
}




/**
  * @brief          hal CAN fifo call back, receive Jetson data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)//can1
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);

    switch (rx_header.StdId)
    {
        case CAN_Stm32_ALL_ID:
        {
			 get_motor_measure(&Motor_z, rx_data);

			  
            break;
        }

        default:
        {
            break;
        }
    }
}
