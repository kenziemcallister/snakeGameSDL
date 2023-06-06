#include "game.h"

const int SCREEN_WIDTH = 1024; //setting the default screen dimensions
const int SCREEN_HEIGHT = 768;

SDL_Color bg_color = { 128, 128, 128, 255 }; //bg color

void moveSnake(Game* game) {

	for (int i = game->snakeLength - 1; i >= 1; i--) { //moving all segments of the snake to the previous position of the one before it
		game->snake[i].seg_row = game->snake[i - 1].seg_row;
		game->snake[i].seg_col = game->snake[i - 1].seg_col;
	}

	switch (game->snake_dir) { //moving the snakes head logic
	case Up:
		game->snake[0].seg_row--;
		break;

	case Down:
		game->snake[0].seg_row++;
		break;

	case Left:
		game->snake[0].seg_col--;
		break;

	case Right:
		game->snake[0].seg_col++;
		break;
	}

	for (int i = 1; i < game->snakeLength; i++) { //if the snakes head touches any other part of the snake then quit game
		if (game->snake[0].seg_row == game->snake[i].seg_row && game->snake[0].seg_col == game->snake[i].seg_col) {
			SDL_Quit();
		}
	}
	
}

void growSnake(Game* game) {

	game->snakeLength++; 
	printf("Score is %d\n", game->snakeLength - 1); //setting the score

	game->snake[game->snakeLength - 1].seg_row = game->snake[game->snakeLength - 2].seg_row; //creating a new segment of the snake
	game->snake[game->snakeLength - 1].seg_col = game->snake[game->snakeLength - 2].seg_col;

}

//what to do if the user presses up/down arrow keys etc
void handleEvent(SDL_Window* window, SDL_Event sdlEvent, Game* game) {

	switch (sdlEvent.type) { //sdl event stuff
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.sym) {
		case SDLK_UP:
			game->snake_dir = Up;
			break;
		case SDLK_DOWN:
			game->snake_dir = Down;
			break;
		case SDLK_LEFT:
			game->snake_dir = Left;
			break;
		case SDLK_RIGHT:
			game->snake_dir = Right;
			break;
		}
	}

}

// updateGame is called repeatedly from the game loop. delta is the amount of time in milliseconds that has elapsed since
// the last time updateGame was invoked. Use this function to change game state based on time that has elapsed.
void updateGame(int delta, Game* game) {

	game->tick += delta; 

	if (game->tick > 200) {
		moveSnake(game);
		game->tick = 0;
	}

	if (game->snake[0].seg_row == game->apple_row && game->snake[0].seg_col == game->apple_column) { //if snake eats apple then apple goes to new random position
		game->apple_row = rand() % 18;
		game->apple_column = rand() % 18; //need to update the score and add on to the snake
		growSnake(game);
	}
		
}

void drawGrid(SDL_Window* window, SDL_Renderer* renderer, Game* game) {

	int cell_size = 36;
	int grid_w = 18;
	int grid_h = 18;

	SDL_Rect border;
	border.x = 150;
	border.y = 50;
	border.w = cell_size * grid_w;
	border.h = cell_size * grid_h;

	SDL_Color grid_line_color = { 44, 44, 44, 255 };
	SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);

	for (int x = 0; x < grid_w; x++) {
		SDL_RenderDrawLine(renderer, (x * cell_size) + border.x, border.y, (x * cell_size) + border.x, (cell_size * grid_h) + border.y);
	}

	for (int y = 0; y < grid_h; y++) {
		SDL_RenderDrawLine(renderer, border.x, (y * cell_size) + border.y, (cell_size * grid_w) + border.x, (y * cell_size) + border.y);
	}

	SDL_Color border_color = { 255, 255, 255, 255 };
	SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
	SDL_RenderDrawRect(renderer, &border);

}

int colToX(int col) {

	int x = (col * 36) + 150;
	return x;

}

int rowtoY(int row) {

	int y = (row * 36) + 50;
	return y;

}

void drawApple(SDL_Window* window, SDL_Renderer* renderer, Game* game) {

	SDL_Rect apple;
	apple.x = colToX(game->apple_column);
	apple.y = rowtoY(game->apple_row);
	apple.w = 36;
	apple.h = 36;

	SDL_Color appleColor = { 255, 0, 0, 255 };
	SDL_SetRenderDrawColor(renderer, appleColor.r, appleColor.g, appleColor.b, appleColor.a);
	SDL_RenderFillRect(renderer, &apple);

}

void drawSnake(SDL_Window* window, SDL_Renderer* renderer, Game* game) { //drawing snake in its current pos
	
	SDL_Rect snake;

	for (int i = 0; i < game->snakeLength; i++) {
		snake.x = colToX(game->snake[i].seg_col);
		snake.y = rowtoY(game->snake[i].seg_row);
		snake.w = 36;
		snake.h = 36;

		SDL_Color snakeColor = { 0, 255, 0, 150 };
		SDL_SetRenderDrawColor(renderer, snakeColor.r, snakeColor.g, snakeColor.b, snakeColor.a);
		SDL_RenderFillRect(renderer, &snake);
	}

	if (game->snake[0].seg_col < 0 || game->snake[0].seg_row < 0 || game->snake[0].seg_col > 17 || game->snake[0].seg_row > 17) { //if snake goes outside of border quit game
		SDL_Quit();
	}

	//put in that if the head of the snake touches any other part of the snake then quit the game
	

}

// drawFrame is called repeatedly from the game loop. Use this function to draw the current game state.
void drawFrame(SDL_Window* window, SDL_Renderer* renderer, Game* game) {

	//first draw the grid
	drawGrid(window, renderer, game);
	drawApple(window, renderer, game);
	drawSnake(window, renderer, game);

}

int main() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) //inits the sdl 
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {

		SDL_Window* window = NULL; //setting a ptr to the window
		window = SDL_CreateWindow("Kenzie's Snake Game!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //creating window
		
		if (window == NULL) {
			printf("Window could not be created! SDL Error: %s", SDL_GetError()); //if window couldnt be created then display error
		}
		else {
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); //rendering the window
			Uint32 lastTicks = 0; //amount of time elapsed since we initialized sdl

			Game * game = newGame();

			//start the game loop!
			bool quit = false; //while the game is not quit
			while (!quit) {

				//dont want to slow down computer so every time we are in the loop lets delay just for a millisecond
				SDL_Delay(100);

				SDL_Event sdlEvent; //keyboard/mouse events,, struct that gives us the info about the event ex which key it was
				while (SDL_PollEvent(&sdlEvent)) {
					handleEvent(window, sdlEvent, game);
					if (sdlEvent.type == SDL_QUIT) //if user quits game, quit is true
						quit = true;
				}
				Uint32 ticks = SDL_GetTicks(); //the number of milliseconds since the game started/last updated game
				if (ticks > lastTicks) {
					updateGame(ticks-lastTicks, game); //ticks-lastticks gives us the # of millisecs since the last update
				}
				lastTicks = ticks;

				SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a); //set colors of the bg
				SDL_RenderClear(renderer);

				drawFrame(window, renderer, game);

				//update the window, applies the things done to the screen
				SDL_RenderPresent(renderer);

			}
			freeGame(game);

			SDL_DestroyRenderer(renderer);

		}

		SDL_DestroyWindow(window);

	}
	SDL_Quit();

	return 0;

}