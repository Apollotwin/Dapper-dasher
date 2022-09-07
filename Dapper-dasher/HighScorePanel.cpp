#include "HighScorePanel.h"

HighScorePanel::HighScorePanel(HighScoreData& high_score_data, int winWidth, int winHeight): data(high_score_data)
{
    panel = {
        static_cast<float>(winWidth) * 0.05f,
        static_cast<float>(winHeight) * 0.3f,
        static_cast<float>(winWidth) * 0.3f,
        static_cast<float>(winHeight) * 0.4f
    };

    panelBGrect = {
        panel.x - 2.f,
        panel.y -2.f,
        panel.width + 4.f,
        panel.height + 4.f
    };

    titlePanel = {
        panel.x, panel.y,
        panel.width,
        panel.height * 0.15f
    };

    columnLeft = {
        panel.x,panel.y + titlePanel.height,
        panel.width/4 * 3,
        panel.height - titlePanel.height
    };

    columnRight = {
        panel.x + columnLeft.width,
        columnLeft.y,
        panel.width/4,
        columnLeft.height
    };

    titleText = "HIGH SCORES";
    name = "AAAAAAAAAAAAAAAAAAW";

    //Set the fontsize depending on title panel width
    while (MeasureText(titleText, titleFontSize) < titlePanel.width - 60.f) titleFontSize++;

    for(int i = 0; i < 10; i++)
    {
        if(i == 0)
        {
            playerInfoPanel[0] = {
                columnLeft.x,
                columnLeft.y,
                panel.width,
                columnLeft.height/10
            };
        }
        else
        {
            playerInfoPanel[i] = {
                columnLeft.x,
                columnLeft.y + columnLeft.height/10 * i,
                panel.width,
                columnLeft.height/10
            };
        }
    }

    GetTop10Players();
}

void HighScorePanel::Render()
{
    DrawRectangleRec(panel,DARKPURPLEISH);
    DrawRectangleLinesEx(titlePanel,1.5f, WHITE);
    DrawRectangleLinesEx(columnLeft, 1.5f, WHITE);
    DrawRectangleLinesEx(columnRight, 1.5f, WHITE);

    for(int i = 0; i < 10; i++)
    {
        DrawRectangleLinesEx(playerInfoPanel[i], 1.5f, WHITE);

        for(int j = 0; j < 10; j++)
        {
            if(data.players[i].name == data.players[j].name &&
            data.players[i].highscore > data.players[j].highscore)
            {
                data.players[j].name = "";
                data.players[j].highscore = 0;
            }
        }

        string index;
        index = std::to_string(i + 1);
        index.append(". ");

        if(!data.players[i].name.empty()) index.append(data.players[i].name);

        name = index.c_str();

        string hscore = std::to_string(data.players[i].highscore);
        highscore = hscore.c_str();
        
        DrawText(name, columnLeft.x + 5.f, playerInfoPanel[i].y + playerInfoPanel[i].height/3, playerInfoFontSize, YELLOW);
        DrawText(highscore, columnRight.x + columnRight.width - MeasureText(highscore,playerInfoFontSize) - 10.f, playerInfoPanel[i].y + playerInfoPanel[i].height/3, playerInfoFontSize, YELLOW);
    }

    DrawText(titleText, titlePanel.x + 30.f, titlePanel.y + titlePanel.height/5, titleFontSize, YELLOW);
}

void HighScorePanel::Update(Player player)
{
    data.players.push_back(player);
    GetTop10Players();
}

void HighScorePanel::GetTop10Players()
{
    SortPlayersByHighscore();
}

void HighScorePanel::SortPlayersByHighscore()
{
    std::sort(data.players.begin(), data.players.end(), [](Player a, Player b)
    {
        return a.highscore > b.highscore;
    });

    while (data.players.capacity() > 10)
    {
        data.players.pop_back();
        data.players.shrink_to_fit();
    }
}
