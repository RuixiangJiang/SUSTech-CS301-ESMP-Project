#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "../Inc/tetris.h"
#include "../Inc/utils.h"
#include "../Inc/lcd.h"
#include "../Inc/rtc.h"
#include "../Inc/usart.h"

Face map;
basicBlock block[10][4];

unsigned char infoUART[1024] = {0};
int infoLen;

int hasPaused = 0;

int COL_N = 100;

int srandNum = 1;

extern enum SCREEN_STATE screen_state;
extern unsigned char user_name[20];
extern struct User friend1;
extern int chat_remind;

void initMap(){
	memset(map.data, 0, sizeof(map.data));
	memset(map.color, 0, sizeof(map.color));
	memset(map.id, 0, sizeof(map.id));
	for (int i = 0; i <= ROW; i++){
		map.data[i][0] = map.data[i][COL + 1] = 1;
		map.color[i][0] = map.color[i][COL + 1] = 1;
		map.id[i][0] = map.id[i][COL + 1] = 0;
	}
	for (int j = 0; j <= COL; j++){
		map.data[ROW + 1][j] = 1;
		map.color[ROW + 1][j] = 1;
		map.id[ROW + 1][j] = 0;
	}
    map.data[ROW + 1][COL + 1] = 1;
    map.color[ROW + 1][COL + 1] = 1;
    map.id[ROW + 1][COL + 1] = 0;
}

void initBlock(){
	// T - shape
	for (int i = 0; i <= 2; i++) block[2][0].space[1][i] = 1;
	block[2][0].space[2][1] = 1;

	// L - shape
	for (int i = 1; i <= 3; i++) block[3][0].space[i][1] = 1;
	block[3][0].space[3][2] = 1;

	// J - shape
	for (int i = 1; i <= 3; i++) block[4][0].space[i][2] = 1;
	block[4][0].space[3][1] = 1;

	for (int i = 0; i <= 1; i++){
		// Z - shape
		block[5][0].space[1][i] = 1;
		block[5][0].space[2][i + 1] = 1;
		// S - shape
		block[6][0].space[1][i + 1] = 1;
		block[6][0].space[2][i] = 1;
		// O - shape
		block[7][0].space[1][i + 1] = 1;
		block[7][0].space[2][i + 1] = 1;
	}

	// I - shape
	for (int i = 0; i <= 3; i++) block[8][0].space[i][1] = 1;

	int temp[4][4];
	for (int shape = 2; shape < 9; shape++){
		for (int form = 0; form < 3; form++){
			// form = 0, 1, 2, 3 means rotating 0, 90, 180, 270 degrees clockwise
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					temp[i][j] = block[shape][form].space[i][j];
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					block[shape][form + 1].space[i][j] = temp[3 - j][i];
		}
	}
}

unsigned int generate_seed(){
    int a[srandNum];
    srandNum = (srandNum + 1) % 300 + 1;
    return &a;
}
void initGame(){
	gameScore = 0;
	initMap();
	initBlock();
	srand(generate_seed());
}

// after rotation the (shape) block has (form) and top-left position (x, y)
int drawBlock(int shape, int form, int id, int x, int y){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (block[shape][form].space[i][j]){
                if (map.data[x + i][y + j] && map.id[x + i][y + i] != 0 && map.id[x + i][y + i] != id) return 1;
				map.data[x + i][y + j] = 1;
				map.color[x + i][y + j] = shape;
				map.id[x + i][y + j] = id;
			}
    return 0;
}

// use space to cover the block before rotation
void drawSpace(int shape, int form, int id, int x, int y){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (block[shape][form].space[i][j]){
				map.data[x + i][y + j] = 0;
				map.color[x + i][y + j] = 0;
				map.id[x + i][y + j] = 0;
			}
}

int isLegal(int shape, int form, int id, int x, int y){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (block[shape][form].space[i][j] == 1){
				if (map.data[x + i][y + j] == 1 && map.id[x + i][y + j] != id) return 0;
				if (x + i > ROW) return 0;
				if (y + j < 1) return 0;
				if (y + j > COL) return 0;
		}
	return 1;
}

// bottom-up clean rows from row = ROW to row = 4
int clearRow(){
	int flag = 0;
	for (int i = ROW; i >= 4; i--){
		int rowSum = 0;
		for (int j = 1; j <= COL; j++) rowSum += map.data[i][j];
		if (rowSum == COL){ // full row
			gameScore += 10;
			flag = 1;
			for (int j = 1; j <= COL; j++){
				map.data[i][j] = 0;
				map.color[i][j] = 0;
				map.id[i][j] = 0;
			}
			for (int j = i; j > 0; j--){
				for (int k = 1; k <= COL; k++){ // move row above to this row
					map.data[j][k] = map.data[j - 1][k];
					map.color[j][k] = map.color[j - 1][k];
					map.id[j][k] = map.id[j - 1][k];
				}
			}
		}
	}
	return flag;
}

int inRange(int x, int y, int x0, int x1, int y0, int y1){return x >= x0 && x <= x1 && y >= y0 && y <= y1;}
char getInstruction(int x, int y){
    if (inRange(x, y, 10, 80, 10, 40)) return 'R';
    if (inRange(x, y, 90, 130, 10, 40)) return 'p';
    if (inRange(x, y, 140, 180, 10, 40)) return 'c';
    if (inRange(x, y, 190, 230, 160, 190)) return 'a';
    if (inRange(x, y, 190, 230, 200, 230)) return 'd';
    if (inRange(x, y, 190, 230, 240, 270)) return 'q';
    if (inRange(x, y, 190, 230, 280, 310)) return 'e';
}

void printMap(){
    if (screen_state != Tetris) return;
    uint16_t COLOR[COL_N + 5];
    for (int i = 0; i <= ROW + 1; i++){
        int x = 10, y = 50 + i * 10;
        for (int j = 0; j <= COL + 1; j++){
            switch (map.color[i][j]){
            case 0:
                for (int k = 0; k < COL_N; k++) COLOR[k] = WHITE;
                break;
            case 1:
                for (int k = 0; k < COL_N; k++) COLOR[k] = GRAY;
                break;
            case 2:
                for (int k = 0; k < COL_N; k++) COLOR[k] = BLUE;
                break;
            case 3:
                for (int k = 0; k < COL_N; k++) COLOR[k] = RED;
                break;
            case 4:
                for (int k = 0; k < COL_N; k++) COLOR[k] = GREEN;
                break;
            case 5:
                for (int k = 0; k < COL_N; k++) COLOR[k] = YELLOW;
                break;
            case 6:
                for (int k = 0; k < COL_N; k++) COLOR[k] = BROWN;
                break;
            case 7:
                for (int k = 0; k < COL_N; k++) COLOR[k] = BRED;
                break;
            case 8:
                for (int k = 0; k < COL_N; k++) COLOR[k] = MAGENTA;
                break;
            default:
                for (int k = 0; k < COL_N; k++) COLOR[k] = WHITE;
                break;
            }
            LCD_Color_Fill(x + 2, y + 2, x + 8, y + 8, COLOR);
            infoLen = snprintf((char *)infoUART, sizeof(infoUART), "(%d, %d)\r\n", x, y);
            //HAL_UART_Transmit(&huart1, infoUART, infoLen, HAL_MAX_DELAY);
            x += 10;
        }
    }
}

void printNextBlock(int shape, int form){
    if (screen_state != Tetris) return;
    uint16_t COLOR[COL_N + 5];
    for (int i = 0; i < 4; i++){
        int x = 190, y = 110 + i * 10;
        for (int j = 0; j < 4; j++){
            if (!block[shape][form].space[i][j]){
                for (int k = 0; k < COL_N; k++) COLOR[k] = WHITE;
            }
            else switch (shape){
                case 0:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = WHITE;
                    break;
                case 2:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = BLUE;
                    break;
                case 3:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = RED;
                    break;
                case 4:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = GREEN;
                    break;
                case 5:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = YELLOW;
                    break;
                case 6:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = BROWN;
                    break;
                case 7:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = BRED;
                    break;
                case 8:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = MAGENTA;
                    break;
                default:
                    for (int k = 0; k < COL_N; k++) COLOR[k] = RED;
                    break;
            }
            LCD_Color_Fill(x + 2, y + 2, x + 8, y + 8, COLOR);
            x += 10;
        }
    }
}
void startGame(){
	int shape = rand() % 7 + 2, form = rand() % 4, id = 1;
    int hasPressed = 0;
    NRF24L01_RX_Mode();
	while (screen_state == Tetris){
		check2();
		check();
		u8 check_buf[33];
		if(NRF24L01_RxPacket(check_buf)==0){
			char check_buf_substr[7]; // 5字符 + 1 null 终止符
			memcpy(check_buf_substr, check_buf, 6);
			check_buf_substr[6] = '\0'; // 添加 null 终止符
			if (strcmp(check_buf_substr, "INVITE") == 0) {
				LCD_Fill(50, 10, 100, 40, WHITE);
				char invite[300];
				sprintf(invite, "%s %s",friend1.name,"invite you!");
				LCD_ShowString(60,20,200,24,16,(uint8_t*) invite);
				HAL_Delay(1000);
				LCD_Fill(50, 10, 200, 40, WHITE);
			}
		}
		int nxtShape = rand() % 7 + 2, nxtForm = rand() % 4;
		int x = 1, y = COL / 2 - 2; // initial position of block
		while (screen_state == Tetris){
            tp_dev.scan(0);
            if (drawBlock(shape, form, id, x, y)){ // finish
                LCD_ShowString(35, 155, 200, 80, 24, (uint8_t*) "Game Over!");
                return;
            }
            printMap();
            printNextBlock(nxtShape, nxtForm);

            if (hasPaused){
                if (tp_dev.sta & TP_PRES_DOWN){
                    HAL_Delay(10);
                    if (tp_dev.sta & TP_PRES_DOWN){
                        if (getInstruction(tp_dev.x[0], tp_dev.y[0]) == 'p'){
                            hasPaused = 0;
                        };
                    }
                }
                continue;
            }
			
			char ch = ' '; // input instruction from board
            int waitCount = 80;
            while (waitCount--){
            	check2();
            	check();
                if (tp_dev.sta & TP_PRES_DOWN){
                    HAL_Delay(10);
                    if (tp_dev.sta & TP_PRES_DOWN && !hasPressed){
                        ch = getInstruction(tp_dev.x[0], tp_dev.y[0]);
                        hasPressed = 1;
                        infoLen = snprintf((char *)infoUART, sizeof(infoUART), "(%d, %d) -> ", tp_dev.x[0], tp_dev.y[0]);
                        //HAL_UART_Transmit(&huart1, infoUART, infoLen, HAL_MAX_DELAY);
                        break;
                    }
                }
                else{
                    HAL_Delay(10);
                    if (!(tp_dev.sta & TP_PRES_DOWN)) hasPressed = 0;
                }
            }
            infoLen = snprintf((char *)infoUART, sizeof(infoUART), "input instruction is %c.\r\n", ch);
//			HAL_UART_Transmit(&huart1, infoUART, infoLen, HAL_MAX_DELAY);

			if (ch == ' '){ // no instruction from board
				if (!isLegal(shape, form, id, x + 1, y)){ // reaches bottom
					for (int i = 0; i < 4; i++)
						for (int j = 0; j < 4; j++)
							if (block[shape][form].space[i][j]){
								map.data[x + i][y + j] = 1;
								map.color[x + i][y + j] = shape;
								map.id[x + i][y + j] = id;
							}
					while (clearRow());
					break;
				}
				else{ // keeps falling
					drawSpace(shape, form, id, x++, y);
				}
			}
			else if (ch == 'a'){ // moves left
				if (isLegal(shape, form, id, x, y - 1)){
					drawSpace(shape, form, id, x, y--);
				}
			}
			else if (ch == 'd'){ // moves right
				if (isLegal(shape, form, id, x, y + 1)){
					drawSpace(shape, form, id, x, y++);
				}
			}
			else if (ch == 'q'){ // rotate counterclockwise
				if (isLegal(shape, (form + 1) % 4, id, x, y)){
					drawSpace(shape, form, id, x, y);
					form = (form + 1) % 4;
				}
			}
			else if (ch == 'e'){
				if (isLegal(shape, (form + 3) % 4, id, x, y)){
					drawSpace(shape, form, id, x, y);
					form = (form + 3) % 4;
				}
			}
			else if (ch == 'p'){ // pause
                hasPaused = 1;
			}
            else if (ch == 'c'){ // fall down to the bottom
                drawSpace(shape, form, id, x, y);
                while (isLegal(shape, form, id, x + 1, y)) x++;
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        if (block[shape][form].space[i][j]){
                            map.data[x + i][y + j] = 1;
                            map.color[x + i][y + j] = shape;
                            map.id[x + i][y + j] = id;
                        }
                while (clearRow());
                break;
            }
			else if (ch == 'R'){ // back to menu
				screen_state = INITIAL;
                draw_initial_screen((uint8_t *)user_name);
                return;
			}
		}
        if (screen_state != Tetris) return;
		shape = nxtShape, form = nxtForm, id++;
        gameScore++;
        POINT_COLOR = BLACK;
        char gameScoreStr[10] = {0};
        sprintf(gameScoreStr, "%d", gameScore);
        LCD_ShowString(190, 40, 40, 30, 24, (uint8_t*) gameScoreStr);
	}
}

void tetris(){
    initGame();
    draw_tetris_screen();
    int cooool[121];
	startGame();
}

Face getMap(){return map;}

int getScore(){return gameScore;}

void draw_tetris_screen() {
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowString(10, 10, 70, 30, 24, (uint8_t*) "BACK");
	LCD_ShowString(90, 10, 40, 30, 24, (uint8_t*) "P");
	LCD_ShowString(140, 10, 40, 30, 24, (uint8_t*) "C");
	LCD_ShowString(190, 10, 40, 30, 16, (uint8_t*) "Score");
    char gameScoreStr[10] = {0};
    sprintf(gameScoreStr, "%d", gameScore);
	LCD_ShowString(190, 40, 40, 30, 24, (uint8_t*) gameScoreStr);
	LCD_ShowString(190, 80, 40, 20, 16, (uint8_t*) "Next");
	LCD_ShowString(190, 160, 40, 30, 16, (uint8_t*) "Left");
	LCD_ShowString(190, 200, 40, 30, 16, (uint8_t*) "Right");
	LCD_ShowString(190, 240, 40, 30, 16, (uint8_t*) "TurnL");
	LCD_ShowString(190, 280, 40, 30, 16, (uint8_t*) "TurnR");

	POINT_COLOR = BLUE;
	LCD_DrawRectangle(10, 10, 80, 40);
	LCD_DrawRectangle(90, 10, 130, 40);
	LCD_DrawRectangle(140, 10, 180, 40);
	LCD_DrawRectangle(190, 160, 230, 190);
	LCD_DrawRectangle(190, 200, 230, 230);
	LCD_DrawRectangle(190, 240, 230, 270);
	LCD_DrawRectangle(190, 280, 230, 310);
}
