#include "./ddcLib/ddcChars.h"
#include "./ddcLib/ddcPrint.h"

#define FGBLOCK CS_BLOCK
#define BGBLOCK CS_BLOCK
#define BGCOLOR "\x1b[38;2;0;0;0m"
#define FGCOLOR "\x1b[38;2;255;255;0m"

int WIDTH;
int HEIGHT;

char** grid;
char** ngrid;

int count_neighbors(int x, int y)
{
	int count = 0;
	count += grid[(y+1)%HEIGHT][(x-1+WIDTH)%WIDTH];
	count += grid[(y+1)%HEIGHT][x];
	count += grid[(y+1)%HEIGHT][(x+1)%WIDTH];
	count += grid[y][(x-1+WIDTH)%WIDTH];
	count += grid[y][(x+1)%WIDTH];
	count += grid[(y-1+HEIGHT)%HEIGHT][(x-1+WIDTH)%WIDTH];
	count += grid[(y-1+HEIGHT)%HEIGHT][x];
	count += grid[(y-1+HEIGHT)%HEIGHT][(x+1)%WIDTH];

	return count;
}

void draw_cell(int x, int y, int state)
{
	cursor_move_to(x*2, y);
	if (state)
		ddPrints(FGCOLOR FGBLOCK FGBLOCK);
	else ddPrints(BGCOLOR BGBLOCK BGBLOCK);

}

void copy_grid(void)
{
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			grid[y][x] = ngrid[y][x];
}

void compute(void)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int state = grid[y][x];

			int neighbors = count_neighbors(x, y);

			if (state == 0 && neighbors == 3)
			{
				ngrid[y][x] = 1;
				draw_cell(x, y, 1);
			}
			else if (state == 1 && (neighbors < 2 || neighbors > 3))
			{
				ngrid[y][x] = 0;
				draw_cell(x, y, 0);
			}
			else ngrid[y][x] = grid[y][x];
		}
	}
	copy_grid();
}

void draw(void)
{
	draw_rect(0, 0, (WIDTH*2)-1, HEIGHT-1, BGCOLOR BGBLOCK);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (grid[y][x])
			{
				cursor_move_to(x*2, y);
				ddPrints(FGCOLOR FGBLOCK FGBLOCK);
			}
		}
	}
}

int main(void)
{
	srand(time(0));

	WIDTH = cursor_get_width()/2;
	HEIGHT = cursor_get_height();

	grid = malloc(sizeof(char*)*HEIGHT);
	ngrid = malloc(sizeof(char*)*HEIGHT);

	for (int y = 0; y < HEIGHT; y++)
	{
		grid[y] = malloc(sizeof(char)*WIDTH);
		ngrid[y] = malloc(sizeof(char)*WIDTH);
	}

	cursor_clear();
	cursor_home();
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			grid[y][x] = rand()%2;

	draw();
	while (1)
	{
		usleep(100000);
		compute();
		//draw();
	}


	cursor_move_to(0, 35);

	return 0;
}
