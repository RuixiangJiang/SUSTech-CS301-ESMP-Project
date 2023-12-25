#include "utils.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "rtc.h"
#include "usart.h"
#include "calc.h"
#include "tetris.h"
#include "pic.h"
#include "24l01.h"

extern enum SCREEN_STATE screen_state;

uint16_t calc_equation_left, calc_equation_right;

const char *getDayOfWeekString(uint8_t dow)
{
  const char *days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  return days[dow];
}


void draw_initial_screen(uint8_t* username) {

    LCD_Clear(WHITE);
    POINT_COLOR = BLACK;
    update_time();
    
    char buffer[1024];

    sprintf(buffer, "user: [%s]", username);

    LCD_ShowString((240 - strlen(buffer) * 8) / 2, 110, 200, 16, 16, (uint8_t*) buffer);

    LCD_ShowPicture(10, 150, 60, 57, (uint16_t *) gImage_chat);

    LCD_ShowPicture(90, 150, 60, 60, (uint16_t *) gImage_calc);

    LCD_ShowPicture(170, 150, 60, 59, (uint16_t *) gImage_pic);

	LCD_ShowPicture(10, 220, 60, 60, (uint16_t *) gImage_tetris);

}

const uint32_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//月份日期表
struct CALENDAR { 
    uint16_t w_year;
    uint16_t  w_month;
    uint16_t  w_date;
    uint16_t  week;		 
    uint16_t  hour;
    uint16_t  min;
    uint16_t  sec;
} calendar;						//公历日历结构体

uint8_t Is_Leap_Year(uint16_t year)//平年,闰年判断
{			  
    if(year%4==0) //必须能被4整除
    { 
        if(year%100==0) 
        { 
            if(year%400==0)return 1;//如果以00结尾,还要能被400整除	   
            else return 0;   
        }else return 1;   
    }else return 0;	
}

int calculate_week( int year , int month, int day )
{
	int c,y,week;
	if (month == 1 || month == 2)
	year--, month += 12;
	c = year / 100;
	y = year - c * 100;
	week = y + y / 4 + c / 4 - 2 * c + 26 * (month + 1) / 10 + day - 1;
	while (week < 0)
	week += 7;
	week %= 7;
	return week;
}

uint8_t RTC_Get()
{
	static uint16_t  daycnt=0;
	uint32_t  timecount=0; 
	uint32_t  temp=0;
	uint16_t  temp1=0;
 	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL; 

 	temp=timecount/86400;   //得到天数(秒钟数对应的)


	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else break;  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	calendar.week=calculate_week(calendar.w_year,calendar.w_month,calendar.w_date);
    temp=timecount%86400;     		//得到秒钟数
    calendar.hour=temp/3600;     	//小时
    calendar.min=(temp%3600)/60; 	//分钟
    calendar.sec=(temp%3600)%60; 	//秒钟
    


	return 0;
}	 



uint8_t RTC_Set(uint16_t  syear,uint8_t  smon,uint8_t  sday,uint8_t  hour,uint8_t  min, uint8_t  sec)
{
	uint16_t  t;
	uint32_t  seccount=0;
	
	if(syear<1970||syear>2099)return 1;	     //实例是以1970年作为基准
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(uint32_t)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(uint32_t)hour*3600;//小时秒钟数
    seccount+=(uint32_t)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

    RCC->APB1ENR|=1<<28;//使能电源时钟
    RCC->APB1ENR|=1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护
	//上面三步是必须的!
	RTC->CRL|=1<<4;   //允许配置 
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成 

	RTC_Get();//设置完之后更新一下数据 	
    
	return 0;	    
}



void update_time() {
	// the screen is 320 (height) * 240 (width)
	RTC_Get();
	if (screen_state == INITIAL) {
		POINT_COLOR = BLACK;
		char now_time[20], now_data[20];
		sprintf(now_time, "%02d:%02d:%02d", calendar.hour, calendar.min, calendar.sec);
		sprintf(now_data, "%04d/%02d/%02d %s", calendar.w_year, calendar.w_month, calendar.w_date, getDayOfWeekString(calendar.week));
		LCD_ShowString((240 - strlen(now_time) * 12) / 2, 50, 200, 24, 24, (uint8_t*) now_time);
		LCD_ShowString((240 - strlen(now_data) * 8) / 2, 85, 200, 16, 16, (uint8_t*) now_data);
	}
}

//struct User{
//	int status; // 1 is online, 0 is offline
//	unsigned char name[30];
//};

struct User friend1 = {0,"Jack"};

//int send(const char* msg){
//	NRF24L01_TX_Mode();
//	sprintf(DATA_TO_SEND,msg);
//	int cnt = 0;
////	while(1)
////	{
//		if(NRF24L01_TxPacket(DATA_TO_SEND)==TX_OK)
//		{
//			if (strncmp((char*)DATA_TO_SEND, "check", strlen("check")) != 0){
//				char message[300];
//				sprintf(message, "%s: %s",user_name,msg);
//				LCD_ShowString(10,50+20*message_cnt,lcddev.width-1,16,16,message);
//				message_cnt++;
//			}
//			NRF24L01_RX_Mode();
//			return 1;
//		}
//		delay_ms(20);
////	}
//	NRF24L01_RX_Mode();
//	return 0;
//}



extern unsigned char user_name[20];
int message_cnt = 0;
int interface = 0;
u8 tmp_buf[33];
int mode24 = 0;
extern int chat_remind;
#define MAX_MSG_COUNT 50
#define MAX_MSG_LENGTH 100
extern unsigned char DATA_TO_SEND[200];

char messages[MAX_MSG_COUNT][MAX_MSG_LENGTH];
int msg_count = 0;
int count = 0;


int send(const char* msg){
	NRF24L01_TX_Mode();
	sprintf(DATA_TO_SEND,msg);
	int cnt = 0;

	int max_chars_per_line = (lcddev.width - 10) / 8; // 每行能容纳的字符数量
    int max_lines_per_page = 10; // 每页能容纳的行数
    int total_lines = message_cnt; // 总共的聊天行数
    int total_pages = (total_lines - 1) / max_lines_per_page + 1; // 总页数
    int current_page = 1; // 当前显示的页数
//	while(1){
		if(NRF24L01_TxPacket(DATA_TO_SEND)==TX_OK)
		{
			if (strncmp((char*)DATA_TO_SEND, "check", strlen("check")) != 0){
				if (strncmp((char*)DATA_TO_SEND, "emoji1", strlen("emoji1")) == 0){
					char message[30];
					sprintf(message, "%s:", user_name);
					LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, message);
					LCD_ShowPicture(60, 50 + 20 * message_cnt, 30, 30, (uint16_t *) gImage_emoji_cool);
					message_cnt+=2;

					char message_save[30];
					sprintf(message_save, "%s: %s",user_name,"emoji1");
					strncpy(messages[msg_count], message_save, MAX_MSG_LENGTH - 1);

					messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
					msg_count+=2;

				}else if(strncmp((char*)DATA_TO_SEND, "emoji2", strlen("emoji2")) == 0){
					char message[30];
					sprintf(message, "%s:", user_name);
					LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, message);
					LCD_ShowPicture(60, 50 + 20 * message_cnt, 30, 29, (uint16_t *) gImage_emoji_smile);
					message_cnt+=2;

					char message_save[30];
					sprintf(message_save, "%s: %s",user_name,"emoji2");
					strncpy(messages[msg_count], message_save, MAX_MSG_LENGTH - 1);

					messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
					msg_count+=2;

				}else{
					char message[300];
					sprintf(message, "%s: %s", user_name, msg);

					int msg_length = strlen(message);

					if (msg_length > max_chars_per_line){
						int start_index = 0; // 要显示的消息的起始索引
						while (start_index < msg_length) {
							char tmp_msg[400] = {0};
							strncpy(tmp_msg, message + start_index, max_chars_per_line);
							tmp_msg[max_chars_per_line] = '\0'; // 添加字符串结束符

							if(message_cnt <= 10){
								LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, tmp_msg);
							}else{
								message_cnt = 0;
							}

							strncpy(messages[msg_count], tmp_msg, MAX_MSG_LENGTH - 1);
							messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
							msg_count++;

							message_cnt++;
							start_index += max_chars_per_line;
						}
					}else{
						LCD_ShowString(10,50+20*message_cnt,lcddev.width-1,16,16,message);

						strncpy(messages[msg_count], message, MAX_MSG_LENGTH - 1);
						messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
						msg_count++;
						message_cnt++;
					}
				}
			}
			NRF24L01_RX_Mode();
			return 1;

		}delay_ms(20);
//	}
		if (strncmp((char*)DATA_TO_SEND, "check", strlen("check")) != 0){
			//发送消息失败
			friend1.status = 0;
			POINT_COLOR = RED;
			if(screen_state == CHAT){
				LCD_ShowString(120,65,200,16,16,(uint8_t*)"offline");
			}
			chat_remind = 0;

		}



	NRF24L01_RX_Mode();
	return 0;
}




void check2(){
	u8 check_buf[33];
	if(NRF24L01_RxPacket(check_buf)==0){
		char check_buf_substr1[7]; // 5字符 + 1 null 终止符
		memcpy(check_buf_substr1, check_buf, 6);
		check_buf_substr1[6] = '\0'; // 添加 null 终止符

		char check_buf_substr2[6]; // 5字符 + 1 null 终止符
		memcpy(check_buf_substr2, check_buf, 5);
		check_buf_substr2[5] = '\0'; // 添加 null 终止符

		//收到invite
		if (strcmp(check_buf_substr1, "INVITE") == 0) {
			LCD_Fill(50, 10, 100, 40, WHITE);
			char invite[300];
			POINT_COLOR = RED;
			sprintf(invite, "%s %s",friend1.name,"invite you!");
			LCD_ShowString(60,20,200,24,16,(uint8_t*) invite);
			HAL_Delay(1000);
			LCD_Fill(50, 10, 200, 40, WHITE);
			chat_remind = 1;
		}
	}
	POINT_COLOR = BLACK;
	NRF24L01_RX_Mode();
}


void check(){
	if(send("check")==1){
		friend1.status = 1;
		POINT_COLOR = RED;
		if(screen_state == CHAT){
			if(interface == 0){
			LCD_ShowString(120,65,200,16,16,(uint8_t*)"online");
			}
		}else{
			if(chat_remind == 0){
				LCD_Fill(50, 10, 100, 40, WHITE);
				char remind[300];
				sprintf(remind, "%s %s",friend1.name,"is online!");
				LCD_ShowString(60,20,200,24,16,(uint8_t*) remind);
				HAL_Delay(1000);
				LCD_Fill(50, 10, 200, 40, WHITE);
				chat_remind = 1;
			}
		}
	}else{
		friend1.status = 0;
		POINT_COLOR = RED;
		if(screen_state == CHAT){
			LCD_ShowString(120,65,200,16,16,(uint8_t*)"offline");
		}
		chat_remind = 0;
	}
	POINT_COLOR = BLACK;
	NRF24L01_RX_Mode();
	check2();
}

void draw_chat_screen() {
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowString(70,10,200,24,24,(uint8_t*)"Contacts");
	LCD_ShowPicture(0,40,219,80,(uint16_t *)gImage_contact);
	LCD_ShowString(90,65,200,16,16,(uint8_t *)friend1.name);
}


//void chat_touch_handler(){
//	if(interface==0){
//		check();
//		if (tp_dev.sta&TP_PRES_DOWN) {
//			HAL_Delay(50);
//			if (tp_dev.sta&TP_PRES_DOWN) {
//				uint16_t x = tp_dev.x[0], y = tp_dev.y[0];
//				//0,40,219,80
//				if(x>20&&x<380&&y>40&&y<120&&friend1.status==1)
//				{
//					LCD_Clear(WHITE);
//					POINT_COLOR = BLACK;
//					interface = 1;
//					LCD_ShowString(70,10,200,24,24,(uint8_t*)"Chat");
//					send("INVITE");
////					char message[300];
////					sprintf(message, "%s: %s",user_name,test_message);
////					LCD_ShowString(10,50,200,16,16,message);
//				}
//			}
//		} else {
//			HAL_Delay(50);
//		}
//	}else if(interface==1)
//	{
//		POINT_COLOR = BLACK;
//		NRF24L01_RX_Mode();
//		while(1){
//			if(NRF24L01_RxPacket(tmp_buf)==0){
//				tmp_buf[32]=0;//加入字符串结束符
//				char tmp_buf_substr[6]; // 5字符 + 1 null 终止符
//				memcpy(tmp_buf_substr, tmp_buf, 5);
//				tmp_buf_substr[5] = '\0'; // 添加 null 终止符
//				if (strcmp(tmp_buf_substr, "check") != 0) {
//					char message[300];
//					sprintf(message, "%s: %s",friend1.name,tmp_buf);
//					LCD_ShowString(10,50+20*message_cnt,lcddev.width-1,16,16,message);
//					message_cnt++;
//				}
//			}else delay_us(100);
//		}
//	}
//}


void chat_touch_handler(){
	if(interface==0){
//		check();
		if (tp_dev.sta&TP_PRES_DOWN) {
			HAL_Delay(50);
			if (tp_dev.sta&TP_PRES_DOWN) {
				uint16_t x = tp_dev.x[0], y = tp_dev.y[0];
				//0,40,219,80
				if(x>20&&x<380&&y>40&&y<120 && friend1.status ==1){

					LCD_Clear(WHITE);
					interface = 1;
					send("INVITE");
					LCD_ShowString(70,10,200,24,24,(uint8_t*)"Chat");
					LCD_ShowPicture(50, 280, 30, 30, (uint16_t *) gImage_emoji_cool);
					LCD_ShowPicture(130, 280, 30, 29, (uint16_t *) gImage_emoji_smile);
				}
			}
		} else {
			HAL_Delay(50);
		}
	}else if(interface==1){
		NRF24L01_RX_Mode();
		//返回按钮
		POINT_COLOR = BLUE;
		LCD_ShowString(140,10,200,24,24,(uint8_t*)"RETURN");
		POINT_COLOR = BLACK;

		//显示第一页的内容
        for (int i = 0; i < 10; i++) {
			char tmp_buf_substr[7]; // 5字符 + 1 null 终止符
			memcpy(tmp_buf_substr, messages[i], 6);
			tmp_buf_substr[6] = '\0'; // 添加 null 终止符

//        	if (strcmp(tmp_buf_substr, "emoji1") == 0){
        	if (strstr(messages[i], "emoji1") != NULL){
//				char message[30];
//				sprintf(message, "%s:",friend1.name);

				char message[6]; // 5字符 + 1 null 终止符
				strncpy(message, messages[i], 5);
				message[5] = '\0'; // 添加 null 终止符

				LCD_ShowString(10, 50 + 20 * i, lcddev.width - 1, 16, 16, message);
				LCD_ShowPicture(60, 50 + 20 * i, 30, 30, (uint16_t *) gImage_emoji_cool);
				i++;
        	}else if (strstr(messages[i], "emoji2") != NULL){
//				char message[30];
//				sprintf(message, "%s:",friend1.name);
				char message[6]; // 5字符 + 1 null 终止符
				strncpy(message, messages[i], 5);
				message[5] = '\0'; // 添加 null 终止符
				LCD_ShowString(10, 50 + 20 * i, lcddev.width - 1, 16, 16, message);
				LCD_ShowPicture(60, 50 + 20 * i, 30, 29, (uint16_t *) gImage_emoji_smile);
				i++;
        	}else{
        		LCD_ShowString(10, 50 + 20 * i, lcddev.width - 1, 16, 16, (uint8_t*)messages[i]);
        	}
        }

        message_cnt = msg_count;
		while(1){
		    tp_dev.scan(0);
			if (tp_dev.sta&TP_PRES_DOWN) {
				uint16_t x = tp_dev.x[0], y = tp_dev.y[0];
	            uint16_t x_start, y_start, x_end, y_end;
	            uint8_t threshold = 20; // 滑动阈值，根据实际情况进行调整
	            x_start = x;
	            y_start = y;
	            //返回按钮->回到interface = 0
	            if(x > 130 && x < lcddev.width && y > 0 && y < 40){
	            	interface = 0;
	            	LCD_Clear(WHITE);
	            	POINT_COLOR = BLACK;
	            	LCD_ShowString(70,10,200,24,24,(uint8_t*)"Contacts");
	            	LCD_ShowPicture(0,40,219,80,(uint16_t *)gImage_contact);
	            	LCD_ShowString(90,60,200,16,16,(uint8_t *)friend1.name);
	            	break;
	            }

	            //点击emoji
//				LCD_ShowPicture(50, 280, 30, 30, (uint16_t *) gImage_emoji_cool);
//				LCD_ShowPicture(130, 280, 30, 29, (uint16_t *) gImage_emoji_smile);
	            if(x > 50 && x < 90 && y > 280 && y < 320 && (x_start == x_end)){ //emoji1  cool
					send("emoji1");
	            }
	            if(x > 130 && x < 170 && y > 280 && y < 320 && (x_start == x_end)){ //smile
	            	send("emoji2");
	            }

	            HAL_Delay(50); // 等待一段时间，防止滑动过快导致的误触
	            tp_dev.scan(0);
	            if (tp_dev.sta & TP_PRES_DOWN) {
	                x_end = tp_dev.x[0];
	                y_end = tp_dev.y[0];
	                // 判断滑动方向和距离
	                if ((x_start > x_end) && (x_start - x_end > threshold)) { //翻页
	                    // 执行翻页操作
	                    LCD_Clear(WHITE);
						LCD_ShowString(70,10,200,24,24,(uint8_t*)"Chat");
						LCD_ShowPicture(50, 280, 30, 30, (uint16_t *) gImage_emoji_cool);
						LCD_ShowPicture(130, 280, 30, 29, (uint16_t *) gImage_emoji_smile);
						interface = 2;
						break;
	                }
	            }
			}

			if(NRF24L01_RxPacket(tmp_buf)==0){ //接收到信息

				tmp_buf[32]=0;//加入字符串结束符

				char tmp_buf_substr[6]; // 5字符 + 1 null 终止符
				memcpy(tmp_buf_substr, tmp_buf, 5);
				tmp_buf_substr[5] = '\0'; // 添加 null 终止符

				char tmp_buf_substr2[7]; // 5字符 + 1 null 终止符
				memcpy(tmp_buf_substr2, tmp_buf, 6);
				tmp_buf_substr2[6] = '\0'; // 添加 null 终止符

				if (strcmp(tmp_buf_substr, "check") != 0) {

					if(strcmp(tmp_buf_substr2, "emoji1") == 0){
						char message[30];
						sprintf(message, "%s:",friend1.name);
						LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, message);
						LCD_ShowPicture(60, 50 + 20 * message_cnt, 30, 30, (uint16_t *) gImage_emoji_cool);
						message_cnt+=2;

						char message_save[30];
						sprintf(message_save, "%s: %s",friend1.name,"emoji1");
						strncpy(messages[msg_count], message_save, MAX_MSG_LENGTH - 1);
						messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
						msg_count+=2;

					}else if(strcmp(tmp_buf_substr2, "emoji2") == 0){
						char message[30];
						sprintf(message, "%s:",friend1.name);
						LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, message);
						LCD_ShowPicture(60, 50 + 20 * message_cnt, 30, 29, (uint16_t *) gImage_emoji_smile);
						message_cnt+=2;

						char message_save[30];
						sprintf(message_save, "%s: %s",friend1.name,"emoji2");
						strncpy(messages[msg_count], message_save, MAX_MSG_LENGTH - 1);
						messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
						msg_count+=2;

					}else{
						char message[300];
						sprintf(message, "%s: %s",friend1.name,tmp_buf);

						int msg_length = strlen(message);
						int max_chars_per_line = (lcddev.width - 10) / 8; // 每行能容纳的字符数量
						int max_lines_per_page = 10; // 每页能容纳的行数
						int current_page = 1; // 当前显示的页数

						if (msg_length > max_chars_per_line){ //需要换行
							int start_index = 0; // 要显示的消息的起始索引
							while (start_index < msg_length) {
								char tmp_msg[400] = {0};
								strncpy(tmp_msg, message + start_index, max_chars_per_line);
								tmp_msg[max_chars_per_line] = '\0'; // 添加字符串结束符

								if(message_cnt <= 10){
									LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, tmp_msg);
								}
//								else{
//									message_cnt = 0;
//								}

								strncpy(messages[msg_count], tmp_msg, MAX_MSG_LENGTH - 1);
								messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
								msg_count++;

								message_cnt++;
								start_index += max_chars_per_line;
								count++;
							}
						}else{ //不需要换行
							LCD_ShowString(10,50+20*message_cnt,lcddev.width-1,16,16,message);

							strncpy(messages[msg_count], message, MAX_MSG_LENGTH - 1);
							messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
							msg_count++;

							message_cnt++;
							count++;
						}
					}

				}
			}else delay_us(100);
		}
	}else if (interface = 2){//新的一页
		//返回按钮
		POINT_COLOR = BLUE;
		LCD_ShowString(140,10,200,24,24,(uint8_t*)"RETURN");
		POINT_COLOR = BLACK;

//		//显示之前没显示出来的消息
//        for (int i = 10; i < msg_count; i++) {
//            LCD_ShowString(10, 50 + 20 * (i-10), lcddev.width - 1, 16, 16, (uint8_t*)messages[i]);
//        }

		//显示之前没显示出来的消息
        for (int i = 10; i < msg_count; i++) {
        	int j = i-10;
			char tmp_buf_substr[7]; // 5字符 + 1 null 终止符
			memcpy(tmp_buf_substr, messages[i], 6);
			tmp_buf_substr[6] = '\0'; // 添加 null 终止符

//        	if (strcmp(tmp_buf_substr, "emoji1") == 0){
        	if (strstr(messages[i], "emoji1") != NULL){
//				char message[30];
//				sprintf(message, "%s:",friend1.name);

				char message[6]; // 5字符 + 1 null 终止符
				strncpy(message, messages[i], 5);
				message[5] = '\0'; // 添加 null 终止符

				LCD_ShowString(10, 50 + 20 * j, lcddev.width - 1, 16, 16, message);
				LCD_ShowPicture(60, 50 + 20 * j, 30, 30, (uint16_t *) gImage_emoji_cool);
				i++;
        	}else if (strstr(messages[i], "emoji2") != NULL){
//				char message[30];
//				sprintf(message, "%s:",friend1.name);
				char message[6]; // 5字符 + 1 null 终止符
				strncpy(message, messages[i], 5);
				message[5] = '\0'; // 添加 null 终止符
				LCD_ShowString(10, 50 + 20 * j, lcddev.width - 1, 16, 16, message);
				LCD_ShowPicture(60, 50 + 20 * j, 30, 29, (uint16_t *) gImage_emoji_smile);
				i++;
        	}else{
        		LCD_ShowString(10, 50 + 20 * j, lcddev.width - 1, 16, 16, (uint8_t*)messages[i]);
        		//LCD_ShowString(10, 50 + 20 * i, lcddev.width - 1, 16, 16, (uint8_t*)messages[i]);
        	}
        }

        message_cnt = msg_count-10;
        //接收
		NRF24L01_RX_Mode();
		while(1){
		    tp_dev.scan(0);
			if (tp_dev.sta&TP_PRES_DOWN) {
				uint16_t x = tp_dev.x[0], y = tp_dev.y[0];
	            uint16_t x_start, y_start, x_end, y_end;
	            uint8_t threshold = 20; // 滑动阈值，根据实际情况进行调整
	            x_start = x;
	            y_start = y;

	            //返回按钮->回到interface = 0
	            if(x > 130 && x < lcddev.width && y > 0 && y < 40){
	            	interface = 0;
	            	LCD_Clear(WHITE);
	            	POINT_COLOR = BLACK;
	            	LCD_ShowString(70,10,200,24,24,(uint8_t*)"Contacts");
	            	LCD_ShowPicture(0,40,219,80,(uint16_t *)gImage_contact);
	            	LCD_ShowString(90,60,200,16,16,(uint8_t *)friend1.name);
	            	break;
	            }

	            //点击emoji
//				LCD_ShowPicture(50, 280, 30, 30, (uint16_t *) gImage_emoji_cool);
//				LCD_ShowPicture(130, 280, 30, 29, (uint16_t *) gImage_emoji_smile);
	            if(x > 50 && x < 90 && y > 280 && y < 320 && (x_start == x_end)){ //emoji1  cool
					send("emoji1");
	            }
	            if(x > 130 && x < 170 && y > 280 && y < 320 && (x_start == x_end)){ //smile
	            	send("emoji2");
	            }

	            HAL_Delay(50); // 等待一段时间，防止滑动过快导致的误触
	            tp_dev.scan(0);
	            if (tp_dev.sta & TP_PRES_DOWN) {
	                x_end = tp_dev.x[0];
	                y_end = tp_dev.y[0];
	                // 判断滑动方向和距离
	                //右滑
	                if ((x_start < x_end) && (x_end - x_start > threshold)) {
	                    // 执行翻页操作
	                	interface = 1;
	            		LCD_Clear(WHITE);
	            		LCD_ShowString(70,10,200,24,24,(uint8_t*)"Chat");
	            		LCD_ShowPicture(50, 280, 30, 30, (uint16_t *) gImage_emoji_cool);
	            		LCD_ShowPicture(130, 280, 30, 29, (uint16_t *) gImage_emoji_smile);
	                	break;
	                }
	            }
			}

			if(NRF24L01_RxPacket(tmp_buf)==0){ //接收到信息
				tmp_buf[32]=0;//加入字符串结束符

				char tmp_buf_substr[6]; // 5字符 + 1 null 终止符
				memcpy(tmp_buf_substr, tmp_buf, 5);
				tmp_buf_substr[5] = '\0'; // 添加 null 终止符

				char tmp_buf_substr2[7]; // 5字符 + 1 null 终止符
				memcpy(tmp_buf_substr2, tmp_buf, 6);
				tmp_buf_substr2[6] = '\0'; // 添加 null 终止符

				if (strcmp(tmp_buf_substr, "check") != 0) {

					if(strcmp(tmp_buf_substr2, "emoji1") == 0){
						char message[30];
						sprintf(message, "%s:",friend1.name);
						LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, message);
						LCD_ShowPicture(60, 50 + 20 * message_cnt, 30, 30, (uint16_t *) gImage_emoji_cool);
						message_cnt+=2;

						char message_save[30];
						sprintf(message_save, "%s: %s",friend1.name,"emoji1");
						strncpy(messages[msg_count], message_save, MAX_MSG_LENGTH - 1);

						messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
						msg_count+=2;

					}else if(strcmp(tmp_buf_substr2, "emoji2") == 0){
						char message[30];
						sprintf(message, "%s:",friend1.name);
						LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, message);
						LCD_ShowPicture(60, 50 + 20 * message_cnt, 30, 29, (uint16_t *) gImage_emoji_smile);
						message_cnt+=2;

						char message_save[30];
						sprintf(message_save, "%s: %s",friend1.name,"emoji1");
						strncpy(messages[msg_count], message_save, MAX_MSG_LENGTH - 1);

						messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
						msg_count+=2;

					}else{
						char message[300];
						sprintf(message, "%s: %s",friend1.name,tmp_buf);

						int msg_length = strlen(message);
						int max_chars_per_line = (lcddev.width - 10) / 8; // 每行能容纳的字符数量
						int max_lines_per_page = 10; // 每页能容纳的行数
						int current_page = 1; // 当前显示的页数

						if (msg_length > max_chars_per_line){ //需要换行
							int start_index = 0; // 要显示的消息的起始索引
							while (start_index < msg_length) {
								char tmp_msg[400] = {0};
								strncpy(tmp_msg, message + start_index, max_chars_per_line);
								tmp_msg[max_chars_per_line] = '\0'; // 添加字符串结束符

								if(message_cnt <= 10){
									LCD_ShowString(10, 50 + 20 * message_cnt, lcddev.width - 1, 16, 16, tmp_msg);
								}
//								else{
//									message_cnt = 0;
//								}
								strncpy(messages[msg_count], tmp_msg, MAX_MSG_LENGTH - 1);
								messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
								msg_count++;

								message_cnt++;
								start_index += max_chars_per_line;
								count++;
							}
						}else{ //不需要换行
							LCD_ShowString(10,50+20*message_cnt,lcddev.width-1,16,16,message);

							strncpy(messages[msg_count], message, MAX_MSG_LENGTH - 1);
							messages[msg_count][MAX_MSG_LENGTH - 1] = '\0'; // 确保字符串以'\0'结尾
							msg_count++;

							message_cnt++;
							count++;
						}
					}
				}
			}else delay_us(100);
		}
	}
}



uint16_t left_up_x[5][5], left_up_y[5][5], right_down_x[5][5], right_down_y[5][5];
uint16_t move_left_up_x[2], move_left_up_y[2], move_right_down_x[2], move_right_down_y[2];
uint8_t equation[1024];
uint16_t equation_len;
uint8_t has_pressed;

void draw_calc_screen() {
	LCD_Clear(WHITE);
	LCD_ShowPicture(0, 120, 240, 26, (uint16_t *) gImage_left_or_right);
//	LCD_ShowPicture(0, 150, 240, 165, (uint16_t *) gImage_calc_bg_DE);
	LCD_ShowPicture_black(0, 150, 240, 165, (uint8_t *) gImage_calc_bg_DE);
	if (screen_state == CALC_D) {
		char notice[50] = "[mode] decimal computation";
		LCD_ShowString((240 - strlen(notice) * 8) / 2, 90, 400, 16, 16, (uint8_t*) notice);
	} else if (screen_state == CALC_E) {
		char notice[50] = "[mode] equation solving";
		LCD_ShowString((240 - strlen(notice) * 8) / 2, 90, 400, 16, 16, (uint8_t*) notice);
	} else if (screen_state == CALC_B) {
		char notice[50] = "[mode] binary computation";
		LCD_ShowString((240 - strlen(notice) * 8) / 2, 90, 400, 16, 16, (uint8_t*) notice);
	}

	uint16_t x, y = 150;
	for (int i = 0; i < 5; i++) {
		x = 0;
		for (int j = 0; j < 5; j++) {
			left_up_x[i][j] = x;
			left_up_y[i][j] = y;
			right_down_x[i][j] = x + 40;
			right_down_y[i][j] = y + 25;
			x += 50;
		}
		y += 35;
	}

	move_left_up_x[0] = 0;
	move_left_up_y[0] = 120;
	move_right_down_x[0] = 90;
	move_right_down_y[0] = 120 + 25;

	move_left_up_x[1] = 150;
	move_left_up_y[1] = 120;
	move_right_down_x[1] = 240;
	move_right_down_y[1] = 120 + 25;

	uint16_t i = 0;
	while (equation[i] != 0 && i < 1024) {
		equation[i++] = 0;
	}
	equation_len = 0;
	has_pressed = 0;
	calc_equation_left = 0;
	calc_equation_right = 28;
}

void draw_pic_screen() {
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowString(10, 10, 200, 16, 16, (uint8_t*) "pic");
}

uint8_t press_between(uint16_t x, uint16_t y, uint16_t lp_x, uint16_t lp_y, uint16_t rd_x, uint16_t rd_y) {
	if (x >= lp_x && x <= rd_x && y >= lp_y && y <= rd_y) {
		return 1;
	} else {
		return 0;
	}
}
uint8_t press_calc_button(uint16_t x, uint16_t y, uint16_t i, uint16_t j) {
	if (press_between(x, y, left_up_x[i][j], left_up_y[i][j], right_down_x[i][j], right_down_y[i][j])) {
		return 1;
	} else {
		return 0;
	}	
}

void clear_calc_equation() {
	LCD_ShowString(10, 10, 240, 16, 16, (uint8_t*) "                                                  ");
}
void clear_calc_result() {
	LCD_ShowString(10, 40, 240, 16, 16, (uint8_t*) "                                                  ");
}

void display_calc_equation() {
	char display_part[30];
	uint16_t i = calc_equation_left;
	uint16_t j = 0;
	while (i < calc_equation_right && equation[i] != 0) {
		display_part[j++] = equation[i++];
	}
	display_part[j] = 0;
	LCD_ShowString(10, 10, 240, 16, 16, (uint8_t*) display_part);
}


void floatToString(float num, char* buffer, int decimalPlaces) {

	float eps = 0.1;
	for (int i = 0; i < decimalPlaces; i++) {
		eps /= 10;
	}

	uint8_t negative = 0;
	if (num < 0) {
		num = -num;
		negative = 1;
	}

    int integerPart = (int)num;
    float fractionalPart = num - integerPart;

    // 处理整数部分
	if (negative) {
		sprintf(buffer, "-%d", integerPart);
	} else {
		sprintf(buffer, "%d", integerPart);
	}

	if (fractionalPart == 0) {
		return;
	}

    // 添加小数点
    strcat(buffer, ".");

    // 处理小数部分
    for (int i = 0; i < decimalPlaces; i++) {
        if (fractionalPart - 0 < eps) {
			break;
		}
		fractionalPart *= 10;
        int digit = (int)fractionalPart;
        sprintf(buffer + strlen(buffer), "%d", digit);
        fractionalPart -= digit;
    }
}

void work_for_calc(uint16_t i, uint16_t j) {
	if (i < 0 || i > 4 || j < 0 || j > 4) {
		return;
	}
	uint8_t is_append = 1;
	if (i == 0 && j == 0) {
		if (screen_state == CALC_D) {
			screen_state = CALC_E;
		} else if (screen_state == CALC_E) {
			screen_state = CALC_B;
		} else if (screen_state == CALC_B) {
			screen_state = CALC_D;
		}
		draw_calc_screen();
		is_append = 0;
	} else if (i == 0 && j == 1) {
		while (equation_len > 0) {
			equation[--equation_len] = 0;
		}
		clear_calc_equation();
		clear_calc_result();
		calc_equation_left = 0;
		calc_equation_right = 28;
		is_append = 0;
	} else if (i == 0 && j == 2) {
		if (equation_len > 0) {
			equation[--equation_len] = 0;
		}
		clear_calc_equation();
		calc_equation_right = equation_len;
		if (calc_equation_right < 28) {
			calc_equation_right = 28;
		}
		calc_equation_left = calc_equation_right - 28;
		is_append = 0;
	} else if (i == 0 && j == 3) {
		equation[equation_len++] = '^';
	} else if (i == 0 && j == 4) {
		equation[equation_len++] = '/';
	} else if (i == 1 && j == 0) {
		equation[equation_len++] = '(';
	} else if (i == 1 && j == 1) {
		equation[equation_len++] = '7';
	} else if (i == 1 && j == 2) {
		equation[equation_len++] = '8';
	} else if (i == 1 && j == 3) {
		equation[equation_len++] = '9';
	} else if (i == 1 && j == 4) {
		equation[equation_len++] = '*';
	} else if (i == 2 && j == 0) {
		equation[equation_len++] = ')';
	} else if (i == 2 && j == 1) {
		equation[equation_len++] = '4';
	} else if (i == 2 && j == 2) {
		equation[equation_len++] = '5';
	} else if (i == 2 && j == 3) {
		equation[equation_len++] = '6';
	} else if (i == 2 && j == 4) {
		equation[equation_len++] = '-';
	} else if (i == 3 && j == 0) {
		equation[equation_len++] = 'z';
	} else if (i == 3 && j == 1) {
		equation[equation_len++] = '1';
	} else if (i == 3 && j == 2) {
		equation[equation_len++] = '2';
	} else if (i == 3 && j == 3) {
		equation[equation_len++] = '3';
	} else if (i == 3 && j == 4) {
		equation[equation_len++] = '+';
	} else if (i == 4 && j == 0) {
		equation[equation_len++] = 'y';
	} else if (i == 4 && j == 1) {
		equation[equation_len++] = 'x';
	} else if (i == 4 && j == 2) {
		equation[equation_len++] = '0';
	} else if (i == 4 && j == 3) {
		equation[equation_len++] = '.';
	} else if (i == 4 && j == 4) {
		if (screen_state == CALC_D) {
			Pair result = CalcExpression((char *) equation);
			char result_str[100];
			uint8_t result_len = 0;
			while (result_str[result_len] != 0) {
				result_str[result_len++] = 0;
			}
			if (result.errorCode != 0) {
				sprintf(result_str, "ERROR [%d]", result.errorCode);
			} else {
				floatToString(result.result, result_str, 5);
			}
			clear_calc_result();
			LCD_ShowString(10, 40, 240, 16, 16, (uint8_t *) result_str);
			is_append = 0;
		} else if (screen_state == CALC_E) {
			uint16_t cnt_equal = 0;
			for (int i = 0; i < equation_len; i++) {
				if (equation[i] == '=') {
					cnt_equal++;
				}
			}
			if (cnt_equal == 0) {
				equation[equation_len++] = '=';
			} else {
				char unknown_var = 0;
				for (int i = 0; i < equation_len; i++) {
					if (equation[i] == 'x' || equation[i] == 'y' || equation[i] == 'z') {
						unknown_var = equation[i];
						break;
					}
				}
				Pair2 result = SolveEquation((char *) equation);
				char result_str[100];
				uint8_t result_len = 0;
				while (result_str[result_len] != 0) {
					result_str[result_len++] = 0;
				}
				if (result.errorCode != 0) {
					sprintf(result_str, "ERROR [%d]", result.errorCode);
				} else {
					Root answer = result.root;
					if (answer.rootNum == 0) {
						sprintf(result_str, "NO ROOT");
					} else if (answer.rootNum == 1) {
						char root1_str[100];
						floatToString(answer.root1, root1_str, 5);
						sprintf(result_str, "%c = %s", unknown_var, root1_str);
					} else if (answer.rootNum == 2) {
						char root1_str[100];
						if (answer.root1 > answer.root2) {
							float tmp = answer.root1;
							answer.root1 = answer.root2;
							answer.root2 = tmp;
						}
						floatToString(answer.root1, root1_str, 5);
						char root2_str[100];
						floatToString(answer.root2, root2_str, 5);
						sprintf(result_str, "%c = %s | %c = %s", unknown_var, root1_str, unknown_var, root2_str);
					}
				}
				clear_calc_result();
				LCD_ShowString(10, 40, 240, 16, 16, (uint8_t *) result_str);
				is_append = 0;
			}
		} else if (screen_state == CALC_B) {
			Pair result = CalcBinaryExpression((char *) equation);
			char result_str[100];
			uint8_t result_len = 0;
			while (result_str[result_len] != 0) {
				result_str[result_len++] = 0;
			}
			if (result.errorCode != 0) {
				sprintf(result_str, "ERROR [%d]", result.errorCode);
			} else {
				int integerPart = (int)result.result;
				sprintf(result_str, "%d", integerPart);
			}
			clear_calc_result();
			LCD_ShowString(10, 40, 240, 16, 16, (uint8_t *) result_str);
			is_append = 0;
		}
	}
	if (is_append && equation_len > 28) {
		calc_equation_right = equation_len;
		calc_equation_left = calc_equation_right - 28;
		clear_calc_equation();
	}
	display_calc_equation();
}

void calc_button_shift_handler(uint8_t press_key_id) {
	// press_key_id == 1: left (key1 按下，方程左移)
	// press_key_id == 0: right （key0 按下，方程右移）
	if (press_key_id == 1) {
		// 左移
		if (calc_equation_left > 0) {
			calc_equation_left--;
			calc_equation_right--;
			clear_calc_equation();
			display_calc_equation();
		}

	} else if (press_key_id == 0) {
		// 右移
		if (calc_equation_right < equation_len) {
			calc_equation_left++;
			calc_equation_right++;
			clear_calc_equation();
			display_calc_equation();
		}
	}
}


void calc_touch_screen_handler() {

	if (tp_dev.sta&TP_PRES_DOWN) {
		HAL_Delay(50);
		if (tp_dev.sta&TP_PRES_DOWN) {
			uint16_t x = tp_dev.x[0], y = tp_dev.y[0];

			if (press_between(x, y, move_left_up_x[0], move_left_up_y[0], move_right_down_x[0], move_right_down_y[0])) {
				calc_button_shift_handler(1);
			} else if (press_between(x, y, move_left_up_x[1], move_left_up_y[1], move_right_down_x[1], move_right_down_y[1])) {
				calc_button_shift_handler(0);
			} else if (!has_pressed) {
				uint16_t press_i = 5, press_j = 5;
				uint8_t find_press_button = 0;
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						if (press_calc_button(x, y, i, j)) {
							press_i = i;
							press_j = j;
							find_press_button = 1;
							break;
						}
					}
					if (find_press_button) {
						break;
					}
				}
				if (find_press_button) {
					work_for_calc(press_i, press_j);
				}
			}
			has_pressed = 1;
		}

	} else {
		HAL_Delay(50);
		if (!(tp_dev.sta&TP_PRES_DOWN)) {
			has_pressed = 0;
		}
	}
}
