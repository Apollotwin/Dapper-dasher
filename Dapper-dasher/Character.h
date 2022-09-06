#pragma once
#include "AnimData.h"
#include "BaseCharacter.h"
#include "Debugger.h"


class Character 
{
public:
    Character(
        int windowWidth,
        int windowHeight,
        AnimData run_anim_data,
        AnimData idle_anim_data,
        AnimData death_anim_data,
        float jumpHeight,
        float Gravity);
    
    void Tick(float deltaTime);
    void UpdateAnimation(float deltaTime);
    void SetAnimation(AnimData anim_data);
    void PlayDeathSound();
    State GetState();
    Rectangle GetRect();
    Rectangle GetCollisionRect();
    Vector2 GetPosition();
    void SetPosition(float x, float y);
    bool IsOnGround();
    void Unload();

    float Velocity{0};
    float JumpVelocity{-600};
    bool IsInAir{false};
    bool IsDead{false};
    bool StartGame{false};
    bool FreezeFrame{false};
    
    AnimData run_animation;
    AnimData idle_animation;
    AnimData death_animation;

    Sound deathSound { LoadSound("Sound/Death.wav") };
    
private:
    Debugger debugger{};
    int winWidth{};
    int winHeight{};
    float gravity{};
    Rectangle collisionRect{};

    void UpdateCollitionRect();

    //Sound
    int footstepIndex{0};
    Sound jumpSound { LoadSound("Sound/Jump_vox.wav") };
    Sound landSound { LoadSound("Sound/Land_vox.wav") };
    Sound footsteps[6] = {
        LoadSound("Sound/Footstep_0.wav"),
        LoadSound("Sound/Footstep_1.wav"),
        LoadSound("Sound/Footstep_2.wav"),
        LoadSound("Sound/Footstep_3.wav"),
        LoadSound("Sound/Footstep_4.wav"),
        LoadSound("Sound/Footstep_5.wav"),
    };
    
    //Animation
    AnimData scarfy_animation;
    int frame{0};
    int maxFrame{5};
    float runningTime{0.0f};
    float updateTime{1.f/12.0f};
};
