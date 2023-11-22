#ifndef __UTILS_H
#define __UTILS_H
#include "stdint.h"
#include "main.h"

enum SCREEN_STATE {
  INITIAL,
  CHAT,
  CALC,
  PIC
};

uint8_t RTC_Set(uint16_t  syear,uint8_t  smon,uint8_t  sday,uint8_t  hour,uint8_t  min, uint8_t  sec);
void draw_initial_screen(uint8_t* username);
void update_time();
#endif
