#include "AnimData.h"

void AnimData::InitAnimData(int windowWidth, int windowHeight,Texture2D texture2D ,Anim_Data_Position position,State animation_state ,int picWidth,int picHeight)
{
    texture = texture2D;
    rect.width = static_cast<float>(texture.width)/static_cast<float>(picWidth);
    rect.height = static_cast<float>(texture.height)/static_cast<float>(picHeight);
    rect.x = 0;
    rect.y = 0;
    
    if(position == Center) pos.x = static_cast<float>(windowWidth)/2 - rect.width/2;
    if(position == Off_Screen_Right) pos.x = static_cast<float>(windowWidth);
    if(position == Off_Screen_Left) pos.x = rect.width * -1;
    
    pos.y = static_cast<float>(windowHeight) - rect.height;
    
    frame = 0;
    maxFrame = (picWidth * picHeight) - 1;
    runningTime = 0.0f;
    updateTime = 1.f/12.0f;
    animationState = animation_state;
}

AnimData AnimData::UpdateAnimData(AnimData data, float deltaTime)
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
    return data;
}
