#include "Character.h"

#include <string>

#include "Debugger.h"

Character::Character(int windowWidth, int windowHeight,AnimData run_anim_data, AnimData idle_anim_data, AnimData death_anim_data,int jumpHeight, int Gravity ):
    scarfy_animation(idle_anim_data),
    runAnimData(run_anim_data),
    idleAnimData(idle_anim_data),
    deathAnimData(death_anim_data)
{
    JumpVelocity = static_cast<float>(jumpHeight);
    winWidth = windowWidth;
    winHeight = windowHeight;
    gravity = Gravity;
}

void Character::Tick(float deltaTime)
{
    //Update character position 
    scarfy_animation.pos.y += Velocity * deltaTime;

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
            PlaySound(landSound);
            PlaySound(footsteps[0]);
            Velocity = 0;
        }
    }
    else
    {   // Rectangle is in air
        IsInAir = true;
        FreezeFrame = true;
        Velocity += static_cast<float>(gravity) * deltaTime;
    }

    // Check for jump
    if(IsKeyPressed(KEY_SPACE) && !IsInAir && !IsDead && GetState() != Idle)
    {
        Velocity += JumpVelocity;
        PlaySound(jumpSound);
        PlaySound(footsteps[1]);
    }

    if(IsDead && GetState() != Dead) SetAnimationData(deathAnimData);

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
}

void Character::SetAnimationData(AnimData anim_data)
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

int index = 0;


bool Character::IsOnGround()
{
    return scarfy_animation.pos.y >= static_cast<float>(winHeight) - scarfy_animation.rect.height;
}

