#include "AnimData.h"

AnimData UpdateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //Update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;
            
        //Update animation frame
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if(data.frame > maxFrame) data.frame = 0;
    }
    return data;
}