/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "arm_math.h"
#include "arm_const_structs.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
#define FFT_LENGTH  1024
float fft_inputbuf[FFT_LENGTH*2];
float fft_outputbuf[FFT_LENGTH];
/* USER CODE END Variables */
/* Definitions for main */
osThreadId_t mainHandle;
const osThreadAttr_t main_attributes = {
        .name = "main",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 64 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void App_main(void *argument);

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
    /* creation of main */
    mainHandle = osThreadNew(App_main, NULL, &main_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_App_main */
/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_App_main */
void App_main(void *argument) {
    /* USER CODE BEGIN App_main */
    LCD_ShowString(30,0,"DSP test",BLACK,WHITE,24,0);
    int i = 0;
    //gennerat signal
    for (i = 0; i < FFT_LENGTH; i++) {
        fft_inputbuf[2 * i] = 100 + 10 * arm_sin_f32(2 * PI * i * 10 / FFT_LENGTH) + 20 * arm_sin_f32(2 * PI * i * 50 / FFT_LENGTH) + 30 * arm_cos_f32(2 * PI * i * 300 / FFT_LENGTH);
        fft_inputbuf[2 * i + 1] = 0;
    }

//    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
//
//    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputbuf, 0, 1);
//    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);
//
//    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);


    /* Infinite loop */
    for (;;) {

        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);

        arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputbuf, 0, 1);
        arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);

        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//        vTaskDelay(1);
    }
    /* USER CODE END App_main */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

