#include "Nebuala.h"

Nebula::Nebula(int windowWidth, int windowHeight)
{
    nebulaAnimationData.rect.x = 0.0;
    nebulaAnimationData.rect.y = 0.0;
    nebulaAnimationData.rect.width = nebula.width/8;
    nebulaAnimationData.rect.height = nebula.height/8;
    nebulaAnimationData.pos.y = windowHeight - nebula.height/8;
    nebulaAnimationData.pos.x = windowWidth + (300 * 0);
    nebulaAnimationData.frame = 0;
    nebulaAnimationData.maxFrame = 7;
    nebulaAnimationData.runningTime = 1.0/12.0;
    nebulaAnimationData.updateTime = 0.0;
}


// void Nebula::InitNebulae(const Texture2D& nebula,const int nebulaeAmount, AnimData(nebulea)[15])
// {
//     for (int i = 0; i < nebulaeAmount; i++)
//     {
//         nebulea[i].rect.x = 0.0;
//         nebulea[i].rect.y = 0.0;
//         nebulea[i].rect.width = nebula.width/8;
//         nebulea[i].rect.height = nebula.height/8;
//         nebulea[i].pos.y = windowHeight - nebula.height/8;
//         nebulea[i].pos.x = windowWidth + (300 * i);
//         nebulea[i].frame = i;
//         nebulea[i].maxFrame = 7;
//         nebulea[i].runningTime = 1.0/12.0;
//         nebulea[i].updateTime = 0.0;
//     }
// }

