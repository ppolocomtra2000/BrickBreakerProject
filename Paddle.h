#pragma once
#include <iostream>
using namespace std;
class Paddle
{
private:
	float pos;
	float width;
	float speed;
public:
	Paddle();
	Paddle(float _pos, float _width);
	float getPos() { return this->pos; };
	float getWidth() { return this->width; };
	float getSpeed() { return this->speed; };
	void setPos(float newPos) { this->pos = newPos; };
	void setSpeed(float newSpeed) { this->speed = newSpeed; };
};

