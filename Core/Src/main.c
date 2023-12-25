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
//#include "remote.h"
#include "24l01.h"
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// config for the users
unsigned char user_name[20] = "Rose";
struct TIME_SETTING time_setting = {2023, 11, 23, 0, 5, 11};
uint16_t mark_seed = 0x1677; // used to check if the RTC is initialized (if you want to reset the RTC, change this value)
int time24 = 0;

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
DIR picdir;	 						//ͼƬĿ¼
FILINFO *picfileinfo;				//�ļ���Ϣ
u8 *pname;							//��·�����ļ�????
u16 totpicnum; 						//ͼƬ�ļ�����
u16 curindex;						//ͼƬ��ǰ����
u8 key,mode;							//��???
u8 pause=0;							//��ͣ���
u16 temp;
u32 *picoffsettbl;					//ͼƬ�ļ�offset����????
u16 t=0;
//u8 tmp_buf[33];

unsigned char txBuffer[1024] = {0};
int txLen;


u16 pic_get_tnum(u8 *path)
{
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO *tfileinfo;	//��ʱ�ļ���Ϣ
	tfileinfo=(FILINFO*)mymalloc(sizeof(FILINFO));//�����ڴ�
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//��ѯ�ܵ���Ч�ļ�????
		{
	        res=f_readdir(&tdir,tfileinfo);       		//��ȡĿ¼�µ�????����????
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//����????/��ĩβ��,????????
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ��????
			{
				rval++;//��Ч�ļ�����????1
			}
		}
	}
	myfree(tfileinfo);//�ͷ��ڴ�
	return rval;
}
void album(){
	NRF24L01_RX_Mode();
	check2();
	check();
  while(f_opendir(&picdir,"0:/PICTURE"))//��ͼƬ�ļ���
   	{
  		LCD_ShowString(10,170,240,16, 16, (uint8_t*)"/PICTURE is wrong!");
  		delay_ms(200);
  		LCD_Fill(30,170,240,186,WHITE);//�����ʾ
  		delay_ms(200);
  	}
  	totpicnum=pic_get_tnum("0:/PICTURE"); //�õ�����Ч�ļ���
    if(totpicnum==NULL)//ͼƬ�ļ�Ϊ0
   	{
  		LCD_ShowString(10,300,240,16, 16, (uint8_t*)"No picture!");
  		delay_ms(200);
  		LCD_Fill(30,170,240,186,WHITE);//�����ʾ
  		delay_ms(200);
  	}
  	picfileinfo=(FILINFO*)mymalloc(sizeof(FILINFO));	//�����ڴ�
   	pname=mymalloc(_MAX_LFN*2+1);					//Ϊ��·�����ļ��������ڴ�
   	picoffsettbl=mymalloc(4*totpicnum);				//����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
   	while(!picfileinfo||!pname||!picoffsettbl)				//�ڴ�������
   	{
  		LCD_ShowString(10,300,240,16, 16, (uint8_t*)"Fail to allocate memory!");
  		delay_ms(200);
  		LCD_Fill(30,170,240,186,WHITE);//�����ʾ
  		delay_ms(200);
  	}
  	//��¼����
      res=f_opendir(&picdir,"0:/PICTURE"); //��Ŀ¼
  	if(res==FR_OK)
  	{
  		curindex=0;//��ǰ����Ϊ0
  		while(1)//ȫ����ѯһ��
  		{
  			check2();
  			check();
  			temp=picdir.dptr;								//��¼��ǰdptrƫ��
  	        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
  	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�
  			res=f_typetell((u8*)picfileinfo->fname);
  			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�
  			{
  				picoffsettbl[curindex]=temp;//��¼����
  				curindex++;
  			}
  		}
  	}
    LCD_ShowString(10,300,240,16, 16, (uint8_t*)"Start showing...");
  	delay_ms(1500);
  	piclib_init();										//��ʼ����ͼ
  	curindex=0;											//��0��ʼ��ʾ
     	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//��Ŀ¼
  	while(res==FR_OK)//�򿪳ɹ�
  	{
  		check2();
  		check();
  		dir_sdi(&picdir,picoffsettbl[curindex]);			//�ı䵱ǰĿ¼����
          res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
          if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�
  		strcpy((char*)pname,"/PICTURE/");				//����·��(Ŀ¼)
  		strcat((char*)pname,(const char*)picfileinfo->fname);//���ļ������ں���
   		LCD_Clear(WHITE);
   		ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//��ʾͼƬ

      POINT_COLOR = BLUE;
      LCD_ShowString(10, 10, lcddev.width, 16, 16, (uint8_t*)pname);
      char PicIndex[40] = {0};
      sprintf(PicIndex, "This picture is %d/%d.", curindex + 1, totpicnum);
      LCD_ShowString(10, 30, 200, 16, 16, (uint8_t*)PicIndex);
      
  		t=0;
  		while(1)
  		{
  			check();
  			key=KEY_Scan(0);		//ɨ�谴��
  			if(t>250)key=1;			//ģ��һ�ΰ���KEY0
  			if((t%20)==0)LED0=!LED0;//LED0��˸,��ʾ������������.
  			if(key==KEY1_PRES)		//��һ��
  			{
  				if(curindex)curindex--;
  				else curindex=totpicnum-1;
  				break;
  			}else if(key==KEY0_PRES)//��һ��
  			{
  				curindex++;
  				if(curindex>=totpicnum)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
  				break;
  			}else if(key==WKUP_PRES)
  			{
  				pause=!pause;
  				LED1=!pause; 	//��ͣ��ʱ��LED1��.
  				screen_state == INITIAL;
  				break;
  			}
  			if(pause==0)t++;
  			delay_ms(10);
  		}
  		res=0;
  	}
  	myfree(picfileinfo);			//�ͷ��ڴ�
  	myfree(pname);				//�ͷ��ڴ�
  	myfree(picoffsettbl);		//�ͷ��ڴ�
}

extern struct User friend1;
int invite = 0;
int chat_remind = 0;
void rtp_test(void)
{

  NRF24L01_RX_Mode();
  while (1) {


//	check2();
//	if(friend1.status==1){
//		LCD_Fill(50, 10, 100, 60, WHITE);
//		LCD_ShowString(60,20,200,24,16,(uint8_t*)"Jack is online!");
//	}else{
//		LCD_Fill(50, 10, 100, 60, WHITE);
//	}

	u8 check_buf[33];
	if(NRF24L01_RxPacket(check_buf)==0){
		char check_buf_substr[7]; // 5字符 + 1 null 终止符
		memcpy(check_buf_substr, check_buf, 6);
		check_buf_substr[6] = '\0'; // 添加 null 终止符
		if (strcmp(check_buf_substr, "INVITE") == 0) {
			LCD_Fill(50, 10, 100, 40, WHITE);
			char invite[300];
			POINT_COLOR = RED;
			sprintf(invite, "%s %s",friend1.name,"invite you!");
			LCD_ShowString(60,20,200,24,16,(uint8_t*) invite);
			POINT_COLOR = BLACK;
			HAL_Delay(1000);
			LCD_Fill(50, 10, 200, 40, WHITE);
		}
	}

	check();

    tp_dev.scan(0);
    if (screen_state == INITIAL) {
      if(tp_dev.sta&TP_PRES_DOWN) {
//        HAL_UART_Transmit(&huart1, (uint8_t*)"pressed\r\n", 9 , 0xFFFF);
        // between (10, 150) and (10 + 60, 150 + 60) is the area of the [CHAT] button
        if (tp_dev.x[0] > 10 && tp_dev.x[0] < 70 && tp_dev.y[0] > 150 && tp_dev.y[0] < 210) {
//          HAL_UART_Transmit(&huart1, (uint8_t*)"CHAT\r\n", 6 , 0xFFFF);
          screen_state = CHAT;
          draw_chat_screen();
        }
        // between (90, 150) and (90 + 60, 150 + 60) is the area of the [CALCULATOR] button
        else if (tp_dev.x[0] > 90 && tp_dev.x[0] < 150 && tp_dev.y[0] > 150 && tp_dev.y[0] < 210) {
//          HAL_UART_Transmit(&huart1, (uint8_t*)"CALCULATOR\r\n", 12 , 0xFFFF);
          screen_state = CALC_D;
          draw_calc_screen();
        }
        // between (170, 150) and (170 + 60, 150 + 60) is the area of the [PICTURE] button
        else if (tp_dev.x[0] > 170 && tp_dev.x[0] < 230 && tp_dev.y[0] > 150 && tp_dev.y[0] < 210) {
//          HAL_UART_Transmit(&huart1, (uint8_t*)"PICTURE\r\n", 9 , 0xFFFF);
          screen_state = PIC;
          album();
        }
        // between (10, 220) and (10 + 60, 220 + 60) is the area of the [Tetris] button
        else if (tp_dev.x[0] > 10 && tp_dev.x[0] < 70 && tp_dev.y[0] > 220 && tp_dev.y[0] < 280) {
//          HAL_UART_Transmit(&huart1, (uint8_t*)"Tetris\r\n", 8 , 0xFFFF);
          screen_state = Tetris;
          tetris();
        }
      }
    } else if (screen_state == CALC_B || screen_state == CALC_D || screen_state == CALC_E) {
      NRF24L01_RX_Mode();
      check2();
      calc_touch_screen_handler();
    }else if(screen_state == CHAT){
    	chat_touch_handler();
    }
    
  } 
}

extern int message_cnt;


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
  LED_Init();							//��ʼ��LED
  KEY_Init();							//��ʼ����????
  mem_init();							//��ʼ���ڴ��
  exfuns_init();						//Ϊfatfs��ر��������ڴ�
	f_mount(fs[0],"0:",1); 				//����SD????
	f_mount(fs[1],"1:",1); 				//����FLASH.
  Remote_Init();
	NRF24L01_Init();    		    	//��ʼ��NRF24L01
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
