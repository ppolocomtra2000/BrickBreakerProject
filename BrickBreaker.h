//
// Created by quan on 14/12/2019.
//

#ifndef BRICK_BREAKER_BRICKBREAKER_H
#define BRICK_BREAKER_BRICKBREAKER_H

#include <bits/stdc++.h>
#include "olcConsoleGameEngine.h"
#include "Ball.h"
#include "Paddle.h"
#include "pixel_art.h"

class menu;

class BrickBreaker : public olcConsoleGameEngine {
private:
    string map;
    //on pixel
    int mapWidth = 21;
    int mapHeight = 22;
    int playHeight = 18;
    int Block = 8;
    int level;
    float double_score_left = 0;
    float x_spawn_chance = 0.1;
    float u_spawn_chance = 0.06;

    //các vật thể trong trò chơi
    Ball ball;
    Paddle pad;

    //score
    int score, highscore;
    int life;
    int hit_left = 0;
    CHAR_INFO *captured_screen;

    void CaptureScreen();
    void OnDamage(int x, int y);
public:
    menu* active_menu;

    BrickBreaker() = default;

    void DrawGlyph(int& x, int y, char ch, short color);
    void DrawGlyph(int x, int y, const pixel_glyph& glyph, short color);
    void DrawString(int x, int y, const string& str, short color);
    void FillCell(int x, int y, short color, wchar_t c = PIXEL_SOLID);
    int RoughXCenter();

    void ResetBall();
    void RecreateMap ();

    void Save();
    bool Load();

    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;
};


#endif //BRICK_BREAKER_BRICKBREAKER_H
