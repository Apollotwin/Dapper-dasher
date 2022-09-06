#pragma once
#include <raylib.h>

#include "Serializing.h"

class HighScorePanel
{
public:
    HighScorePanel(HighScoreData& high_score_data, int winWidth, int winHeight);
    void Render();
    void Update(Player player);
    void GetTop10Players();
    void SortPlayersByHighscore();
    
private:
    HighScoreData& data;
    vector<Player> Top10Players;
    Rectangle panel{};
    Rectangle panelBGrect{};
    Rectangle titlePanel{};
    Rectangle playerInfoPanel[10]{};
    Rectangle columnLeft{};
    Rectangle columnRight{};
    int titleFontSize{0};
    int playerInfoFontSize{25};
    const char* titleText{};
    const char* name{};
    const char* highscore{};
};
