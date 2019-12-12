#include <bits/stdc++.h>
#include "olcConsoleGameEngine.h"
#include "Ball.h"
#include "Paddle.h"
using namespace std;
class BrickBreaker :public olcConsoleGameEngine {
private:
	string map;
	//on pixel
	int mapWidth = 17;
	int mapHeight = 15;
	int Block = 8;

	//các vật thể trong trò chơi
	Ball ball;
	Paddle pad;


	//score
	int score = 0;
	int life = 3;

	//gameOver
	bool bResetGame = false;
public:
	BrickBreaker() {};
	bool OnUserCreate() {
		//tạo khung cho sân chơi
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
		//khởi tạo các giá trị khởi đầu của quả bóng
		ball.setX(64.0f);
		ball.setY(64.0f);
		ball.setSpeed(40.0f);


		//khởi tạo các giá trị khởi đầu của ván hứng
		pad.setPos(64.0f);
		pad.setSpeed(30.0f);

		//Góc xác định hướng bật của quả bóng
		float fAngle = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;


		//khởi tạo hướng
		fAngle = 0.69f;
		ball.setDX(cosf(fAngle)) ;
		ball.setDY(sinf(fAngle));
		bResetGame = true;
		return true;
	};


	bool OnUserUpdate(float fElapsedTime) {
		// nếu hết mạng chơi thì game over
		if (life == 0)
		{
			bResetGame = 0;
			DrawString(ScreenWidth() / 2, ScreenHeight() / 2, L"You loose!!", FG_RED);
		}
		else {
			ball.setWidth(15);
			//clrscr
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
			//di chuyển ván hứng
			if (GetKey(VK_LEFT).bHeld)
				pad.setPos(pad.getPos() - pad.getSpeed() * fElapsedTime);
			if (GetKey(VK_RIGHT).bHeld)
				pad.setPos(pad.getPos() + pad.getSpeed() * fElapsedTime);
			//điều kiện để ván không thể di chuyển ra khỏi sân chơi
			if (pad.getPos() - ball.getWidth() < Block)
				pad.setPos(Block + ball.getWidth());
			if (pad.getPos() + ball.getWidth()  > Block * (mapWidth - 2))
				pad.setPos(Block * (mapWidth - 2) - ball.getWidth());

			//xử lí va chạm
			//2 biến này để lưu giữ giá trị  tọa độ hiện thời ( chưa có elapsedTime ) của quả bóng
			float fOldX = ball.getX();
			float fOldY = ball.getY();

			
			// set vị trí mới dựa trên Elapsedtime và tốc độ bóng
			ball.setX(ball.getX() + ball.getDX() * fElapsedTime * ball.getSpeed());
			ball.setY(ball.getY() + ball.getDY() * fElapsedTime * ball.getSpeed());
			
			// cung cấp một ô trong map để thể hiện vị trí của quả bóng dựa trên biến fOldX đã khai báo ở trên
			int nCellOldX = (int)fOldX / Block;
			int nCellOldY = (int)fOldY / Block;
			//cung cấp một ô trong map để thể hiện vị trí của quả bóng hiện thời đang xét ( đã bao gồm Elapsedtime )
			int nCellNewX = (int)ball.getX() / Block;
			int nCellNewY = (int)ball.getY() / Block;
			// chỉ rõ ra ô đó là ô nào trong sân chơi
			char newCell = map[nCellNewY * mapWidth + nCellNewX];
			char oldCell = map[nCellOldY * mapWidth + nCellOldX];
			// va chạm với gạch ( dựa trên ô vị trí của quả bóng và ô vị trí của các viên gạch )
			if (newCell != '.')
			{
				// nếu là gạch thường ( màu xanh ) thì biến mất, score tăng 50
				if (newCell == '1') {
					map[nCellNewY * mapWidth + nCellNewX] = '.';
					score += 50;
				}
				// nếu là gạch ống ( đỏ ) thì biến thành gạch thường, score tăng 100
				if (newCell == '2') {
					map[nCellNewY * mapWidth + nCellNewX] = '1';
					score += 100;
				}
				// va chạm xong thì cho banh bật lại )
				if (nCellNewX != nCellOldX)
					
					ball.setDX(-1 * ball.getDX());
				if (nCellNewY != nCellOldY)
					
					ball.setDY(-1 * ball.getDY());

			}

			// đụng độ với ván hứng
			if (ball.getY() > (mapHeight * Block - 4))
			{	
				// nếu hứng được
				if ((ball.getX() > pad.getPos() - ball.getWidth()) && (ball.getX() < pad.getPos() + ball.getWidth()))
				{
					//bóng bật lại
					ball.setDY(ball.getDY() * -1);
					//tăng tốc độ lên 10%
					ball.setSpeed(ball.getSpeed()*1.1f);
					//tăng tốc độ ván lên 5%
					pad.setSpeed(pad.getSpeed() * 1.05f);
				}
				else
				{
					//không hứng được
					life--;
					// set lại vị trí bóng và tốc độ như ban đầu
					ball.setX((mapWidth / 2.0f) * Block);
					
					ball.setY((mapHeight / 2.0f) * Block);
					float fAngle = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
					ball.setDX(cosf(fAngle));
					ball.setDY(sinf(fAngle));
					ball.setSpeed(40.0f);
				}
			}
			//vẽ sân chơi, bóng, ván hứng

			for (int i = 0; i < mapHeight; i++)
			{
				for (int j = 0; j < mapWidth; j++)
				{
					switch (map[i * mapWidth + j])
					{
					case '#':
						Fill(j * Block, i * Block, (j + 1) * Block, (i + 1) * Block, PIXEL_SOLID, FG_CYAN);
						break;
					case '.':
						Fill(j * Block, i * Block, (j + 1) * Block, (i + 1) * Block, PIXEL_SOLID, FG_BLACK);
						break;
					case '1':// gach thường ( xanh )
						Fill(j * Block, i * Block, (j + 1) * Block, (i + 1) * Block, PIXEL_SOLID, FG_GREEN);
						break;
					case '2': // gạch ống ( đỏ )
						Fill(j * Block, i * Block, (j + 1) * Block, (i + 1) * Block, PIXEL_SOLID, FG_RED);
						break;
					case '3': // bê tông cốt thép, đóng vai trò là vật cản ( không thể phá hủy )
						Fill(j * Block, i * Block, (j + 1) * Block, (i + 1) * Block, PIXEL_SOLID, FG_DARK_BLUE);
						break;
					case 's':
						DrawString(j * Block, i * Block, L"Score: " + to_wstring(score), FG_RED);
						break;
					case 'd':
						DrawString(j * Block, i * Block, L"Life: " + to_wstring(life), FG_YELLOW);
						break;
					}
				}
			}
			//vẽ banh
			FillCircle(ball.getX(), ball.getY(), 2.0f, PIXEL_SOLID, FG_YELLOW);
			//vẽ ván hứng
			DrawLine(pad.getPos() - ball.getWidth(), mapHeight * Block - 2, pad.getPos() + ball.getWidth(), mapHeight * Block - 2, PIXEL_SOLID, FG_MAGENTA);
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