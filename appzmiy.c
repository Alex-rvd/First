#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>



int hvost = 5;
int coord_hy;
int	coord_hx;
int score = 0;
bool p_placed = 0;
bool t_placed = 0;
bool location = 0;

		
		
int create (int rows, int cols, char (*map)[cols], int coords [6])
{
	
	int py, px;
	int hy, hx;
	int ty, tx;
	srand(time(NULL));
	
	// fill map with walls and borders
	for (int y = 0; y <= rows; y++)
	{
		for (int x = 0; x <= cols; x++)
		{
			//borders
			if (y == 0 || y == rows || y == rows-1 || x == 0 || x == cols)
			{
				map[y][x] = '%';	
			}
			// walls
			else
			{
				map[y][x] = ' ';
			}	
		}
	}
	
	
	// spawn goblin
	do
	{
	ty = rand() % rows;
	tx = rand() % cols;
	}
	while (map [ty][tx] != ' ');
	t_placed = 1;
	map [ty][tx] = 't';
	
	
	
	// spawn sneek
	//if (!p_placed)   // p_placed == 0
	py = 16;
	px = 8;
	hy = py;
	hx = px - (hvost-1);

	for (int x = hvost - 1; x >= 0; x--)
	{
		if (x == 0) 
		{
			map [py][px] = '@';
		}
		else
		{
			map [py][px-x] = '#';
		}
	}
	
		
	// draw location
	for (int y = 0; y <= rows-1; y++)
		for (int x = 0; x <= cols; x++)
			mvaddch(y, x, map[y][x]);
	
	coords [0] = py;	
	coords [1] = px;	
	coords [2] = hy;	
	coords [3] = hx;	
	coords [4] = ty;	
	coords [5] = tx;	

	return 0 ;
}

int dangeon(int c, int rows, int cols, char (*map)[cols], int coords [6], int leng [200])
{
	int py = coords[0];
	int px = coords[1];	
	int hy = coords[2];
	int hx = coords[3];
	int ty = coords[4];
	int tx = coords[5];
	
	
	int up = 1;
	int down = 2;
	int left = 3;
	int right = 4;
	int change_dir;
	int moved;
	int z_mov;
	int lpy, lpx;
	bool game_over;
	
	srand(time(NULL));
	
	
	// enter user action
	game_over = 0;
	lpy = py;
	lpx = px;
	if (c == KEY_UP)
	{
		change_dir = up;
		if (map[py-1][px] == ' ' || map[py-1][px] == 't')
		{
			//moved = right;
			map[py][px]='#';
			py--;
			map[py][px]='@';
			coords[0] = py;
		}
		else
		{
			game_over = 1;
		}
	}
	else if (c == KEY_DOWN)
	{
		change_dir = down;
		if (map[py+1][px] == ' ' || map[py+1][px] == 't')
		{
			//moved = right;
			map[py][px]='#';
			py++;
			map[py][px]='@';
			coords[0] = py;
		}
		else
		{
			game_over = 1;
		}
	}
	else if (c == KEY_LEFT)
	{
		change_dir = left;
		if (map[py][px-1] == ' ' || map[py][px-1] == 't')
		{
			//moved = left;
			map[py][px]='#';
			px--;
			map[py][px]='@';
			coords[1] = px;
		}
		else
		{
			game_over = 1;
		}
	}
	else if (c == KEY_RIGHT )
	{
		change_dir = right;
		if (map[py][px+1] == ' ' || map[py][px+1] == 't')
		{
			//moved = right;
			map[py][px]='#';
			px++;
			map[py][px]='@';
			coords[1] = px;
		}
		else
		{
			game_over = 1;
		}
	}
	
	//filling array zmeinyj hvost
	if (change_dir != leng[leng[0]*3])
	{
		leng[0] += 1;
		leng[leng[0]*3] = change_dir;
		leng[leng[0]*3+1] = lpy;
		leng[leng[0]*3+2] = lpx;
		
	}
	
	// kill goblin...
	if (py == ty && tx == px)
	{
		map [ty][tx] = '@';
		t_placed = 0;
		score += 5;
		hvost++;
	}
	else	// .... and going hvost
	{
		
		map[hy][hx]=' ';
		
		if (leng[1] == up)
		{
			hy--;
		}
		else if (leng[1] == down)
		{
			hy++;
		}
		else if (leng[1] == left)
		{
			hx--;
		}
		else if (leng[1] == right)
		{
			hx++;
		}
		
		if (hy == leng[4] && hx == leng[5])
		{
			leng[1] = leng[3];
			
			for (int mov = 3 ; mov <= (leng[0]*3)+2; mov++)
			{
				leng[mov] = leng[mov+3];
				
			}
			leng[0] -= 1;
		}
		
		
	/* 	if (map[hy-1][hx]=='#')
		{
			hy--;
		}
		else if (map[hy+1][hx]=='#')
		{
			hy++;
		}
		else if (map[hy][hx-1]=='#')
		{
			hx--;
		}
		else if (map[hy][hx+1]=='#')
		{
			hx++;
		} */
		
		coords [2] = hy;
		coords [3] = hx;
		
	}
	
	
	// spawn goblin
	if (!t_placed)   // t_placed == 0
	{
		do
		{
		ty = rand() % rows;
		tx = rand() % cols;
		}
		while (map [ty][tx] != ' ');
		t_placed = 1;
		map [ty][tx] = 't';
		coords [4] = ty;
		coords [5] = tx;

	}
	
	
		
	
		// draw location
	for (int y = 0; y <= rows-1; y++)
		for (int x = 0; x <= cols; x++)
			mvaddch(y, x, map[y][x]);
		

	
	
	
	if (game_over) return 1 ; //kill yourself
	
	// draw sneek
	
	return 0;
}


int main()
{
	int c = 0;
	int rows, cols;
	int coord[6];
	int leng [200] = {0,4,0,4,0};
	
	initscr();
	keypad(stdscr, 1); //allow arrows
	noecho();
	curs_set(0);
	getmaxyx(stdscr, rows, cols);
	char map[rows][cols];
	
	//create location 
	create(rows-1, cols-1, map, coord);
	
	
	
	
	while ((c = getch()) != 27 ) //ESC 
	{
		if (c == KEY_UP || c == KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT)
		{
			//kill yourself
			if (dangeon(c, rows-1, cols-1, map, coord, leng) == 1)
			{
				mvprintw(rows-1, 0, " !!! YOU KILLED YOURSELF !!! ===> GAME OVER <=== ");
				break;
			}
		}
		
		int r = 3, c = 10;
		mvprintw(0, 0, "%d\n", leng[2]);
		for (int x = 0; x < 200; x++)
		{
			if (c >= 60)
			{
				c = 10;
				r ++;
			}
			//printw("%d  ", leng[x]);
			mvprintw(r, c,"%d ", leng[x]);
			c+=3;
		}
			
		
		
		
	//	printw("\nmov = %d, y = %d, x = %d",leng[leng[0]*3], leng[leng[0]*3 +1], leng[leng[0]*3+2]);
	//	printw("\nmov = %d, y = %d, x = %d",leng[0], leng[1], leng[2]);
		
		mvprintw(rows-1, 0, "rows:%d, cols:%d ", rows, cols);	
		printw("<py:%d px:%d> ", coord[0], coord[1]);
		printw("<hy:%d, hx:%d> ", coord[2], coord[3]);
		printw("<ty:%d, tx:%d > ", coord[4], coord[5]);
		printw("SCORE: %d ", score);
		
	}
	
	

	
	getch();
	endwin();
	return 0 ;
	
}


