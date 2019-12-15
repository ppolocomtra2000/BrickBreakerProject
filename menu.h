//
// Created by quan on 14/12/2019.
//

#ifndef BRICK_BREAKER_MENU_H
#define BRICK_BREAKER_MENU_H

#include <string>
#include "olcConsoleGameEngine.h"
using namespace std;


class menu {
protected:
    // tên hiển thị của các lựa chọn
    string choice1;
    string choice2;
    string choice3;
    // màu tiêu đề menu
    short message_color;
    // màu hiển thị các lựa chọn
    short choice_color;
public:
    // lựa chọn hiện tại
    int current_choice{};
    // tiêu đề menu
    string message;
    // khởi tạo menu
    menu(string message, string choice1, string choice2, string choice3);
    // vẽ một frame và update menu
    virtual void DrawMenu();
protected:
    // hành động của các lựa chọn
    virtual void action1() = 0;
    virtual void action2() = 0;
    virtual void action3() = 0;
};


#endif //BRICK_BREAKER_MENU_H
