#pragma once
#include <raylib.h>

#include "AnimData.h"

class Nebula
{
public:
    Nebula(int windowWidth, int windowHeight);
private:
    const Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebulaAnimationData{};
    int nebVel{-200};
};
