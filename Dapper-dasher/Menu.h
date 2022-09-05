#pragma once
#include "Button.h"
#include "raylib.h"

struct Menu
{
    Menu(int windowWidth, int windowHeight, const char* title, bool isActive);
    void Show(bool isActive, Button button1, Button button2);
    void AddButton(Button button);
    void DrawButton(Button button);
    void DrawTitle(bool isActive);
    
    const char* title;
    Vector2 pos;
    int fontSize;
    Rectangle rect;
    bool isActive;
};


