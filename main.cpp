#include <bits/stdc++.h>
#include "olcConsoleGameEngine.h"

using namespace std;
class BrickBreaker :public olcConsoleGameEngine {
public:
	BrickBreaker() {};
	string map;
	//on pixel
	int nWidth = 17;
	int nHeight = 15;
	int nBlock = 8;
	//init Bat position
	float fBat = 64.0f;

	//init Ball position
	float fBallX = 64.0f;
	float fBallY = 64.0f;
	float fBallDX, fBallDY;
	float fSpeed = 40.0f;
	//score
	int score = 0;
	int life = 3;
	//gameOver
	bool bResetGame = false;
public:
	bool OnUserCreate() {
		//create map skeleton
		map += "################.";
		map += "#21222122212211#.";
		map += "#21211111112112#.";
		map += "#12111221212211#.";
		map += "#.....3...3...3#.";
		map += "#..............#s";
		map += "#..............#.";
		map += "#..............#d";
		map += "#..............#.";
		map += "#..............#.";
		map += "#..............#.";
		map += "#..............#.";
		map += "#..............#.";
		map += "#..............#.";
		map += "#..............#.";

		//physically direction 
		float fAngle = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
		//itnit direction
		fAngle = 0.69f;
		fBallDX = cosf(fAngle);
		fBallDY = sinf(fAngle);
		bResetGame = true;
		return true;
	};


	bool OnUserUpdate(float fElapsedTime) {
		if (life == 0)
		{
			bResetGame = 0;
			DrawString(ScreenWidth() / 2, ScreenHeight() / 2, L"You loose!!", FG_RED);
		}
		else {
			int nBatWidth = 15;
			//clrscr
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
			//move Bat
			if (GetKey(VK_LEFT).bHeld)
				fBat -= 30.0f * fElapsedTime;
			if (GetKey(VK_RIGHT).bHeld)
				fBat += 30.0f * fElapsedTime;
			//Bat not out of range
			if (fBat - nBatWidth < nBlock)
				fBat = nBlock + nBatWidth;
			if (fBat + nBatWidth > nBlock * (nWidth - 2))
				fBat = nBlock * (nWidth - 2) - nBatWidth;

			float fOldX = fBallX;
			float fOldY = fBallY;

			fBallX += fBallDX * fElapsedTime * fSpeed;
			fBallY += fBallDY * fElapsedTime * fSpeed;

			int nCellOldX = (int)fOldX / nBlock;
			int nCellOldY = (int)fOldY / nBlock;

			int nCellNewX = (int)fBallX / nBlock;
			int nCellNewY = (int)fBallY / nBlock;

			char newCell = map[nCellNewY * nWidth + nCellNewX];
			char oldCell = map[nCellOldY * nWidth + nCellOldX];
			//collision with brick
			if (newCell != '.')
			{
				if (newCell == '1') {
					map[nCellNewY * nWidth + nCellNewX] = '.';
					score += 50;
				}
				if (newCell == '2') {
					map[nCellNewY * nWidth + nCellNewX] = '1';
					score += 100;
				}
				if (nCellNewX != nCellOldX)
					fBallDX *= -1;
				if (nCellNewY != nCellOldY)
					fBallDY *= -1;

			}

			//collision with Bat
			if (fBallY > (nHeight * nBlock - 4))
			{
				if ((fBallX > fBat - nBatWidth) && (fBallX < fBat + nBatWidth))
				{
					fBallDY *= -1;
					fSpeed *= 1.1;
				}
				else
				{
					//Dead
					life--;
					fBallX = (nWidth / 2.0f) * nBlock;
					fBallY = (nHeight / 2.0f) * nBlock;
					float fAngle = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
					fBallDX = cosf(fAngle);
					fBallDY = sinf(fAngle);
				}
			}
			//draw map

			for (int i = 0; i < nHeight; i++)
			{
				for (int j = 0; j < nWidth; j++)
				{
					switch (map[i * nWidth + j])
					{
					case '#':
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_WHITE);
						break;
					case '.':
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_BLACK);
						break;
					case '1':// normal brick
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_GREEN);
						break;
					case '2': // normal++ brick
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_RED);
						break;
					case '3': // unbreakable brick
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_DARK_BLUE);
						break;
					case 's':
						DrawString(j * nBlock, i * nBlock, L"Score: " + to_wstring(score), FG_RED);
						break;
					case 'd':
						DrawString(j * nBlock, i * nBlock, L"Life: " + to_wstring(life), FG_YELLOW);
						break;
					}
				}
			}
			//draw ball
			FillCircle(fBallX, fBallY, 2.0f, PIXEL_SOLID, FG_YELLOW);
			//draw Bat
			DrawLine(fBat - nBatWidth, nHeight * nBlock - 2, fBat + nBatWidth, nHeight * nBlock - 2, PIXEL_SOLID, FG_WHITE);
			return true;
		}
	};
};
int main()
{
	BrickBreaker game;
	game.ConstructConsole(160, 160, 4, 4);
	game.Start();
	return 0;
}