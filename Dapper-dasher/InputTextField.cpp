#include "InputTextField.h"

InputTextField::InputTextField(int winWidth, int winHeight, int posX, int posY)
{
    textBoxRect = {
        static_cast<float>(winWidth)/posX,
        static_cast<float>(winHeight)/16 * posY,
        600, 100
    };

    textBoxRect.x -= textBoxRect.width/2;
    backgroundRect = textBoxRect;
    outerLineRect = textBoxRect;

    //Set the fontsize depending on menu width
    while (MeasureText(enterText, fontSize) < textBoxRect.width - 30.f) fontSize++;
    
}

void InputTextField::Update(float deltaTime)
{
    DrawRectangleRec(textBoxRect,highLightColor);
    DrawRectangleLinesEx(backgroundRect,5.f,rectLineColor);
    DrawRectangleLinesEx(outerLineRect,2.5f, highLightColor);
    DrawText(enterText, (float)textBoxRect.x + 10.f, textBoxRect.y - textBoxRect.height * 0.6f,fontSize, WHITE);

    MouseOverInputField(GetMousePosition());
    InputFieldClicked(GetMousePosition());

    if(highLighted)
    {
        key = GetCharPressed();

        while (key < 0)
        {
            if ((key >= 32) && (key <= 125) && (letterCount < 21))
            {
                input[letterCount] = (char)key;
                input[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            key = GetCharPressed();  
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            input[letterCount] = '\0';
        }
    }
}

bool InputTextField::MouseOverInputField(Vector2 mousePos)
{
    if(CheckCollisionPointRec(mousePos,textBoxRect))
    {
        highLightColor = DARKPURPLE;
        rectLineColor = WHITE;
        return true;
    }

    if(!highLighted)
    {
        rectLineColor = DARKPURPLE;
        highLightColor = WHITE;
    }
    else
    {
        rectLineColor = WHITE;
        highLightColor = DARKPURPLE;
    }
    
    return false;
}

bool InputTextField::InputFieldClicked(Vector2 mousePos)
{
    if(MouseOverInputField(mousePos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        highLighted = true;
        return true;
    }

    if(!MouseOverInputField(mousePos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        highLighted = false;
    }
    
    return false;
}
