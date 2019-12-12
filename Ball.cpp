#include "Ball.h"

Ball::Ball(){}

Ball::Ball(float _x, float _y, float _dx, float _dy, float _speed)
{
	this->x = _x;
	this->y = _y;
	this->dx = _dx;
	this->dy = _dy;
	this->speed = _speed;
}