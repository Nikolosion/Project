#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

int nScreenWidth = 175;
int nScreenHeight = 56;
float fPlayerX = 7.0;
float fPlayerY = 7.0;
float fPlayerA = 0.0;
int nMapHeight = 16;
int nMapWidth = 16;
float fFOV = (3.14159 / 7.0);
float fDepth = 350.0;
int x;
int y;
float d;
char map[16][17];
char screen[56][177];
int nCeiling;
int nFloor;
float fDistanceToWall;
int bHitWall;
float fRayAngle;
float fEyeX;
float fEyeY;
int nTestX;
int nTestY;
char nShade;
int oldCoord;
int rebro;
int pov = 15;
int Fps;
float sred = 27.5;
clock_t start, stop;


void MiniMap()
{
	sprintf(map[0],  "################");
	sprintf(map[1],  "#..............#");
	sprintf(map[2],  "#..............#");
	sprintf(map[3],  "#..............#");
	sprintf(map[4],  "#..............#");
	sprintf(map[5],  "#......#.......#");
	sprintf(map[6],  "#.......#......#");
	sprintf(map[7],  "#.....######.#.#");
	sprintf(map[8],  "#............#.#");
	sprintf(map[9],  "#............#.#");
	sprintf(map[10], "#............#.#");
	sprintf(map[11], "#............#.#");
	sprintf(map[12], "#............#.#");
	sprintf(map[13], "#............#.#");
	sprintf(map[14], "#..............#");
	sprintf(map[15], "################");
}
void Contr()
{
	if (map[(int)(fPlayerY + sin(fPlayerA) * 1.0)][(int)(fPlayerX + cos(fPlayerA) * 1.0)] != '#')
		if (GetKeyState('W') < 0)
		{
			fPlayerY += sin(fPlayerA) * 0.2;
			fPlayerX += cos(fPlayerA) * 0.2;
		}
	if (map[(int)(fPlayerY - sin(fPlayerA) * 1.0)][(int)(fPlayerX - cos(fPlayerA) * 1.0)] != '#')
		if (GetKeyState('S') < 0)
		{
			fPlayerY -= sin(fPlayerA) * 0.2;
			fPlayerX -= cos(fPlayerA) * 0.2;
		}
	if (GetKeyState('A') < 0)
		fPlayerA -= (0.19634954);
	if (GetKeyState('D') < 0)
		fPlayerA += (0.19634954);
	if (abs((int)fPlayerA) == 6.0)
		fPlayerA = 0.0;
}

void Clr()
{
	COORD coord;
	coord.Y = 0;
	coord.X = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void RayCst()
{
	for (int x = 0; x <= nScreenWidth; x++)
	{
		rebro = 0;
		fRayAngle = (fPlayerA - fFOV) + ((float)x / (float)nScreenWidth) * fFOV*2;
		fDistanceToWall = 0.05;
		bHitWall = 0;
		fEyeX = cos(fRayAngle);
		fEyeY = sin(fRayAngle);
		while (!bHitWall && fDistanceToWall < fDepth)
		{
			fDistanceToWall += 0.1;
			nTestX = (int)(fPlayerX + (fEyeX*fDistanceToWall));
			nTestY = (int)(fPlayerY + (fEyeY*fDistanceToWall));
			if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
			{
				bHitWall = 1;
				fDistanceToWall = fDepth;
			}
			else if (map[nTestY][nTestX] == '#')
				bHitWall = 1;
		}
		
		if (x != 0 && (oldCoord != nTestX - nTestY))
		{
			rebro = 1;
		}
		else
		{
			rebro = 0;
			pov += 1;
		}

		bHitWall = 0;
			
		if ((int)fDistanceToWall == 0)
		{
			nCeiling = 0;
			nFloor = nScreenHeight;
		}
		else
		{
			nCeiling = (int)(sred * (1 - 1 / (fDistanceToWall)));
			nFloor = nScreenHeight - nCeiling;
		}
		
		for (int a = 0; a < nCeiling; a++)
			screen[a][x] = ' ';
		
		for (int b = nFloor; b < nScreenHeight; b++)
		{
			d = 1.0 - ((float)b - nScreenHeight / 2.0) / ((float)nScreenHeight / 2.0);
			if (d < 0.25)
				nShade = '#';
			else if (d < 0.5)
				nShade = 'x';
			else if (d < 0.75)
				nShade = '~';
			else if (d < 0.9)
				nShade = '-';
			else
				nShade = ' ';
			
			screen[b][x] = nShade;
		}

		for (int c = nCeiling; c < nFloor; c++)
		{
			if (rebro == 1 && pov > 10)
			{
				screen[c][x] = 0x7C;
			}
			else
			{
				if (fDistanceToWall <= fDepth / 3.0)
					screen[c][x] = 0xB2;
				else if (fDistanceToWall < fDepth / 2.0)
					screen[c][x] = 0xB1;
				else if (fDistanceToWall < fDepth / 1.0)
					screen[c][x] = 0xB0;
				else
					screen[c][x] = ' ';
			}
		}
		if (rebro == 1)
			pov = 0;
		oldCoord = nTestX-nTestY;
	}
}

void dop()
{
	map[(int)fPlayerY][(int)fPlayerX] = '@';
	for (int i=0; i<16; i++)
		for (int ii=0; ii<16; ii++)
			screen[i][ii] = map[i][ii];
}

void Play()
{
	for (int d = 0; d < nScreenHeight; d++)
		printf("%s\n", screen[d]);
	printf("Fps=%d", Fps);
}

int main()
{
	do
	{
		
		start = clock();
		MiniMap();
		RayCst();
		dop();
		Play();
		Contr();
		stop = clock();
		Fps = 1000 / (stop - start);
		Sleep(10);
		Clr();
	}
	while(GetKeyState('E') == 0);
	system("cls");
	printf("Hello World");
	return 2730;
}