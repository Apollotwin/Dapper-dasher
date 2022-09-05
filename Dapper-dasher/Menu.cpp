#include "Menu.h"

Menu::Menu(int windowWidth, int windowHeight,const char* title, bool isActive): title(title), isActive(isActive)
{
    rect.x = windowWidth / 5;
    rect.y = windowHeight / 8;
    rect.width = windowWidth / 5 * 3;
    rect.height = windowHeight / 8 * 4;
    pos.x = rect.x;
    pos.y = rect.y;
    fontSize = rect.width;

    //Set the fontsize depending on menu width
    while (MeasureText(title, fontSize) > rect.width) fontSize--;
}

void Menu::Show(bool isActive, Button button1, Button button2)
{
    this->isActive = isActive;
    if(isActive)
    {
        DrawText(title,pos.x,pos.y,fontSize,WHITE);
        DrawText(title,pos.x + 2,pos.y + 2,fontSize,DARKPURPLE);
        DrawButton(button1);
        DrawButton(button2);
    }
}

void Menu::AddButton(Button button) 
{
    
}

void Menu::DrawButton(Button button)
{
    DrawRectangle(button.rect.x,button.rect.y,button.rect.width,button.rect.height, button.rectColor);
    DrawText(button.text, button.txt_pos.x, button.txt_pos.y, button.fontSize, button.textColor);
}

void Menu::DrawTitle(bool isActive)
{
    if(!isActive) return;
    DrawText(title,pos.x,pos.y,fontSize,WHITE);
    DrawText(title,pos.x + 2,pos.y + 2,fontSize,DARKPURPLE);
}


/*void Menu::AddButton()
{
    Button button{{title},{txt_Color},{bg_color},{hover_color}};

    int index = amountOfButtons;
    amountOfButtons++;
    Button newButtonArr[amountOfButtons]{};

    for(int i = 0; i < index; i++)
    {
        newButtonArr[i] = buttons[i];
    }

    newButtonArr[amountOfButtons] = button;
    buttons = newButtonArr;
}*/
