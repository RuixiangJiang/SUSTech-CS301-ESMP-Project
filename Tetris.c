#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROW 14 // rows of map
#define COL 10 // columns of map

#define LEFT 1 // keyboard input 1
#define RIGHT 2 // keyboard input 2

struct Face{
	int data[ROW + 5][COL + 5]; // if map[i][j] has a square, 1 = yes / 0 = no
	int color[ROW + 5][COL + 5]; // what color should map[i][j] be
	int id[ROW + 5][COL + 5];
	// color = 0 means no square
	// color = 1 means boundary square
	// color = 2..8 means 7 kinds of squares
} map; // map[4..ROW][1..COL] are valid

struct basicBlock{
	int space[4][4]; // contains the shape of 7 basic kinds of blocks
} block[10][4];
// use a 4*4 square to define a block
// use the coordinate of top left corner to position a block

int gameScore = 0;

void initMap(){
	memset(map.data, 0, sizeof(map.data));
	memset(map.color, 0, sizeof(map.color));
	memset(map.id, 0, sizeof(map.id));
	for (int i = 0; i <= ROW; i++){ // boundary
		map.data[i][0] = map.data[i][COL + 1] = 1;
		map.color[i][0] = map.color[i][COL + 1] = 1;
		map.id[i][0] = map.id[i][COL + 1] = 0;
	}
	for (int j = 0; j <= COL; j++){ // boundary
		map.data[ROW + 1][j] = 1;
		map.color[ROW + 1][j] = 1;
		map.id[ROW + 1][j] = 0;
	}
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

void initGame(){
	gameScore = 0;
	initMap();
	initBlock();
	srand((unsigned int)time(NULL));
}

// after rotation the (shape) block has (form) and top-left position (x, y)
void drawBlock(int shape, int form, int id, int x, int y){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (block[shape][form].space[i][j]){
				map.data[x + i][y + j] = 1;
				map.color[x + i][y + j] = shape;
				map.id[x + i][y + j] = id;
			}
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

void printMap(){
	for (int i = 1; i <= ROW; i++){
		for (int j = 1; j <= COL; j++) printf("%d ", map.id[i][j]);
		printf("\n");
	}
	printf("----------------------------------\n");
}

void printBlock(int shape, int form){
	printf("Block %d of shape %d is:\n", shape, form);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++) printf("%d ", block[shape][form].space[i][j]);
		printf("\n");
	}
	printf("end block\n");
}

int isLegal(int shape, int form, int id, int x, int y){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (block[shape][form].space[i][j] == 1){
				if (map.data[x + i][y + j] == 1 && map.id[x + i][y + j] != id){
					printf("illegal: map(%d + %d, %d + %d).id = %d, this block has id %d\n",
						x,i,y,j,map.id[x+i][y+j],id);
					return 0;
				}
				if (x + i > ROW) return 0;
				if (y + j < 1) return 0;
				if (y + j > COL) return 0;
		}
	return 1;
}

// bottom-up clean rows from row = ROW to row = 4
int clearRow(){
	printf("New clearRow\n");
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
	printMap();
	return flag;
}

// judge if finish, return 1 means game over
int judgeFinish(){
	while (clearRow());
	for (int j = 1; j <= COL; j++){
		if (map.data[1][j] == 1){
			printf("Game Over.\n");
			return 1;
		}
	}
	return 0;
}

void tetris();

void startGame(){
	int shape = rand() % 7, form = rand() % 4, id = 1;
	while (1){
		int nxtShape = rand() % 7 + 2, nxtForm = rand() % 4;
		int x = 1, y = COL / 2 - 2; // initial position of block
		printBlock(shape, form);
		while (1){
			drawBlock(shape, form, id, x, y);

			// here you should use TIMER to control the falling speed
			// i.e., how long it should take to fall one row
			
			printMap();
			char ch = getchar(); // input instruction from keyboard
			if (ch == ' '){ // no instruction from keyboard
				if (!isLegal(shape, form, id, x + 1, y)){ // reaches bottom
					printf("No instruction, move to bottom\n");
					for (int i = 0; i < 4; i++)
						for (int j = 0; j < 4; j++)
							if (block[shape][form].space[i][j]){
								map.data[x + i][y + j] = 1;
								map.color[x + i][y + j] = shape;
								map.id[x + i][y + j] = id;
							}
					judgeFinish();
					break;
				}
				else{ // keeps falling
					drawSpace(shape, form, id, x++, y);
				}
			}
			else if (ch == 'a'){ // moves left
				printf("Move left: %d\n", isLegal(shape, form, id, x, y - 1));
				if (isLegal(shape, form, id, x, y - 1)){
					drawSpace(shape, form, id, x, y--);
				}
			}
			else if (ch == 'd'){ // moves right
				printf("Move right: %d\n", isLegal(shape, form, id, x, y + 1));
				if (isLegal(shape, form, id, x, y + 1)){
					drawSpace(shape, form, id, x, y++);
				}
			}
			else if (ch == 'p'){ // pause or go on
			}
			else if (ch == 'R'){ // restart
				tetris();
			}
		}
		shape = nxtShape, form = nxtForm, id++;
	}
}

void tetris(){
	initGame();
	for (int s = 2; s <= 8; s++)
		for (int f = 0; f < 4; f++)
		printBlock(s,f);
	startGame();
}

int main(){
	tetris();
}