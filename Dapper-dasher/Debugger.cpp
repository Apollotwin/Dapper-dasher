#include "Debugger.h"
#include <raylib.h>
#include <string>

void Debugger::DebugLog(std::string text, int winWidth, int winHeight) 
{
    Vector2 txtSize = MeasureTextEx(GetFontDefault(), text.c_str(),50.f,0.f);
    DrawText(text.c_str(), winWidth/4 - txtSize.x/2,winHeight/8 * 7, 50.f,WHITE );
}
