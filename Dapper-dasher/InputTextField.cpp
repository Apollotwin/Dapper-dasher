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
    DrawText(enterText, (float)textBoxRect.x + 4.f, textBoxRect.y - textBoxRect.height * 0.6f - 1.f,fontSize, highLightColor);
    DrawText(enterText, (float)textBoxRect.x + 5.f, textBoxRect.y - textBoxRect.height * 0.6f,fontSize, rectLineColor);

    MouseOverInputField(GetMousePosition());
    InputFieldClicked(GetMousePosition());

    if(highLighted)
    {
        key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (letterCount < 21))
            {
                input[letterCount] = static_cast<char>(key);
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

    if (highLighted) frameCounter++;
    else frameCounter = 0;

    inputFontsize = 0;
    int InputTextWidth = MeasureText(input, fontSize - 25);
    DrawText(input, textBoxRect.x + textBoxRect.width/2 - InputTextWidth/2 - 40.f, textBoxRect.y + textBoxRect.height/3 , fontSize - 20, WHITE);

    const char* charCounter = TextFormat("Characters: %i/%i", letterCount, 21);
    int textWidth = MeasureText(charCounter, fontSize - 25);
    DrawText(charCounter, textBoxRect.x + textBoxRect.width/2 - textWidth/2 - 1.f, textBoxRect.y + textBoxRect.height + 20.f -1.f, fontSize - 25, DARKPURPLE);
    DrawText(charCounter, textBoxRect.x + textBoxRect.width/2 - textWidth/2, textBoxRect.y + textBoxRect.height + 20.f , fontSize - 25, WHITE);

    if (letterCount <= 0)
    {
        // Draw blinking underscore char
        if (((frameCounter/60)%2) == 0) DrawText("_", (int)textBoxRect.x + 15.f + (float)MeasureText(input, fontSize), textBoxRect.y + textBoxRect.height/2 - 5.f, fontSize, rectLineColor);
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

bool InputTextField::IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetCharPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}
