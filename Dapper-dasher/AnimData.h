#pragma once
#include <raylib.h>

enum State
{
    Idle, Running, Dead
};

struct AnimData
{
    Texture2D texture{};
    Rectangle rect{};
    Vector2 pos{};
    int frame{};
    int maxFrame{};
    float updateTime{1.f/12.0f};
    float runningTime{0.0f};
    State animationState;
    
    AnimData(Texture2D texture, int picWidth, int picHeight, int winWidth, int winHeight, State animState);
    void UpdateAnimData(AnimData data, float deltaTime);
};


