#pragma once
#include <raylib.h>

struct Button
{
    Button(const char* title, int windowWidth, int windowHeight,int sizeX, int sizeY,int fontSize, Color txt_Color, Color bg_color, Color hover_color);
    bool MouseOverButton(Vector2 mousePos);
    bool MouseExitButton(Vector2 mousePos);
    void ChangeColor(Color color);
    bool Clicked(Vector2 mousePos);
    bool Down(Vector2 mousePos);
    void UpdateTextPos(float pad);
    void UpdatePosition(Vector2 newPos);

    const char* text;
    int fontSize;
    Color textColor;
    Color textOriginalColor;
    Color textHoverColor;
    Color bgColor;
    Color hoverColor;
    Color rectColor;
    Vector2 txt_pos;
    Vector2 pos;
    Rectangle rect;
    bool playClickSound{true};
    bool playHoverSound{true};
    
};
