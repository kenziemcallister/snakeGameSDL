#pragma once //tell compiler to only compile once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#define ROW_COUNT 18
#define COL_COUNT 18

enum Direction { Up, Down, Left, Right };

typedef struct {
	int seg_row;
	int seg_col;
} snakeSeg;

typedef struct {
	int apple_row;
	int apple_column;
	enum Direction snake_dir;
	int tick;
	snakeSeg snake[ROW_COUNT * COL_COUNT];
	int snakeLength;
} Game;


Game* newGame();

void freeGame(Game * game);

