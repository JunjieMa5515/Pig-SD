/*********************************************************************************
FILE NAME:sdio_sdcard.h

SDIO�ӿ�SD�����������HAL��ʹ��
									��֧�ֵ���SD��
									��������ԭ��̽���߿���������
									
									��Fatfs�ǵ����ɹ���ʱ��Heap�ռ䣡������
									SDIOʱ��Ƶ�ʲ�Ҫ����25MHz
									����SDIOʱ�ǵ���CubeMX���̸߼�ѡ�ѡ��Ҫ����SDIO��ʼ��������
				
				From:@rjgawuie
*********************************************************************************/
#include "main.h"
#ifndef _SDMMC_SDCARD_H
#define _SDMMC_SDCARD_H

#define SD_TIMEOUT 			((uint32_t)100000000)  	//��ʱʱ��
#define SD_TRANSFER_OK     	((uint8_t)0x00)
#define SD_TRANSFER_BUSY   	((uint8_t)0x01)

extern HAL_SD_CardInfoTypeDef  SDCardInfo;         //SD����Ϣ�ṹ��

uint8_t  SD_Init(void);
uint8_t  SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo);
uint8_t  SD_GetCardState(void);
uint8_t  SD_ReadDisk(uint8_t * buf,uint32_t  sector,uint32_t  cnt);
uint8_t  SD_WriteDisk(uint8_t  *buf,uint32_t  sector,uint32_t  cnt);



#endif
