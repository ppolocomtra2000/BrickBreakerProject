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
	// mỗi khối là 8pixel
	int nBlock = 8;
	//khởi tạo vị trí miếng hứng
	float fBat = 64.0f;

	//khởi tạo vị trí bóng
	float fBallX = 64.0f;
	float fBallY = 64.0f;
	//tốc độ bóng
	float fBallDX, fBallDY;
	float fSpeed = 40.0f;
	//điểm
	int score = 0;
	int life = 3;
	//gameOver
	bool bResetGame = false;
public:
	bool OnUserCreate() {
		//tạo khung cho sân chơi và các vật cản
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

		//chuyển hướng bóng
		float fAngle = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
		//khởi tạo hướng bóng ( không random hướng)
		fAngle = 0.69f;
		fBallDX = cosf(fAngle);
		fBallDY = sinf(fAngle);
		bResetGame = true;
		return true;
	};


	bool OnUserUpdate(float fElapsedTime) {
		// nếu hết mạng thì thua
		if (life == 0)
		{
			bResetGame = 0;
			DrawString(ScreenWidth() / 2, ScreenHeight() / 2, L"You loose!!", FG_RED);
		}
		// chơi
		else {
			//bề dày của miếng hứng
			int nBatWidth = 15;
			//xóa màn hình
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
			//di chuyển miếng hứng
			if (GetKey(VK_LEFT).bHeld)
				fBat -= 30.0f * fElapsedTime;
			if (GetKey(VK_RIGHT).bHeld)
				fBat += 30.0f * fElapsedTime;
			//miếng hứng không thể ra khỏi phạm vi sân chơi
			if (fBat - nBatWidth < nBlock)
				fBat = nBlock + nBatWidth;
			if (fBat + nBatWidth > nBlock * (nWidth - 2))
				fBat = nBlock * (nWidth - 2) - nBatWidth;
			//vị trí đã qua của bóng
			float fOldX = fBallX;
			float fOldY = fBallY;
			// vị trí mới của bóng
			fBallX += fBallDX * fElapsedTime * fSpeed;
			fBallY += fBallDY * fElapsedTime * fSpeed;
			// ô ( cũ + mới) mà bóng đã đang đi qua
			int nCellOldX = (int)fOldX / nBlock;
			int nCellOldY = (int)fOldY / nBlock;

			int nCellNewX = (int)fBallX / nBlock;
			int nCellNewY = (int)fBallY / nBlock;
			// ô mới mà bóng đang tại vị
			char newCell = map[nCellNewY * nWidth + nCellNewX];
			char oldCell = map[nCellOldY * nWidth + nCellOldX];
			//va chạm với vật cản 
			if (newCell != '.')
			{
				// nếu là gạch thường ( xanh ) thì gạch sẽ bể và điểm tăng 50
				if (newCell == '1') {
					map[nCellNewY * nWidth + nCellNewX] = '.';
					score += 50;
				}
				// nếu là gạch ống ( đỏ ) thì gạch sẽ chuyển sang gạch thường và điểm tăng 100
				if (newCell == '2') {
					map[nCellNewY * nWidth + nCellNewX] = '1';
					score += 100;
				}
				// bóng bật ngược lại nếu va chạm gạch / tường
				if (nCellNewX != nCellOldX)
					fBallDX *= -1;
				if (nCellNewY != nCellOldY)
					fBallDY *= -1;

			}

			//va chạm với vật hứng
			if (fBallY > (nHeight * nBlock - 4))
			{
				//nếu hứng được thì bóng bật ngược lại
				if ((fBallX > fBat - nBatWidth) && (fBallX < fBat + nBatWidth))
				{
					fBallDY *= -1;
					fSpeed *= 1.1;
				}
				// nếu không hứng được thì mất mạng, khởi tạo lại vị trí bóng
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
			//vẽ sân chơi + gạch dựa theo khung 

			for (int i = 0; i < nHeight; i++)
			{
				for (int j = 0; j < nWidth; j++)
				{
					switch (map[i * nWidth + j])
					{
						//sân
					case '#':
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_WHITE);
						break;
						//none
					case '.':
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_BLACK);
						break;
						//gạch thường ( xanh )
					case '1':
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_GREEN);
						break;
						//gạch ống (đỏ )
					case '2': 
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_RED);
						break;
						//bê tông cốt thép ( xanh đen ): không thể phá hủy, đóng vai trò là vật cản
					case '3': 
						Fill(j * nBlock, i * nBlock, (j + 1) * nBlock, (i + 1) * nBlock, PIXEL_SOLID, FG_DARK_BLUE);
						break;
						//hiển thị điểm
					case 's':
						DrawString(j * nBlock, i * nBlock, L"Score: " + to_wstring(score), FG_RED);
						break;
						//hiển thị mạng còn lại
					case 'd':
						DrawString(j * nBlock, i * nBlock, L"Life: " + to_wstring(life), FG_YELLOW);
						break;
					}
				}
			}
			//vẽ bóng
			FillCircle(fBallX, fBallY, 2.0f, PIXEL_SOLID, FG_YELLOW);
			//vẽ vật hứng
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