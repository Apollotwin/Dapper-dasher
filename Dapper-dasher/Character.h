#pragma once
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(int windowWidth,int windowHeight,Texture2D texture2d, int picWidth, int picHeight, int jumpHeight, int Gravity);
    void Tick(float deltaTime) override;
    bool IsOnGround();
    
    bool IsInAir{false};
    bool IsDead{false};
    bool StartGame{false};
    bool QuitGame{false};
    float Velocity{0};
    float JumpVelocity{-600};
private:
    int winHeight{};
    int gravity{};

    Sound 
};
