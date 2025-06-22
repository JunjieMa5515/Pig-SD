
/* USER CODE BEGIN 0 */
/**
 ****************************************************************************************************
 * @file        sdio_sdcard.c
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
 * 
 ****************************************************************************************************
 */
#include "ff.h"
#include "sdio.h"
#include "string.h"
#include "stdlib.h"	
FATFS *fs[2];

/**
 * @brief       Ϊexfuns�����ڴ�
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��.
 */
uint8_t exfuns_init(void)
{
    uint8_t i;
    uint8_t res = 0;

    for (i = 0; i < FF_VOLUMES; i++)
    {
        fs[i] = (FATFS *)malloc( sizeof(FATFS));   /* Ϊ����i�����������ڴ� */

        if (!fs[i])break;
    }
    
#if USE_FATTESTER == 1  /* ���ʹ�����ļ�ϵͳ���� */
    res = mf_init();    /* ��ʼ���ļ�ϵͳ����(�����ڴ�) */
#endif
    
    if (i == FF_VOLUMES && res == 0)
    {
        return 0;   /* ������һ��ʧ��,��ʧ��. */
    }
    else 
    {
        return 1;
    }
}

/* SD��� */
SD_HandleTypeDef g_sd_handle = {0};

/* SD��Ϣ */
HAL_SD_CardInfoTypeDef g_sd_card_info = {0};

/**
 * @brief   ��ʼ��SD��
 * @param   ��
 * @retval  ��ʼ�����
 * @arg     0: ��ʼ���ɹ�
 * @arg     1: ��ʼ��ʧ��
 * @arg     2: ���߿������ʧ��
 */
uint8_t sd_init(void)
{
    /* ����SD */
    g_sd_handle.Instance = SDIO;
    g_sd_handle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    g_sd_handle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    g_sd_handle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    g_sd_handle.Init.BusWide = SDIO_BUS_WIDE_1B;
    g_sd_handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    g_sd_handle.Init.ClockDiv = 1;
    if (HAL_SD_Init(&g_sd_handle) != HAL_OK)
    {
        return 1;
    }
    
    /* ��ȡSD��Ϣ */
    HAL_SD_GetCardInfo(&g_sd_handle, &g_sd_card_info);
    
    /* ����4bit���߿�� */
    if (HAL_SD_ConfigWideBusOperation(&g_sd_handle, SDIO_BUS_WIDE_4B) != HAL_OK)
    {
        return 2;
    }
    
    return 0;
}

/**
 * @brief   HAL��SD��ʼ��MSP����
 * @param   hsd: SD���
 * @retval  ��
 */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    if (hsd->Instance == SDIO)
    {
        __HAL_RCC_SDIO_CLK_ENABLE();
        SD_D0_GPIO_CLK_ENABLE();
        SD_D1_GPIO_CLK_ENABLE();
        SD_D2_GPIO_CLK_ENABLE();
        SD_D3_GPIO_CLK_ENABLE();
        SD_SCK_GPIO_CLK_ENABLE();
        SD_CMD_GPIO_CLK_ENABLE();
        
        /* ����D0���� */
        gpio_init_struct.Pin = SD_D0_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = SD_D0_GPIO_AF;
        HAL_GPIO_Init(SD_D0_GPIO_PORT, &gpio_init_struct);
        
        /* ����D1���� */
        gpio_init_struct.Pin = SD_D1_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = SD_D1_GPIO_AF;
        HAL_GPIO_Init(SD_D1_GPIO_PORT, &gpio_init_struct);
        
        /* ����D2���� */
        gpio_init_struct.Pin = SD_D2_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = SD_D2_GPIO_AF;
        HAL_GPIO_Init(SD_D2_GPIO_PORT, &gpio_init_struct);
        
        /* ����D3���� */
        gpio_init_struct.Pin = SD_D3_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = SD_D3_GPIO_AF;
        HAL_GPIO_Init(SD_D3_GPIO_PORT, &gpio_init_struct);
        
        /* ����SCK���� */
        gpio_init_struct.Pin = SD_SCK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = SD_SCK_GPIO_AF;
        HAL_GPIO_Init(SD_SCK_GPIO_PORT, &gpio_init_struct);
        
        /* ����CMD���� */
        gpio_init_struct.Pin = SD_CMD_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = SD_CMD_GPIO_AF;
        HAL_GPIO_Init(SD_CMD_GPIO_PORT, &gpio_init_struct);
    }
}

/**
 * @brief   ��ȡSD����Ϣ
 * @param   info: SD����Ϣ
 * @retval  ��ȡ���
 * @arg     0:��ȡ�ɹ�
 * @arg     1:��ȡʧ��
 */
uint8_t sd_get_card_info(HAL_SD_CardInfoTypeDef *info)
{
    if (HAL_SD_GetCardInfo(&g_sd_handle, info) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief   ��SD��ָ�������Ŀ�����
 * @param   buf: ���ݱ������ʼ��ַ
 * @param   addr: ���ַ
 * @param   count: ������
 * @retval  ��ȡ���
 * @arg     0: ��ȡ�ɹ�
 * @arg     1: ��ȡʧ��
 */
uint8_t sd_read_disk(uint8_t *buf, uint32_t addr, uint32_t count)
{
    uint32_t timeout = SD_DATATIMEOUT;
    
    if (HAL_SD_ReadBlocks(&g_sd_handle, buf, addr, count, SD_DATATIMEOUT) != HAL_OK)
    {
        return 1;
    }
    
    while ((HAL_SD_GetCardState(&g_sd_handle) != HAL_SD_CARD_TRANSFER) && (--timeout != 0));
    
    if (timeout == 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief   дSD��ָ�������Ŀ�����
 * @param   buf: ���ݱ������ʼ��ַ
 * @param   addr: ���ַ
 * @param   count: ������
 * @retval  д����
 * @arg     0: д��ɹ�
 * @arg     1: д��ʧ��
 */
uint8_t sd_write_disk(uint8_t *buf, uint32_t addr, uint32_t count)
{
    uint32_t timeout = SD_DATATIMEOUT;
    
    if (HAL_SD_WriteBlocks(&g_sd_handle, buf, addr, count, SD_DATATIMEOUT) != HAL_OK)
    {
        return 1;
    }
    
    while ((HAL_SD_GetCardState(&g_sd_handle) != HAL_SD_CARD_TRANSFER) && (--timeout != 0));
    
    if (timeout == 0)
    {
        return 1;
    }
    
    return 0;
}

/* USER CODE END 0 */
