#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ff.h"
#include "../Inc/utils.h"
#include "../Inc/lcd.h"
#include "../Inc/rtc.h"
#include "../Inc/usart.h"
#include "../Inc/album.h"
#include "bsp_driver_sd.h"

FATFS fs;                 //工作空间
FIL fil;                  // 文件项目
uint32_t file_byte = 0;		  //文件大小（字节数）
uint32_t byteswritten = 0;    // 写文件计数
uint32_t bytesread = 0;       // 读文件计数
uint8_t wtext[] = "test write"; // 写的内容
uint8_t rtext[1024];             	// 读取的buff,1024bytes

char filename[] = "log.txt"; // 文件名
extern uint8_t retSD; /* Return value for SD */
unsigned char info[1024] = {0};

void TFcard_test(void)
{
	int infoLen;

    /*-1- 挂载文件系统*/
	retSD = f_mount(&fs, "", 0);
   // retSD = f_mount(&fs, "0:", 1);
    if(retSD)
    {
    	infoLen = snprintf((char *)info, sizeof(info), "mount error : %d \r\n", retSD);
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
        Error_Handler();
    }
    else{
    	infoLen = snprintf((char *)info, sizeof(info), "mount successfully\r\n");
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }

    /*-2-创建新的文件并写入数据*/
    retSD = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);		//打开文件，权限包括创建、写（如果没有该文件，会创建该文件）
    if(retSD){ //返回值不为0（出现问题）
    	infoLen = snprintf((char *)info, sizeof(info), "open file error : %d\r\n",retSD);
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }
    else{
    	infoLen = snprintf((char *)info, sizeof(info), "open file successfully\r\n");
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }

    /*-3- 在txt文件中写入数据*/
//	for(uint32_t i=0;i<7000;i++)
//	{
		retSD = f_write(&fil, wtext, sizeof(wtext), (void *)&byteswritten);	//在文件内写入wtext内的内容
		// printf(" this is  %d  bytes\r\n",i);
//	}
	    if(retSD){
	    	infoLen = snprintf((char *)info, sizeof(info), "write file error : %d\r\n",retSD);
	    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
	    }
    else
    {
    	infoLen = snprintf((char *)info, sizeof(info), "write file successfully\r\n write Data : %s\r\n",wtext);
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }


    /*-4- 关闭txt文件*/
    retSD = f_close(&fil);												//关闭该文件
    if(retSD){
    	infoLen = snprintf((char *)info, sizeof(info), "close error : %d\r\n",retSD);
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }
    else{
    	infoLen = snprintf((char *)info, sizeof(info), "close successfully!!! \r\n");
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }

    /*-5- 打开文件读取数据*/
    retSD = f_open(&fil, filename, FA_READ);							//打开文件，权限为只读
    if(retSD){
    	infoLen = snprintf((char *)info, sizeof(info), "open file error : %d\r\n",retSD);
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }
    else{
    	infoLen = snprintf((char *)info, sizeof(info), "open file successfully!!! \r\n");
    	HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }

    /*-6- 读取txt文件数据*/
    retSD = f_read(&fil, rtext, sizeof(rtext), (UINT*)&bytesread);		//读取文件内容放到rtext中
    if(retSD){
		infoLen = snprintf((char *)info, sizeof(info), "read file error : %d\r\n",retSD);
		HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
	}
	else{
		infoLen = snprintf((char *)info, sizeof(info), "read file successfully\r\n write Data : %s\r\n",wtext);
		HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
	}

    /*-7- 关闭文件*/
    retSD = f_close(&fil);												//关闭该文件
    if(retSD){
		infoLen = snprintf((char *)info, sizeof(info), "close error : %d\r\n",retSD);
		HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
	}
	else{
		infoLen = snprintf((char *)info, sizeof(info), "close successfully!!! \r\n");
		HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
	}

    /*##-8- 读写一致性检测 ############*/
    if(bytesread == byteswritten)										//如果读写位数一致
    {
		infoLen = snprintf((char *)info, sizeof(info), "FatFs is working well!!!\r\n");
		HAL_UART_Transmit(&huart1, info, infoLen, HAL_MAX_DELAY);
    }
}
