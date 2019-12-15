#pragma once
#include <iostream>
using namespace std;
class Paddle
{
private:
public:
    float pos;
    float width;
    float speed;
	Paddle();
	Paddle(float _pos, float _width);
};

