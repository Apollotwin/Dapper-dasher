#include "Nebuala.h"


Nebula::Nebula()
= default;

void Nebula::InitNebula(float gapDistance ,int index)
{
    if(index <= 1)
        nebula_animation.pos.x += gapDistance;
    else
        nebula_animation.pos.x += gapDistance * index;

    initPos.x = nebula_animation.pos.x;

    nebula_animation.frame = GetRandomValue(0, nebula_animation.maxFrame);
    UpdateCollisionRect();
}

void Nebula::Tick(float deltaTime)
{
    UpdateAnimation(deltaTime);
    UpdateCollisionRect();
}

void Nebula::SetAnimation(AnimData animation)
{
    nebula_animation = animation;
}

void Nebula::UpdateAnimation(float deltaTime)
{
    nebula_animation.pos.x += static_cast<float>(nebVel) * deltaTime;
    
    //Update running time
    nebula_animation.runningTime += deltaTime;
    if(nebula_animation.runningTime >= nebula_animation.updateTime)
    {
        nebula_animation.runningTime = 0.0f;
        
        //Update animation frame
        nebula_animation.rect.x = static_cast<float>(nebula_animation.frame) * nebula_animation.rect.width;
        nebula_animation.frame++;
        if(nebula_animation.frame > nebula_animation.maxFrame) nebula_animation.frame = 0;
    }

    DrawTextureRec(nebula_animation.texture,nebula_animation.rect,nebula_animation.pos,WHITE);
}

void Nebula::UpdateCollisionRect()
{
    const float pad{50};
    collisionRect = {
        nebula_animation.pos.x + pad/2,
        nebula_animation.pos.y + pad/2,
        nebula_animation.rect.width - pad,
        nebula_animation.rect.height
        };
}

Vector2 Nebula::GetPosition()
{
    return nebula_animation.pos;
}

void Nebula::SetPosition(float x, float y)
{
    nebula_animation.pos.x = x;
    nebula_animation.pos.y = y;
}

Rectangle Nebula::GetRect()
{
    return nebula_animation.rect;
}

Rectangle Nebula::GetCollisionRect()
{
    return collisionRect;
}

float Nebula::GetVelocity()
{
    return nebVel;
}

float Nebula::SetVelocity(float multiplier)
{
    return nebVel += multiplier * -1;
}

void Nebula::Unload()
{
    UnloadTexture(nebula_animation.texture);
}

void Nebula::ResetToInitPos()
{
    nebula_animation.pos.x = initPos.x;
}

