#pragma once
#include <iostream>

using namespace std;

class Ball
{
private:
	float x, y;
	float dx, dy;	//tốc độ 
	float speed;
	float width;
public:
	Ball();
	Ball(float _x, float _y, float _dx, float _dy, float _speed);
	float getX() { return this->x; };
	float getY() { return this->y; };
	float getDX() { return this->dx; };
	float getDY() { return this->dy; };
	float getSpeed() { return this->speed; };
	float getWidth() { return this->width; };
	void setX(float newX) { this->x = newX; };
	void setY(float newY) { this->y = newY; };
	void setDX(float newDx) { this->dx = newDx; };
	void setDY(float newDy) { this->dy = newDy; };
	void setSpeed(float newSpeed) { this->speed = newSpeed; };
	void setWidth(float newWidth) { this->width = newWidth; };
};

