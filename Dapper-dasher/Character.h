#pragma once
#include "AnimData.h"
#include "BaseCharacter.h"
#include "Debugger.h"
#include "TextureData.h"

class Character 
{
public:
    Character(
        int windowWidth,
        int windowHeight,
        AnimData run_anim_data,
        AnimData idle_anim_data,
        AnimData death_anim_data,
        int jumpHeight,
        int Gravity);
    
    void Tick(float deltaTime);
    void UpdateAnimation(float deltaTime);
    void SetAnimationData(AnimData anim_data);
    void PlayDeathSound();
    State GetState();
    bool IsOnGround();
    
    bool IsInAir{false};
    bool IsDead{false};
    bool StartGame{false};
    bool QuitGame{false};
    float Velocity{0};
    float JumpVelocity{-600};
    bool FreezeFrame{false};
    
    AnimData runAnimData;
    AnimData idleAnimData;
    AnimData deathAnimData;
    
private:
    AnimData scarfy_animation;
    int winWidth{};
    int winHeight{};
    int gravity{};
    Debugger debugger{};
    Rectangle rect{};
    Vector2 pos{};

    //Sound
    Sound jumpSound { LoadSound("Sound/Jump_vox.wav") };
    Sound landSound { LoadSound("Sound/Land_vox.wav") };
    Sound deathSound { LoadSound("Sound/Death.wav") };
    int footstepIndex{0};
    Sound footsteps[6] = {
        LoadSound("Sound/Footstep_0.wav"),
        LoadSound("Sound/Footstep_1.wav"),
        LoadSound("Sound/Footstep_2.wav"),
        LoadSound("Sound/Footstep_3.wav"),
        LoadSound("Sound/Footstep_4.wav"),
        LoadSound("Sound/Footstep_5.wav"),
    };
    
    //Animation
    int frame{0};
    int maxFrame{5};
    float runningTime{0.0f};
    float updateTime{1.f/12.0f};
};
