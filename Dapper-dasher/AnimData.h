#pragma once
#include <raylib.h>

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    int maxFrame;
    float updateTime;
    float runningTime;
};
