#include "AnimData.h"

AnimData::AnimData(Texture2D texture, int picWidth, int picHeight, int winWidth, int winHeight, State animState) : texture(texture), animationState(animState)
{
    rect.width = static_cast<float>(texture.width)/static_cast<float>(picWidth);
    rect.height = static_cast<float>(texture.height)/static_cast<float>(picHeight);
    rect.x = 0;
    rect.y = 0;
    pos.x = static_cast<float>(winWidth) / 2 - rect.width / 2;
    pos.y = static_cast<float>(winHeight) - rect.height; 
    frame = 0;
    maxFrame = (picWidth * picHeight) - 1;
    runningTime = 0.0f;
    updateTime = 1.f/12.0f;
}

void AnimData::UpdateAnimData(AnimData data, float deltaTime)
{
    //Update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;
            
        //Update animation frame
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if(data.frame > data.maxFrame) data.frame = 0;
    }
}
