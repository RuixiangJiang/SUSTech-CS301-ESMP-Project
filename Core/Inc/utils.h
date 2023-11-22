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

void draw_initial_screen(uint8_t* username);
void update_time();
#endif
