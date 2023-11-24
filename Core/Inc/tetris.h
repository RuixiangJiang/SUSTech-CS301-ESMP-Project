#ifndef __TETRIS_H
#define __TETRIS_H 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../Inc/utils.h"
#include "../Inc/lcd.h"
#include "../Inc/rtc.h"
#include "../Inc/usart.h"

#define ROW 24 // rows of map
#define COL 15 // columns of map

typedef struct{
	int data[ROW + 5][COL + 5]; // if map[i][j] has a square, 1 = yes / 0 = no
	int color[ROW + 5][COL + 5]; // what color should map[i][j] be
	int id[ROW + 5][COL + 5];
	// color = 0 means no square
	// color = 1 means boundary square
	// color = 2..8 means 7 kinds of squares
} Face;
typedef struct{
	int space[4][4]; // contains the shape of 7 basic kinds of blocks
} basicBlock;

static int gameScore = 0;
static int gamePaused = 0;

void initGame();
void startGame();

void tetris();

Face getMap();
int getScore();

void draw_tetris_screen();

#endif