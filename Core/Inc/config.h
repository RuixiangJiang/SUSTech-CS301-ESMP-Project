#ifndef CONFIG_H
#define CONFIG_H

#include "stdint.h"

struct TIME_SETTING {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

extern unsigned char user_name[20];
extern struct TIME_SETTING time_setting;
extern uint16_t mark_seed;
#endif