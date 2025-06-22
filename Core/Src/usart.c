/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "ff.h"
#include "stdio.h"
#include "common.h"
/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t g_usart1_rx_buf[USART_REC_LEN];
uint8_t g_usart3_rx_buf[USART_REC_LEN];
uint8_t g_usart6_rx_buf[USART_REC_LEN];
uint8_t g_usart3_rx_buf2[18];
uint8_t g_usart6_rx_buf2[18];

unsigned short int CRC_7;
int found = 0;  

//存加速度的包
float usart_acZ0,usart_acZ1,usart_acZ2;
//存速度的包
float usart_vol;
float anl;
float anl_old=0.0f;

float cic_span;

uint16_t move_acc[3];

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
 *  bit13~0，    接收到的有效字节数目
*/
uint16_t g_usart1_rx_sta = 0;
uint16_t g_usart3_rx_sta = 0;
uint16_t g_usart6_rx_sta = 0;

uint8_t g_rx1_buffer[RXBUFFERSIZE];                  /* HAL库使用的串口接收缓冲 */
uint8_t g_rx3_buffer[11];
uint8_t g_rx6_buffer[9];
static uint8_t ReceiveData_All[6];

unsigned char ReceiveData6_All[9];
unsigned char ReceiveData3_All[9];

void number_task(void)
{
	float a[3];
	move_acc[0] = ((uint16_t)ReceiveData_All[1]<<8)|(uint16_t)ReceiveData_All[0];
	move_acc[1] = ((uint16_t)ReceiveData_All[3]<<8)|(uint16_t)ReceiveData_All[2];
	move_acc[2] = ((uint16_t)ReceiveData_All[5]<<8)|(uint16_t)ReceiveData_All[4];
	
   a[0] = move_acc[0]/32769.0f*156.8f;
   a[1] = move_acc[1]/32769.0f*156.8f;
   a[2] = move_acc[2]/32769.0f*156.8f;
for(int i = 0;i<3;i++)
	{
	if(a[i] <156.8f)
	{
	a[i] = a[i]+313.6f;
	}
	}
	usart_acZ0 = a[0];
	usart_acZ1 = a[1];
	usart_acZ2 = a[2];
	

}
/* USER CODE END 0 */

UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
       HAL_UART_Receive_IT(&huart3, (uint8_t *)g_rx3_buffer, 11);

  /* USER CODE END USART3_Init 2 */

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */
     HAL_UART_Receive_IT(&huart6, (uint8_t *)g_rx6_buffer, RXBUFFERSIZE6);

  /* USER CODE END USART6_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9|GPIO_PIN_14);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief       Rx传输回调函数
 * @param       huart: UART句柄类型指针
 * @retval      无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
int8_t i=0,j=0;

	int8_t x =0,y=0;
    
	if(huart->Instance == USART3)             /* 如果是串口1 */
    {
        if((g_usart3_rx_sta & 0x8000) == 0)      /* 接收未完成 */
        {
            if(g_usart3_rx_sta & 0x4000)         /* 接收到了0x0d */
            {
                if(g_rx3_buffer[0] != 0x0a) 
                {
                    g_usart3_rx_sta = 0;         /* 接收错误,重新开始 */
                }
                else 
                {
                    g_usart3_rx_sta |= 0x8000;   /* 接收完成了 */
                }
            }
            else                                /* 还没收到0X0D */
            {
                if(g_rx3_buffer[0] == 0x0d)
                {
                    g_usart3_rx_sta |= 0x4000;
                }
                else
                {
                    g_usart3_rx_buf[g_usart3_rx_sta & 0X3FFF] = g_rx3_buffer[0] ;
                    g_usart3_rx_sta++;
                    if(g_usart3_rx_sta > (USART_REC_LEN - 1))
                    {
                        g_usart3_rx_sta = 0;     /* 接收数据错误,重新开始接收 */
                    }
                }
            }
        }
        HAL_UART_Receive_IT(&huart3, (uint8_t *)g_rx3_buffer, 11);
	for(i=0;i<11;i++)
		{
		if(g_rx3_buffer[i]==0x55)
		{		
			if(i<10)
			{
				if(g_rx3_buffer[i+1]==0x51)j=i;
						}
			else
			{
			if(g_rx3_buffer[0]==0x51)j=i;}
		}
		}
		
			if(j+2<11) ReceiveData_All[0]=g_rx3_buffer[j+2];
			else ReceiveData_All[0]=g_rx3_buffer[j+2-11];
			
		if(j+3<11) ReceiveData_All[1]=g_rx3_buffer[j+3];
			else ReceiveData_All[1]=g_rx3_buffer[j+3-11];
			
				if(j+4<11) ReceiveData_All[2]=g_rx3_buffer[j+4];
			else ReceiveData_All[2]=g_rx3_buffer[j+4-11];
			
				if(j+5<11) ReceiveData_All[3]=g_rx3_buffer[j+5];
			else ReceiveData_All[3]=g_rx3_buffer[j+5-11];
			if(j+6<11) ReceiveData_All[4]=g_rx3_buffer[j+6];
			else ReceiveData_All[4]=g_rx3_buffer[j+6-11];
			if(j+7<11) ReceiveData_All[5]=g_rx3_buffer[j+7];
			else ReceiveData_All[5]=g_rx3_buffer[j+7-11];
				number_task();

    }
	 
	 
	 
	 
	   else  if(huart->Instance == USART6)             /* 如果是串口6 */
    {
      if((g_usart6_rx_sta & 0x8000) == 0)      /* 接收未完成 */
        {
            if(g_usart6_rx_sta & 0x4000)         /* 接收到了0x0d */
            {
                if(g_rx6_buffer[0] != 0x0a) 
                {
                    g_usart6_rx_sta = 0;         /* 接收错误,重新开始 */
                }
                else 
                {
                    g_usart6_rx_sta |= 0x8000;   /* 接收完成了 */
                }
            }
            else                                /* 还没收到0X0D */
            {
                if(g_rx6_buffer[0] == 0x0d)
                {
                    g_usart6_rx_sta |= 0x4000;
                }
                else if(g_rx6_buffer[0] == 0x55)
                {
                    g_usart6_rx_buf[g_usart6_rx_sta & 0X3FFF] = g_rx6_buffer[0] ;
                    g_usart6_rx_sta++;
                    if(g_usart6_rx_sta > (USART6_REC_LEN - 1))
                    {
                        g_usart6_rx_sta = 0;     /* 接收数据错误,重新开始接收 */
                    }
                }
            }
        }
		  

		  HAL_UART_Receive_IT(&huart6, (uint8_t *)g_rx6_buffer, RXBUFFERSIZE6);


		  g_usart6_rx_buf2[0] = g_rx6_buffer[0];
        g_usart6_rx_buf2[1] = g_rx6_buffer[1];
        g_usart6_rx_buf2[2] = g_rx6_buffer[2];
        g_usart6_rx_buf2[4] = g_rx6_buffer[4];
        g_usart6_rx_buf2[5] = g_rx6_buffer[5];
        g_usart6_rx_buf2[6] = g_rx6_buffer[6];
        g_usart6_rx_buf2[7] = g_rx6_buffer[7];
        g_usart6_rx_buf2[8] = g_rx6_buffer[8];
        g_usart6_rx_buf2[9] = g_rx6_buffer[0];
        g_usart6_rx_buf2[10] = g_rx6_buffer[1];
        g_usart6_rx_buf2[11] = g_rx6_buffer[2];
        g_usart6_rx_buf2[12] = g_rx6_buffer[3];
        g_usart6_rx_buf2[13] = g_rx6_buffer[4];
        g_usart6_rx_buf2[14] = g_rx6_buffer[5];
        g_usart6_rx_buf2[15] = g_rx6_buffer[6];
        g_usart6_rx_buf2[16] = g_rx6_buffer[7];
        g_usart6_rx_buf2[17] = g_rx6_buffer[8];
		 for (x = 0; x < 18; x++) {
    if (g_usart6_rx_buf2[x] == 0x01 && g_usart6_rx_buf2[x + 1] == 0x03) {
        for (y = 0; y < 9; y++) {
            ReceiveData6_All[y] = g_usart6_rx_buf2[x + y];
        }
        CRC_7 = Crc_Count(ReceiveData6_All, 6);
        if (CRC_7 == (ReceiveData6_All[7] + (ReceiveData6_All[8] << 8))) {
            found = 1; 
            break;  
        }
    }
    if (found) break;  
}
anl_old = anl;

			 CRC_7 = Crc_Count(ReceiveData6_All, 7); // ??CRC?
if (CRC_7 == (ReceiveData6_All[7] + (ReceiveData6_All[8] << 8))) // ??CRC?
{
 
        anl = ((ReceiveData6_All[3]<<24)|(ReceiveData6_All[4]<<16)|(ReceiveData6_All[5]<<8)|(ReceiveData6_All[6]))/4096.0f;
    
}

//      anl = ((ReceiveData6_All[0]<<24)|(ReceiveData6_All[1]<<16)|(ReceiveData6_All[2]<<8)|(ReceiveData6_All[3]))/4096.0f/(0.1f/60.0f)/60.0f;
//////      anl = Encoder_Data/4096.0f/(0.1f/60.0f)/60.0f;

		usart_vol = (anl-anl_old)*6.28f*5.0f/0.05f;//单位厘米每秒
	  }

}



/* USER CODE END 1 */
