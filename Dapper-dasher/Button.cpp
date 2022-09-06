#include "Button.h"

Button::Button(const char* title, int windowWidth, int windowHeight, int sizeX, int sizeY, int fontSize,
    Color txt_Color, Color bg_color, Color hover_color) : text(title)
{
    text = title;
    textColor = txt_Color;
    textOriginalColor = txt_Color;
    textHoverColor = WHITE;
    bgColor = bg_color;
    hoverColor = hover_color;
    rectColor = bgColor;
    pos.x = windowWidth/2;
    pos.y = windowHeight/5 * 2;
    rect.x = pos.x - sizeX/2;
    rect.y = pos.y - sizeY/2;
    rect.width = sizeX;
    rect.height = sizeY;
    this->fontSize = fontSize;
}

bool Button::MouseOverButton(Vector2 mousePos)
{
    if(CheckCollisionPointRec(mousePos,rect))
    {
        textColor = textHoverColor;
        rectColor = hoverColor;
        return true;
    }
    
    textColor = textOriginalColor;
    rectColor = bgColor;

    return false;
}

void Button::ChangeColor(Color color)
{
    rectColor = color;
}

bool Button::Clicked(Vector2 mousePos)
{
    if(CheckCollisionPointRec(mousePos,rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return true;
    }
    
    return false;
}

bool Button::Down(Vector2 mousePos)
{
    if(MouseOverButton(mousePos) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        return true;
    }
    
    return Released();
}

bool Button::Released()
{
    return IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}


void Button::UpdateTextPos(float pad)
{
    while (MeasureText(text, this->fontSize) > rect.width - pad) this->fontSize--;

    Vector2 textSize = MeasureTextEx(GetFontDefault(),text,this->fontSize,0.f);

    if(pad == 0.f)
        txt_pos.x = rect.x + rect.width/2 - textSize.x/2;
    else
        txt_pos.x = rect.x + pad/2;
    txt_pos.y = rect.y + rect.height/2 - textSize.y/2;
}

void Button::UpdatePosition(Vector2 newPos)
{
    rect.x = newPos.x;
    rect.y = newPos.y;
}
