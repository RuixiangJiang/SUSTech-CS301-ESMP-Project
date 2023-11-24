/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "utils.h"
#include "touch.h"
#include "string.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "config.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// config for the users
unsigned char user_name[20] = "Jiang";
struct TIME_SETTING time_setting = {2023, 11, 23, 0, 5, 11};
uint16_t mark_seed = 0x1677; // used to check if the RTC is initialized (if you want to reset the RTC, change this value)

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
unsigned char DATA_TO_SEND[800];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

enum SCREEN_STATE screen_state = INITIAL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
extern uint8_t rxBuffer[2000];

void rtp_test(void)
{

  HAL_UART_Transmit(&huart1, (uint8_t*)"HELLO WORLD!\r\n", 14 , 0xFFFF);
  while (1) {
    tp_dev.scan(0);
    if (screen_state == INITIAL) {
      if(tp_dev.sta&TP_PRES_DOWN) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"pressed\r\n", 9 , 0xFFFF);
        // between (10, 150) and (10 + 60, 150 + 60) is the area of the [CHAT] button
        if (tp_dev.x[0] > 10 && tp_dev.x[0] < 70 && tp_dev.y[0] > 150 && tp_dev.y[0] < 210) {
          HAL_UART_Transmit(&huart1, (uint8_t*)"CHAT\r\n", 6 , 0xFFFF);
          screen_state = CHAT;
          draw_chat_screen();
        }
        // between (90, 150) and (90 + 60, 150 + 60) is the area of the [CALCULATOR] button
        else if (tp_dev.x[0] > 90 && tp_dev.x[0] < 150 && tp_dev.y[0] > 150 && tp_dev.y[0] < 210) {
          HAL_UART_Transmit(&huart1, (uint8_t*)"CALCULATOR\r\n", 12 , 0xFFFF);
          screen_state = CALC_D;
          draw_calc_screen();
        }
        // between (170, 150) and (170 + 60, 150 + 60) is the area of the [PICTURE] button
        else if (tp_dev.x[0] > 170 && tp_dev.x[0] < 230 && tp_dev.y[0] > 150 && tp_dev.y[0] < 210) {
          HAL_UART_Transmit(&huart1, (uint8_t*)"PICTURE\r\n", 9 , 0xFFFF);
          screen_state = PIC;
          draw_pic_screen();
        }
        // between (10, 220) and (10 + 60, 220 + 60) is the area of the [Tetris] button
        else if (tp_dev.x[0] > 10 && tp_dev.x[0] < 70 && tp_dev.y[0] > 220 && tp_dev.y[0] < 280) {
          HAL_UART_Transmit(&huart1, (uint8_t*)"Tetris\r\n", 8 , 0xFFFF);
          screen_state = Tetris;
          draw_tetris_screen();
        }
      }
    } else if (screen_state == CALC_B || screen_state == CALC_D || screen_state == CALC_E) {
      calc_touch_screen_handler();
    }
    
  } 
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  LCD_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
  HAL_TIM_Base_Start_IT(&htim3);
  Stm32_Clock_Init(RCC_PLL_MUL9);
  delay_init(72);
  // LED_Init();
  // KEY_Init();
  // LCD_Init();
  tp_dev.init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  draw_initial_screen((uint8_t *) user_name);
  rtp_test();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
