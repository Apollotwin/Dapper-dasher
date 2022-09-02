#include "Character.h"
#include <string.h>
#include <string>

Character::Character(int windowWidth, int windowHeight,AnimData run_anim_data, AnimData idle_anim_data, AnimData death_anim_data,float jumpHeight, float Gravity ):
    run_animation(run_anim_data),
    idle_animation(idle_anim_data),
    death_animation(death_anim_data),
    scarfy_animation(idle_anim_data)
{
    JumpVelocity = jumpHeight;
    winWidth = windowWidth;
    winHeight = windowHeight;
    gravity = Gravity;
}

void Character::Tick(float deltaTime)
{
    if(IsKeyPressed(KEY_BACKSPACE))
    {
        IsDead = true;
        PlayDeathSound();
    }
    
    //Ground check
    if(IsOnGround())
    {   //Rectangle is on ground
        FreezeFrame = false;
        IsInAir = false;
        if(Velocity > 0 )
        {
            SetSoundPitch(landSound, -25);
            PlaySound(landSound);
            PlaySound(footsteps[0]);
            Velocity = 0;
        }
    }
    else
    {   // Rectangle is in air
        IsInAir = true;
        FreezeFrame = true;
        Velocity += gravity * deltaTime;
    }

    // Check for jump
    if(IsKeyPressed(KEY_SPACE) && !IsInAir && !IsDead && GetState() != Idle)
    {
        Velocity += JumpVelocity;
        SetSoundPitch(jumpSound, -100.f);
        PlaySound(jumpSound);
        PlaySound(footsteps[1]);
    }

    if(IsDead && GetState() != Dead) SetAnimation(death_animation);

    //Update character position 
    scarfy_animation.pos.y += Velocity * deltaTime;

    UpdateAnimation(deltaTime);
}

void Character::UpdateAnimation(float deltaTime)
{
    //Idle Animation
    if(GetState() == Idle)
    {
        DrawTextureRec(scarfy_animation.texture,scarfy_animation.rect,scarfy_animation.pos,WHITE);
        return;
    }

    //Death Animation
    if(IsDead)
    {
        if(scarfy_animation.frame == scarfy_animation.maxFrame)
        {
            DrawTextureRec(scarfy_animation.texture,scarfy_animation.rect,scarfy_animation.pos,WHITE);
            return;
        }
        scarfy_animation.runningTime += deltaTime;
        if(scarfy_animation.runningTime >= scarfy_animation.updateTime)
        {
            scarfy_animation.runningTime = 0.0f;
            
            //Update animation frame
            scarfy_animation.rect.x = scarfy_animation.frame * scarfy_animation.rect.width;
            if(!FreezeFrame)
            {
                scarfy_animation.frame++;
            }
        }
    
    }
    
    //Running animation
    scarfy_animation.runningTime += deltaTime;
    if(scarfy_animation.runningTime >= scarfy_animation.updateTime)
    {
        scarfy_animation.runningTime = 0.0f;
            
        //Update animation frame
        scarfy_animation.rect.x = scarfy_animation.frame * scarfy_animation.rect.width;
        if(!FreezeFrame)
        {
            scarfy_animation.frame++;
            if(scarfy_animation.frame == 1 || scarfy_animation.frame == 4)
            {
                int chosenFootstep = 0;
                
                if(footstepIndex - 1 >= 2)
                {
                    chosenFootstep = footstepIndex - 1;
                }
                 
                if(!IsSoundPlaying(footsteps[chosenFootstep]))
                {
                    SetSoundVolume(footsteps[chosenFootstep], .5f);
                    SetSoundPitch(footsteps[chosenFootstep], 2.f + static_cast<float>(GetRandomValue(2, 24)) * 0.01f);
                    PlaySound(footsteps[chosenFootstep]);
                }

                if(footstepIndex + 1 < 6)
                {
                    footstepIndex = 0;
                }
                else
                {
                    footstepIndex++;
                }
                
            }
        }
        if(scarfy_animation.frame > scarfy_animation.maxFrame)
        {
            scarfy_animation.frame = 0;
        }
    }
    
    DrawTextureRec(scarfy_animation.texture,scarfy_animation.rect,scarfy_animation.pos,WHITE);
    UpdateCollitionRect();
}

void Character::SetAnimation(AnimData anim_data)
{
    scarfy_animation = anim_data;
}

void Character::PlayDeathSound()
{
    PlaySound(deathSound);
}

State Character::GetState()
{
    return scarfy_animation.animationState;
}

Rectangle Character::GetRect()
{
    return scarfy_animation.rect;
}

Rectangle Character::GetCollisionRect()
{
    return collisionRect;
}

Vector2 Character::GetPosition()
{
    return scarfy_animation.pos;
}

void Character::SetPosition(float x, float y)
{
    scarfy_animation.pos.x = x;
    scarfy_animation.pos.y = y;
}

int index = 0;


bool Character::IsOnGround()
{
    return scarfy_animation.pos.y >= static_cast<float>(winHeight) - scarfy_animation.rect.height;
}

void Character::Unload()
{
    UnloadSound(jumpSound);
    UnloadSound(landSound);
    UnloadSound(deathSound);
    
    for (const auto sound : footsteps)
    {
        UnloadSound(sound);
    }

    UnloadTexture(run_animation.texture);
    UnloadTexture(idle_animation.texture);
    UnloadTexture(death_animation.texture);
}

void Character::UpdateCollitionRect()
{
    const float pad{50};
    collisionRect = {
        scarfy_animation.pos.x + pad/2,
        scarfy_animation.pos.y,
        scarfy_animation.rect.width - pad,
        scarfy_animation.rect.height
    };
}

