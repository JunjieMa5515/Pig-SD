

/* USER CODE BEGIN Includes */
/**
 ****************************************************************************************************
 * @file        sdio_sdcard.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-04-23
 * @brief       SD����������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 * 
 * ʵ��ƽ̨:����ԭ�� M144Z-M4��Сϵͳ��STM32F407��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 * GPIO_AF12_SDIO
 ****************************************************************************************************
 */

#ifndef __SDIO_SDCARD_H
#define __SDIO_SDCARD_H

#include "main.h"
uint8_t exfuns_init(void);                  /* �����ڴ� */

/* ���Ŷ��� */
#define SD_D0_GPIO_PORT             GPIOC
#define SD_D0_GPIO_PIN              GPIO_PIN_8
#define SD_D0_GPIO_AF               GPIO_AF12_SDIO
#define SD_D0_GPIO_CLK_ENABLE()     do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while (0)
#define SD_D1_GPIO_PORT             GPIOC
#define SD_D1_GPIO_PIN              GPIO_PIN_9
#define SD_D1_GPIO_AF               GPIO_AF12_SDIO
#define SD_D1_GPIO_CLK_ENABLE()     do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while (0)
#define SD_D2_GPIO_PORT             GPIOC
#define SD_D2_GPIO_PIN              GPIO_PIN_10
#define SD_D2_GPIO_AF               GPIO_AF12_SDIO
#define SD_D2_GPIO_CLK_ENABLE()     do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while (0)
#define SD_D3_GPIO_PORT             GPIOC
#define SD_D3_GPIO_PIN              GPIO_PIN_11
#define SD_D3_GPIO_AF               GPIO_AF12_SDIO
#define SD_D3_GPIO_CLK_ENABLE()     do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while (0)
#define SD_SCK_GPIO_PORT            GPIOC
#define SD_SCK_GPIO_PIN             GPIO_PIN_12
#define SD_SCK_GPIO_AF              GPIO_AF12_SDIO
#define SD_SCK_GPIO_CLK_ENABLE()    do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while (0)
#define SD_CMD_GPIO_PORT            GPIOD
#define SD_CMD_GPIO_PIN             GPIO_PIN_2
#define SD_CMD_GPIO_AF              GPIO_AF12_SDIO
#define SD_CMD_GPIO_CLK_ENABLE()    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/* SD��������ʱʱ�䶨�� */
#define SD_DATATIMEOUT              ((uint32_t)100000000)

/* �������� */
extern SD_HandleTypeDef g_sd_handle;
extern HAL_SD_CardInfoTypeDef g_sd_card_info;

/* �������� */
uint8_t sd_init(void);                                              /* ��ʼ��SD�� */
uint8_t sd_read_disk(uint8_t *buf, uint32_t addr, uint32_t count);  /* ��SD��ָ�������Ŀ����� */
uint8_t sd_write_disk(uint8_t *buf, uint32_t addr, uint32_t count); /* дSD��ָ�������Ŀ����� */

#endif

/* USER CODE END Includes */


