#pragma once
#include "AnimData.h"

class Character
{
public:
    Character();
private:
    const Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    
};
