#pragma once

#include <raylib.h>

class InputTextField
{
public:
    InputTextField(int winWidth,int winHeight, int posX, int posY);
    void Update(float deltaTime);
    bool MouseOverInputField(Vector2 mousePos);
    bool InputFieldClicked(Vector2 mousePos);
    char GetInput() { return *input; }
    bool IsHighlighted(){ return highLighted; }
    bool IsAnyKeyPressed();
private:
    Rectangle textBoxRect{};
    Rectangle backgroundRect{};
    Rectangle outerLineRect{};
    Color highLightColor = DARKPURPLE;
    Color rectLineColor = WHITE;
    int letterCount = 0;
    char input[21 + 1] = "\0";
    int inputFontsize{0};
    int key{0};
    int fontSize{0};
    bool highLighted = false;
    const char* enterText{"Enter nickname . . ."};
    int frameCounter{0};
};
