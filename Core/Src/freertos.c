/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "CAN_receive.h"
#include "common.h"
#include "gpio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t g_tx3_buffer[8]={0x01,0x06,0x00,0x11,0x00,0x02,0x58,0x0E};
uint8_t SD_state = 1;		
uint8_t SD_txtNumber = 0;	
typedef struct {
    unsigned char high_byte;
    unsigned char low_byte;
}CRC_Result;

 CRC_Result result;
extern int8_t SD_exist;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//CRC校验，用于编码器校验位
unsigned int Crc_Count(unsigned char pbuf[],unsigned char num)
{
   int i,j;
   unsigned int wcrc=0xffff;
   for(i=0;i<num;i++)
   {
     wcrc^=(unsigned int)(pbuf[i]);
		 for (j=0;j<8;j++)
		 {
				if(wcrc&0x0001)
			{
				 wcrc>>=1;
				 wcrc^=0xa001;
			}
			else wcrc>>=1;		
		 }
   }   
   return wcrc;
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Motor */
osThreadId_t MotorHandle;
const osThreadAttr_t Motor_attributes = {
  .name = "Motor",
  .stack_size = 5120 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ad */
osThreadId_t AdHandle;
const osThreadAttr_t Ad_attributes = {
  .name = "Ad",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SDCard */
osThreadId_t SDCardHandle;
const osThreadAttr_t SDCard_attributes = {
  .name = "SDCard",
  .stack_size = 5120 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Motor1(void *argument);
void StartTask04(void *argument);
void SDcard(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Motor */
  MotorHandle = osThreadNew(Motor1, NULL, &Motor_attributes);

  /* creation of Ad */
  AdHandle = osThreadNew(StartTask04, NULL, &Ad_attributes);

  /* creation of SDCard */
  SDCardHandle = osThreadNew(SDcard, NULL, &SDCard_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  { HAL_ADC_Start_IT(&hadc1);

    osDelay(20);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Motor1 */
/**
* @brief Function implementing the Motor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Motor1 */
void Motor1(void *argument)
{
  /* USER CODE BEGIN Motor1 */
  /* Infinite loop */
//	 gtim_timx_pwm_chy_init(500 - 1, 64 - 1);    /* 初始化通用定时器PWM，PWM频率为2KHz TIM3定时器频率84MKZ /预分频+1*预装载值+1*/

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

  for(;;)
  {
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
//CAN_cmd(229,335,443,558);
    osDelay(60);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
	 osDelay(60);

  }
  /* USER CODE END Motor1 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the Ad thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_SDcard */
/**
* @brief Function implementing the SDCard thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SDcard */
void SDcard(void *argument)
{
  /* USER CODE BEGIN SDcard */
  /* Infinite loop */
  for(;;)
  {  if(SD_exist==1&&SD_state == 1&&HAL_GPIO_ReadPin(Stop_SD_GPIO_Port, Stop_SD_Pin) != GPIO_PIN_SET)
	{
		Para_init();
	   SD_state=SD_state-1;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);

	}
   common_loop();
     if(HAL_GPIO_ReadPin(Stop_SD_GPIO_Port, Stop_SD_Pin) == GPIO_PIN_SET && SD_state == 0) 
{  SD_state = 1;
   Para_end();//通过按键检测关闭文件传输
	SD_txtNumber++;
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);

}
	  
 osDelay(60);
  }
  
  /* USER CODE END SDcard */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

