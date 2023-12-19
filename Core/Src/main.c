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
#include "spi.h"
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
#include "tetris.h"
#include "delay.h"
#include "config.h"

#include "led.h"
#include "key.h"
#include "malloc.h"
#include "MMC_SD.h"
#include "ff.h"
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"
#include "24cxx.h"
#include "remote.h"
#include "24l01.h"
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// config for the users
unsigned char user_name[20] = "Bendan Luo";
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
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim3;

u8 res;
DIR picdir;	 						//图片目录
FILINFO *picfileinfo;				//文件信息
u8 *pname;							//带路径的文件????
u16 totpicnum; 						//图片文件总数
u16 curindex;						//图片当前索引
u8 key,mode;							//键???
u8 pause=0;							//暂停标记
u16 temp;
u32 *picoffsettbl;					//图片文件offset索引????
u16 t=0;
u8 tmp_buf[33];

unsigned char txBuffer[1024] = {0};
int txLen;

u16 pic_get_tnum(u8 *path)
{
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO *tfileinfo;	//临时文件信息
	tfileinfo=(FILINFO*)mymalloc(sizeof(FILINFO));//申请内存
    res=f_opendir(&tdir,(const TCHAR*)path); 	//打开目录
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//查询总的有效文件????
		{
	        res=f_readdir(&tdir,tfileinfo);       		//读取目录下的????个文????
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//错误????/到末尾了,????????
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文????
			{
				rval++;//有效文件数增????1
			}
		}
	}
	myfree(tfileinfo);//释放内存
	return rval;
}
void album(){
  while(f_opendir(&picdir,"0:/PICTURE"))//打开图片文件夹
   	{
  		LCD_ShowString(30,170,240,16, 16, (uint8_t*)"/PICTURE is wrong!");
  		delay_ms(200);
  		LCD_Fill(30,170,240,186,WHITE);//清除显示
  		delay_ms(200);
  	}
  	totpicnum=pic_get_tnum("0:/PICTURE"); //得到总有效文件数
    	while(totpicnum==NULL)//图片文件为0
   	{
  		LCD_ShowString(30,170,240,16, 16, (uint8_t*)"No picture!");
  		delay_ms(200);
  		LCD_Fill(30,170,240,186,WHITE);//清除显示
  		delay_ms(200);
  	}
  	picfileinfo=(FILINFO*)mymalloc(sizeof(FILINFO));	//申请内存
   	pname=mymalloc(_MAX_LFN*2+1);					//为带路径的文件名分配内存
   	picoffsettbl=mymalloc(4*totpicnum);				//申请4*totpicnum个字节的内存,用于存放图片索引
   	while(!picfileinfo||!pname||!picoffsettbl)				//内存分配出错
   	{
  		LCD_ShowString(30,170,240,16, 16, (uint8_t*)"Fail to allocate memory!");
  		delay_ms(200);
  		LCD_Fill(30,170,240,186,WHITE);//清除显示
  		delay_ms(200);
  	}
  	//记录索引
      res=f_opendir(&picdir,"0:/PICTURE"); //打开目录
  	if(res==FR_OK)
  	{
  		curindex=0;//当前索引为0
  		while(1)//全部查询一遍
  		{
  			temp=picdir.dptr;								//记录当前dptr偏移
  	        res=f_readdir(&picdir,picfileinfo);       		//读取目录下的一个文件
  	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出
  			res=f_typetell((u8*)picfileinfo->fname);
  			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件
  			{
  				picoffsettbl[curindex]=temp;//记录索引
  				curindex++;
  			}
  		}
  	}
    LCD_ShowString(30,170,240,16, 16, (uint8_t*)"Start showing...");
  	delay_ms(1500);
  	piclib_init();										//初始化画图
  	curindex=0;											//从0开始显示
     	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//打开目录
  	while(res==FR_OK)//打开成功
  	{
  		dir_sdi(&picdir,picoffsettbl[curindex]);			//改变当前目录索引
          res=f_readdir(&picdir,picfileinfo);       		//读取目录下的一个文件
          if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出
  		strcpy((char*)pname,"0:/PICTURE/");				//复制路径(目录)
  		strcat((char*)pname,(const char*)picfileinfo->fname);//将文件名接在后面
   		LCD_Clear(BLACK);
   		ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//显示图片
  		Show_Str(2,2,lcddev.width,16,pname,16,1); 				//显示图片名字
  		t=0;
  		while(1)
  		{
  			key=KEY_Scan(0);		//扫描按键
  			if(t>250)key=1;			//模拟一次按下KEY0
  			if((t%20)==0)LED0=!LED0;//LED0闪烁,提示程序正在运行.
  			if(key==KEY1_PRES)		//上一张
  			{
  				if(curindex)curindex--;
  				else curindex=totpicnum-1;
  				break;
  			}else if(key==KEY0_PRES)//下一张
  			{
  				curindex++;
  				if(curindex>=totpicnum)curindex=0;//到末尾的时候,自动从头开始
  				break;
  			}else if(key==WKUP_PRES)
  			{
  				pause=!pause;
  				LED1=!pause; 	//暂停的时候LED1亮.
  			}
  			if(pause==0)t++;
  			delay_ms(10);
  		}
  		res=0;
  	}
  	myfree(picfileinfo);			//释放内存
  	myfree(pname);				//释放内存
  	myfree(picoffsettbl);		//释放内存
}
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
          album();
        }
        // between (10, 220) and (10 + 60, 220 + 60) is the area of the [Tetris] button
        else if (tp_dev.x[0] > 10 && tp_dev.x[0] < 70 && tp_dev.y[0] > 220 && tp_dev.y[0] < 280) {
          HAL_UART_Transmit(&huart1, (uint8_t*)"Tetris\r\n", 8 , 0xFFFF);
          screen_state = Tetris;
          tetris();
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
  LED_Init();							//初始化LED
  KEY_Init();							//初始化按????
  mem_init();							//初始化内存池
  exfuns_init();						//为fatfs相关变量申请内存
	f_mount(fs[0],"0:",1); 				//挂载SD????
	f_mount(fs[1],"1:",1); 				//挂载FLASH.
  Remote_Init();
	NRF24L01_Init();    		    	//初始化NRF24L01
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_SPI1_Init();
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
