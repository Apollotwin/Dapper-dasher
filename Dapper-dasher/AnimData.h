#pragma once
#include <raylib.h>

enum State
{
    Idle, Running, Dead , None = 0
};

enum Anim_Data_Position
{
    Center, Off_Screen_Right, Off_Screen_Left
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

    void InitAnimData(int windowWidth, int windowHeight,Texture2D texture2D ,Anim_Data_Position position,State animation_state ,int picWidth,int picHeight);
    AnimData UpdateAnimData(AnimData data, float deltaTime);
};


