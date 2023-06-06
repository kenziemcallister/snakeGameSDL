#include "game.h"
#include <time.h>

Game * newGame() {

	srand(time(0));

	Game * game = malloc(sizeof(Game));
	if (game != NULL) {
		game->apple_row = rand() % 18;
		game->apple_column = rand() % 18;
		game->snake_dir = Up;
		game->snakeLength = 1;
		game->snake[0].seg_row = ROW_COUNT / 2;
		game->snake[0].seg_col = COL_COUNT / 2;
		game->tick = 0;
	}
	return game;
}

void freeGame(Game * game) {
	free(game);
	return;
}