#pragma once
#include <iostream>

using namespace std;

class Ball
{
private:
	float width;
public:
    float x, y;
    float dx, dy;	//tốc độ
    float speed;

	Ball();
	Ball(float _x, float _y, float _dx, float _dy, float _speed);
	float getWidth() { return this->width; };
	void setWidth(float newWidth) { this->width = newWidth; };
};

