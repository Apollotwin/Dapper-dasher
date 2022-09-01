#include "Debugger.h"
#include <raylib.h>

void Debugger::DebugLog(const char* text, int winWidth, int winHeight) 
{
    Vector2 txtSize = MeasureTextEx(GetFontDefault(), text,50.f,0.f);
    DrawText(text, winWidth/4 - txtSize.x/2,winHeight/8 * 7, 50.f,WHITE );
}
