#define _crt_secure_no_warnings
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_MAGENTA "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define MAX_STACK_SIZE 5000
#define TRUE 1
#define FALSE 0

typedef struct
{
	short int row;
	short int col;
	short int dir;
} element;

#define EXIT_ROW 6
#define EXIT_COL 6
int input_maze[EXIT_ROW][EXIT_COL] = {
	{0, 0, 1, 0, 1, 1},
	{0, 1, 0, 1, 0, 0},
	{1, 0, 1, 0, 1, 1},
	{1, 0, 0, 1, 0, 0},
	{1, 1, 0, 1, 1, 1},
	{1, 0, 1, 0, 0, 0}};

// #define EXIT_ROW 8
// #define EXIT_COL 8
// int input_maze[EXIT_ROW][EXIT_COL] = {
// 	{0, 0, 1, 1, 0, 0, 0, 0},
// 	{0, 1, 0, 1, 0, 1, 1, 0},
// 	{0, 1, 0, 1, 1, 1, 0, 0},
// 	{0, 1, 1, 1, 0, 0, 1, 1},
// 	{0, 0, 0, 0, 0, 1, 0, 0},
// 	{0, 1, 1, 1, 1, 1, 0, 1},
// 	{0, 0, 0, 0, 0, 0, 0, 0},
// 	{0, 1, 0, 1, 0, 1, 1, 0},
// };

int maze[EXIT_ROW + 2][EXIT_COL + 2];
int mark[EXIT_ROW + 2][EXIT_COL + 2];
wchar_t arrow[] = {L'↑', L'↗', L'→', L'↘', L'↓', L'↖', L'←', L'↙'};
element stack[MAX_STACK_SIZE];
element move[8] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
int top = -1;

int stack_full()
{
	if (top >= MAX_STACK_SIZE - 1)
		return (TRUE);
	return (FALSE);
}

int stack_empty()
{
	if (top < -1)
		return (TRUE);
	return (FALSE);
}

void push(element x)
{
	if (stack_full())
	{
		printf("ERROR");
		return;
	}
	top++;
	stack[top] = x;
}

element pop()
{
	element x = {0, 0, 0};
	if (stack_empty())
	{
		printf("ERROR");
		return (x);
	}
	x = stack[top--];
	return (x);
}

int path(const int startDir)
{
	int row, col, nextRow, nextCol, dir, found = FALSE, backtrack = -1;
	element position;
	mark[1][1] = 1;
	stack[0].row = 1;
	stack[0].col = 1;
	stack[0].dir = 0;
	top++;

	while (top > -1 && !found)
	{
		position = pop();
		backtrack++;
		row = position.row;
		col = position.col;
		dir = position.dir;

		while (dir < 8 && !found)
		{
			nextRow = row + move[(dir + startDir) % 8].row;
			nextCol = col + move[(dir + startDir) % 8].col;
			if (nextRow == EXIT_ROW && nextCol == EXIT_COL)
				found = TRUE;
			else if (!mark[nextRow][nextCol] && !maze[nextRow][nextCol])
			{
				mark[nextRow][nextCol] = 1;
				position.row = row;
				position.col = col;
				position.dir = dir + 1;
				push(position);
				row = nextRow;
				col = nextCol;
				dir = 0;
			}
			else
				dir++;
		}
	}
	return (backtrack);
}

void printResult(int startDir, int backtrack)
{
	setlocale(LC_ALL, "");
	for (int i = 0; i <= top; i++)
		mark[stack[i].row][stack[i].col] = 2 + stack[i].dir;
	printf(ANSI_COLOR_GREEN);
	wprintf(L"Start direction: %d(%lc)\n", startDir, arrow[startDir]);
	printf(ANSI_COLOR_RESET);
	printf(ANSI_COLOR_MAGENTA "Total backtracking : %d\n" ANSI_COLOR_RESET, backtrack);
	for (int i = 0; i < EXIT_ROW + 2; i++)
	{
		for (int j = 0; j < EXIT_COL + 2; j++)
		{
			if (maze[i][j] == 1)
				printf("■ ");
			else if (mark[i][j] > 2)
			{
				int tmpDir = (mark[i][j] + startDir - 2) % 8;
				tmpDir = tmpDir ? tmpDir - 1 : 7;
				printf(ANSI_COLOR_GREEN);
				wprintf(L"%lc ", arrow[tmpDir]);
				printf(ANSI_COLOR_RESET);
			}
			else if (mark[i][j] == 1)
				printf(ANSI_COLOR_MAGENTA "◇ " ANSI_COLOR_RESET);
			else
				printf("  ");
		}
		printf("\n");
	}
	printf("Stack info(%d)\n", top);
	for (int i = 0; i <= top; i++)
	{
		printf("(%2d, %2d) ", stack[i].row, stack[i].col);
		wprintf(L"|%2d", (stack[i].dir + startDir) % 8);
		wprintf(L"(%lc)\n", arrow[(stack[i].dir + startDir) % 8]);
	}
	printf("\n\n");
}

void initializeMaze()
{
	for (int i = 0; i < EXIT_ROW + 2; i++)
		for (int j = 0; j < EXIT_COL + 2; j++)
		{
			if ((i == 0 || i == EXIT_ROW + 1) || (j == 0 || j == EXIT_COL + 1))
				maze[i][j] = 1;
			else
				maze[i][j] = input_maze[i - 1][j - 1];
		}
}

void initializeMark()
{
	for (int i = 0; i < EXIT_ROW + 2; i++)
		for (int j = 0; j < EXIT_COL + 2; j++)
			mark[i][j] = 0;
	top = -1;
}

int main(void)
{
	initializeMaze();
	for (int i = 0; i < 8; i++)
	{
		initializeMark();
		printResult(i, path(i));
	}
}