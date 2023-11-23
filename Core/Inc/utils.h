#ifndef __UTILS_H
#define __UTILS_H
#include "stdint.h"
#include "main.h"
#include "touch.h"
#include "calc.h"

enum SCREEN_STATE {
  INITIAL,
  CHAT,
  CALC_D,
  CALC_E,
  CALC_B,
  PIC
};

uint8_t RTC_Set(uint16_t  syear,uint8_t  smon,uint8_t  sday,uint8_t  hour,uint8_t  min, uint8_t  sec);
void draw_initial_screen(uint8_t* username);
void update_time();
void draw_chat_screen();
void draw_calc_screen();
void draw_pic_screen();
void calc_touch_screen_handler();
#endif
