#pragma once
#include <raylib.h>

class BaseCharacter
{
public:
    virtual ~BaseCharacter() = default;
    virtual void Tick(float deltaTime);
    virtual void Unload_Texture();
protected:
    Texture2D texture{};
    Rectangle rect{};
    Vector2 pos{};
    
    //Animation
    int frame{0};
    bool freezeFrame{false};
    int maxFrame{5};
    float runningTime{0.0f};
    float updateTime{1.f/12.0f};
    
};
