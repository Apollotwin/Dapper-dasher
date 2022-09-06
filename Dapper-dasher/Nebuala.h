#pragma once
#include <raylib.h>

#include "AnimData.h"

class Nebula
{
public:
    Nebula();

    void InitNebula(float gapDistance  ,int index);
    void Tick(float deltaTime);
    void SetAnimation(AnimData animation);
    Vector2 GetPosition();
    void SetPosition(float x, float y);
    Rectangle GetRect();
    Rectangle GetCollisionRect();
    float GetVelocity();
    float SetVelocity(float multiplier);
    void Unload();
    void ResetToInitPos();
private:
    void UpdateAnimation(float deltaTime);
    AnimData nebula_animation;
    Rectangle collisionRect{};
    void UpdateCollisionRect();
    float nebVel{-200};
    Vector2 initPos{};
};
