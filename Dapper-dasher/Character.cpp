#include "Character.h"

Character::Character(int windowWidth, int windowHeight,Texture2D texture2D ,int picWidth, int picHeight,int jumpHeight, int Gravity ) 
{
    texture = texture2D;
    rect.width = static_cast<float>(texture.width)/static_cast<float>(picWidth);
    rect.height = static_cast<float>(texture.height)/static_cast<float>(picHeight);
    rect.x = 0;
    rect.y = 0;
    pos.x = static_cast<float>(windowWidth)/2 - rect.width/2;
    pos.y = static_cast<float>(windowHeight) - rect.height;
    frame = 0;
    maxFrame = (picWidth * picHeight) - 1;
    JumpVelocity = static_cast<float>(jumpHeight);
    winHeight = windowHeight;
    gravity = Gravity;
}

void Character::Tick(float deltaTime)
{
    //Ground check
    if(IsOnGround())
    {   //Rectangle is on ground
        freezeFrame = false;
        IsInAir = false;
        Velocity = 0;
    }
    else
    {   // Rectangle is in air
        IsInAir = true;
        freezeFrame = true;
        Velocity += static_cast<float>(gravity) * deltaTime;
    }

    // Check for jump
    if(IsKeyPressed(KEY_SPACE) && !IsInAir) Velocity += JumpVelocity;
        
    //Update scarfy position 
    pos.y += Velocity * deltaTime;
    
    BaseCharacter::Tick(deltaTime);
}

bool Character::IsOnGround()
{
    return pos.y >= static_cast<float>(winHeight) - rect.height;
}

