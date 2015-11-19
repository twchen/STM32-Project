#include "stdbool.h"
#include "lcd.h"
#include "key.h"
#include "delay.h"
#include "touch.h"
#include "my_list.h"

#define HEIGHT 16
#define WIDTH 32
#define NUM_OF_PIXELS (HEIGHT * WIDTH)

void _ttywrch(int ch)
{
ch = ch;
}


void erasePixel(int x, int y)
{
	LCD_Fill(x * 20 + 20, y * 20 + 20, x*20 + 39, y*20 + 39, WHITE);
}

void fillPixel(int x, int y)
{
	LCD_Fill(x * 20+20, y * 20+20, x*20 + 39, y*20+39, BLACK);
}

void drawFood(int x, int y)
{
	LCD_Fill(x * 20+20, y * 20+20, x*20 + 39, y*20+39, RED);
}

typedef enum Direction
{
	UP = 0,
	RIGHT,
	DOWN,
	LEFT
} Direction;

bool space[NUM_OF_PIXELS]; // 16 * 32

List *snake;
Direction dir;

Point food;
bool game_over;

List *make_snake()
{
	List *list = make_list();
	int i=0;
	for(; i<5; ++i){
		push_front(list, make_point(10, 5 + i));
	}
	return list;
}

void make_food()
{
	while(true){
		int x = rand() % HEIGHT;
		int y = rand() % WIDTH;
		if(space[x * WIDTH + y] == false){
			food = make_point(x, y);
			space[x * WIDTH + y] = true;
			break;
		}
	}
	drawFood(food.x, food.y);
}

/* move one step in the current direction */
void move_forward()
{
	Point p = list_front(snake);
	int x = p.x, y = p.y;
	switch(dir){
		case UP:
			--x;
			break;
		case DOWN:
			++x;
			break;
		case RIGHT:
			++y;
			break;
		case LEFT:
			--y;
			break;
	}

	// out of border?
	if(x >= HEIGHT || y >= WIDTH || x < 0 || y < 0){
		game_over = true;
		return;
	}

	if(space[x * WIDTH + y] == false){ // nothing there
		// simply move forward by one step
		Point tail = list_back(snake);
		space[tail.x * WIDTH + tail.y] = false;
		erasePixel(tail.x, tail.y);
		pop_back(snake);
		push_front(snake, make_point(x, y));
		fillPixel(x, y);
		space[x * WIDTH + y] = true;
	}
	else if(food.x == x && food.y == y){ // food there
		// eat the food and generate new food
		push_front(snake, food);
		fillPixel(x, y);
		space[x * WIDTH + y] = true;
		make_food();
	}
	else{ // collide with itself
		Point tail = list_back(snake);
		if(tail.x == x && tail.y == y){ // catching its tail is not considered as a collision
			pop_back(snake);
			push_front(snake, tail);
		}
		else
			game_over = true;
	}
}

/* initialize space */
void init_space()
{
	int i=0;
	LCD_Clear(WHITE);
	LCD_Fill(0, 0, 20, 680, GREEN);
	LCD_Fill(340, 0, 360, 680, GREEN);
	LCD_Fill(20, 0, 340, 20, GREEN);
	LCD_Fill(20, 660, 340, 680, GREEN);
	for(; i<NUM_OF_PIXELS; ++i)
		space[i] = false;
	/* init snake position */
	i=0;
	for(; i<5; ++i)
		space[10*WIDTH + 5 + i] = true;
}

void print_game_over()
{
	LCD_Clear(WHITE);
	LCD_ShowString(30, 200, 200, 24, 24, "GAME OVER!");
}

#define KEY_UP 3
#define KEY_DOWN 1
#define KEY_RIGHT 2
#define KEY_LEFT 4

void snake_game(void)
{
	//srand ( time(NULL) );
	srand(0);
	init_space();

	snake = make_snake();
	dir = RIGHT;
	make_food();
	game_over = false;

	while(true){
		u8 ch = 0;
		int counter = 500000;
		while(ch == 0 && counter > 0){
			ch = KEY_Scan(0);
			counter--;
		}
		counter = 500000;
		if(ch == KEY_UP && dir != DOWN)
			dir = UP;
		else if(ch == KEY_RIGHT && dir != LEFT)
			dir = RIGHT;
		else if(ch == KEY_DOWN && dir != UP)
			dir = DOWN;
		else if(ch == KEY_LEFT && dir != RIGHT)
			dir = LEFT;
		
		tp_dev.scan(0);
		if((tp_dev.sta) & 0x80 && tp_dev.y[4] > 600){
			clear_list(snake);
			break;
		}
			
		move_forward();
		if(game_over){
			print_game_over();
			clear_list(snake);
			delay_ms(1000);
			break;
		}
	}
}

