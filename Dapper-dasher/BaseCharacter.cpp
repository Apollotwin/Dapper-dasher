#include "BaseCharacter.h"

void BaseCharacter::Tick(float deltaTime)
{
    //Update running time
    runningTime += deltaTime;
    if(runningTime >= updateTime)
    {
        runningTime = 0.0f;
            
        //Update animation frame
        rect.x = frame * rect.width;
        if(!freezeFrame){frame++;}
        if(frame > maxFrame) frame = 0;
    }

    DrawTextureRec(texture,rect,pos,WHITE);
}

void BaseCharacter::Unload_Texture()
{
    UnloadTexture(texture);
}
