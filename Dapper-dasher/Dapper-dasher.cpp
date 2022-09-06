#include <fstream>
#include <string>
#include "AnimData.h"
#include "Character.h"
#include "HighScorePanel.h"
#include "InputTextField.h"
#include "Menu.h"
#include "Nebuala.h"
#include "Serializing.h"
#include "raylib.h"


using namespace std;

//Window
const int windowWidth{1350};
const int windowHeight{1100};

//Game bools
bool StartGame{false};
bool PauseGame{false};
bool QuitGame{false};



// acceleration due to gravity (pixels/s)/s
const float gravity{1000.f};

void DrawBackground(const Texture2D farBG, float bgX)
{
    Vector2 farBG1Pos{bgX,0.0};
    DrawTextureEx(farBG, farBG1Pos,0.0,5.0, WHITE);
        
    Vector2 farBG2Pos{bgX + farBG.width * 5,0.0};
    DrawTextureEx(farBG, farBG2Pos,0.0,5.0, WHITE);
        
    Vector2 farBG3Pos{bgX + farBG.width * 10,0.0};
    DrawTextureEx(farBG, farBG3Pos,0.0,5.0, WHITE);
}

void DrawMiddleGround(const int windowHeight, const Texture2D middleBG, float mdgX)
{
    Vector2 mdgPos1{mdgX,windowHeight - middleBG.height * 4};
    DrawTextureEx(middleBG,mdgPos1,0.0,4.0,WHITE);

    Vector2 mdgPos2{mdgX + middleBG.width * 4,windowHeight - middleBG.height * 4};
    DrawTextureEx(middleBG,mdgPos2,0.0,4.0,WHITE);

    Vector2 mdgPos3{mdgX + middleBG.width * 8,windowHeight - middleBG.height * 4};
    DrawTextureEx(middleBG,mdgPos3,0.0,4.0,WHITE);
}

void DrawForeground(const int windowHeight, const Texture2D foreGround, float fgX)
{
    Vector2 fgPos1{fgX, windowHeight - foreGround.height * 2};
    DrawTextureEx(foreGround,fgPos1,0.0, 2.0, WHITE);
        
    Vector2 fgPos2{fgX + foreGround.width * 2, windowHeight - foreGround.height * 2};
    DrawTextureEx(foreGround,fgPos2,0.0, 2.0, WHITE);

    Vector2 fgPos3{fgX + foreGround.width * 4, windowHeight - foreGround.height * 2};
    DrawTextureEx(foreGround,fgPos3,0.0, 2.0, WHITE);

    Vector2 fgPos4{fgX + foreGround.width * 6, windowHeight - foreGround.height * 2};
    DrawTextureEx(foreGround,fgPos4,0.0, 2.0, WHITE);
}

void DrawParallax(const int winHeight,const Texture2D background, float bgx, const Texture2D middleground, float mgx, const Texture2D foreground, float fgx)
{
    DrawBackground(background,bgx);
    DrawMiddleGround(winHeight,middleground,mgx);
    DrawForeground(winHeight,foreground,fgx);
}

void SaveData(HighScoreData& high_score_data, json& json_data)
{
    //Serialize
    to_json(json_data, high_score_data);
    string jsonStr = json_data.dump();
    ofstream file_reader("HighScoresData.txt");
    if(file_reader.is_open()) file_reader << jsonStr;
    //SaveFileText("HighScoresData.json", const_cast<char*>(json_data.dump().c_str()));
}


void LoadData(HighScoreData& high_score_data, json& json_data)
{
    ifstream file_reader("HighScoresData.txt");
    json_data = json::parse(file_reader);
    
    /*const char* loadFile = LoadFileText("HighScoresData.json");
    json_data = json::parse(loadFile);*/
    
    from_json(json_data,high_score_data);
}

int main()
{
    HighScoreData high_score_data;
    json json_data;
    //SaveData(high_score_data,json_data);
    LoadData(high_score_data, json_data);
    
    InitWindow(windowWidth,windowHeight,"Dapper dasher!");

    //Player score
    int PlayerScore{0};
    float timeElapsed{0};

    
    string playerName;
    Player player;
    player.name = { };
    player.highscore = 0;

    

    HighScorePanel highScorePanel{high_score_data,windowWidth,windowHeight};

#pragma region Music_&_Sound
    //Music & Sound
    InitAudioDevice();
    float musicVolume{0.5f};
    float soundVolume{1.f};
    float lastVolumeValue{musicVolume};
    Music music = LoadMusicStream("Sound/Dapper-dasher_music.wav");
    Sound button_click_sound = LoadSound("Sound/Click_button.wav");
    Sound button_hover_sound = LoadSound("Sound/Hover_button.wav");
    PlayMusicStream(music);
    SetMusicVolume(music,musicVolume);
    SetSoundVolume(button_click_sound,soundVolume);
    SetSoundVolume(button_hover_sound,soundVolume);
#pragma endregion Music_&_Sound
    
#pragma region Background textures & variables    //Background
    const Texture2D farBG = LoadTexture("textures/far-buildings.png");
    const Texture2D middleBG = LoadTexture("textures/back-buildings.png");
    const Texture2D foreGround = LoadTexture("textures/foreground.png");
    float bgX{};
    float mdgX{};
    float fgX{};
#pragma endregion 

#pragma region Scarfy

    //Animation data
    AnimData scarfy_run;
    AnimData scarfy_idle;
    AnimData scarfy_dead;

    //Initialize Animation data
    scarfy_run.InitAnimData(windowWidth,windowHeight,LoadTexture("textures/scarfy_run.png"), Center, Running, 6,1);
    scarfy_idle.InitAnimData(windowWidth,windowHeight,LoadTexture("textures/scarfy_idle.png"), Center,Idle, 1,1);
    scarfy_dead.InitAnimData(windowWidth,windowHeight,LoadTexture("textures/scarfy_death.png"), Center,Dead, 6,1);
    
    //Scarfy
    Character scarfy{windowWidth,windowHeight,scarfy_run,scarfy_idle,scarfy_dead,-600 ,gravity };

    //Set initial animation
    scarfy.SetAnimation(scarfy.idle_animation);

#pragma endregion Scarfy

#pragma region Nebula & Finishline
    
    //Animation data
    AnimData nebula_animation;
    nebula_animation.InitAnimData(windowWidth,windowHeight,LoadTexture("textures/12_nebula_spritesheet.png"), Off_Screen_Right,None, 8,8);

    //Sound that plays if scarfy is hit by nebula
    Sound bzzt = LoadSound("Sound/Bzzt.wav");
    SetSoundVolume(bzzt, 1.5f);

    //Nebula
    Nebula nebula{};
    nebula.SetAnimation(nebula_animation);
    
    const int nebulaeAmount = 9999;
    Nebula nebulea[nebulaeAmount]{};

    for (int i = 0; i < nebulaeAmount; i++)
    {
        nebulea[i] = nebula;
        nebulea[i].InitNebula(500,i);
    }

    float finishLine{nebulea[nebulaeAmount - 1].GetPosition().x};

#pragma endregion Nebula

#pragma region UI
    //Input Text field
    InputTextField inputTextField{windowWidth,windowHeight, 2, 7};
    inputTextField.IsEnteringName = true;
    
    //Menu & Buttons
    Menu menu(windowWidth,windowHeight,"Dapper Dasher", false);

    //Start button
    Button startGameButton{"Start Game", windowWidth,windowHeight,200,100,100,DARKPURPLE,WHITE,DARKPURPLE};
    startGameButton.UpdateTextPos(40.f);
    startGameButton.IsActive = true;

    //Quit button
    Button quitGameButton{"Quit", windowWidth,windowHeight,200,100,startGameButton.fontSize,DARKPURPLE,WHITE,DARKPURPLE};
    quitGameButton.rect.y = startGameButton.rect.y + startGameButton.rect.height + 20.f;
    quitGameButton.UpdateTextPos(0.f);
    quitGameButton.IsActive = true;

    //Restart button
    Button restartButton{"Restart Game", windowWidth,windowHeight,200,100,100,DARKPURPLE,WHITE,DARKPURPLE};
    restartButton.UpdateTextPos(40.0f);
    restartButton.IsActive = false;

    //Credits
    string credits = "Music by Simon Magnusson";
    float textLength = static_cast<float> (MeasureText(credits.c_str(), 20));

    //Music icon
    Texture2D musicText;
    Texture2D musicIcon = LoadTexture("textures/Music_icon.png");
    Texture2D musicMuteIcon = LoadTexture("textures/Music_mute_icon.png");
    bool muteMusic{false};
    float iconScale{0.5f};
    float sliderPad{2.f};
    float playHoverSound{true};
    
    musicIcon.width *= iconScale;
    musicIcon.height *= iconScale;
    musicMuteIcon.width *= iconScale;
    musicMuteIcon.height *= iconScale;
    Vector2 musicIconPosWhite {windowWidth - musicIcon.width/2 - 20.f/iconScale, 10.f/iconScale};
    Vector2 musicIconPosPurple {windowWidth - musicIcon.width/2 - 20.f/iconScale + sliderPad, 10.f/iconScale + sliderPad};
    Button musicButton{"", windowWidth,windowHeight, musicIcon.width,musicIcon.height,0, WHITE, WHITE,WHITE};
    musicButton.UpdatePosition(musicIconPosWhite);

    //Music volume slider
    Vector2 slideRailPos{musicIconPosWhite.x - musicIcon.width * 4.f - 10.f,musicIconPosWhite.y + musicIcon.height/2.f };
    Rectangle slideRail{slideRailPos.x, slideRailPos.y,musicIcon.width * 4.f,5.f };
    Vector2 volumeSliderPosWhite{ slideRailPos.x + (slideRail.width * musicVolume), slideRailPos.y + slideRail.height/2.f};
    float circleRad{12.f};
    Button sliderButton{"", windowWidth,windowHeight, static_cast<int>(slideRail.width),static_cast<int>(circleRad),0, WHITE, WHITE,WHITE};
    sliderButton.UpdatePosition(slideRailPos);

# pragma endregion  UI
    
    SetMasterVolume(1.f);
    
    SetExitKey(KEY_ESCAPE);
    
    SetTargetFPS(60);

    bool playMusic{true};
    
    while (!WindowShouldClose() && !QuitGame)
    {
        //Pause music?
        //if(IsKeyPressed(KEY_P)) playMusic = !playMusic;
        
        if(playMusic) UpdateMusicStream(music);
        
        // Delta time (time since the last frame)
        float deltaTime{GetFrameTime()};

        if(StartGame) timeElapsed += deltaTime * 0.001f;
        
        Vector2 mousePos = GetMousePosition();
        
        BeginDrawing();
        ClearBackground(PURPLE);

        if(StartGame && !scarfy.IsDead && scarfy.GetState() != Running) scarfy.SetAnimation(scarfy.run_animation);

        //Update background
        if(StartGame && !scarfy.IsDead)
        {
            //Update background position
            bgX -= 20 * deltaTime;
        
            if(bgX <= -farBG.width * 5)
            {
                bgX = 0.0;
            }

            //Update middleground position
            mdgX -= 40 * deltaTime;

            if(mdgX <= -middleBG.width * 4)
            {
                mdgX = 0.0;
            }

            //Update foreground position
            fgX -= 80 * deltaTime;

            if(fgX <= -foreGround.width * 2)
            {
                fgX = 0.0;
            }
        }

        //Draw Background
        DrawParallax(windowHeight,farBG,bgX,middleBG,mdgX,foreGround,fgX);

        //Draw Credit text
        DrawText(credits.c_str(), windowWidth/2 - textLength/2, 20.f, 20,WHITE );

        //Draw Title
        menu.DrawTitle(!scarfy.IsDead);
        
        //Draw Input field & Check get input
        inputTextField.Update(deltaTime);
        if(menu.isActive || restartButton.IsActive) highScorePanel.Render();

        if(StartGame && scarfy.IsDead)
        {
            string scoreText = "Score: ";
            scoreText.append(std::to_string(PlayerScore));
            int scoreTextWidth = MeasureText(scoreText.c_str(), (int)restartButton.rect.width * 0.3f);
            DrawText(scoreText.c_str(), restartButton.rect.x + restartButton.rect.width/2 - scoreTextWidth/2 - 2.f, restartButton.rect.y - restartButton.rect.height - 2.f, (int)restartButton.rect.width * 0.3f, GRAY);
            DrawText(scoreText.c_str(), restartButton.rect.x + restartButton.rect.width/2 - scoreTextWidth/2, restartButton.rect.y - restartButton.rect.height , (int)restartButton.rect.width * 0.3f, YELLOW);
        }

        //Character Update
        scarfy.Tick(deltaTime);

        //Draw player score
        if(!inputTextField.IsEnteringName && !scarfy.IsDead && !menu.isActive)
        {
            int textWidth = MeasureText(std::to_string(PlayerScore).c_str(), 300);
            DrawText(std::to_string(PlayerScore).c_str(), windowWidth/2 - textWidth/2 - 3.f, windowHeight/3 - 3.f, 250, GRAY);
            DrawText(std::to_string(PlayerScore).c_str(), windowWidth/2 - textWidth/2, windowHeight/3, 250, YELLOW);
        }

        if(PlayerScore > player.highscore)
        {
            player.highscore = PlayerScore;
            highScorePanel.Update(player);
            SaveData(high_score_data, json_data);
        }

        //Increase nebula speed over time
        if(StartGame)
        {
            for (auto &nebula : nebulea)
            {
                nebula.SetVelocity(timeElapsed);
            }
        }

#pragma region Buttons & sliders
        //Clamp Music volume
        if(musicVolume > 1.f) musicVolume = 1.f;
        if(musicVolume < 0.f) musicVolume = 0.f;

        //Clamp Sound Volume
        if(soundVolume > 1.f) soundVolume = 1.f;
        if(soundVolume < 0.f) soundVolume = 0.f;
        
        //Check if slider is pressed & Update slider position, music volume & sound volume
        if(sliderButton.Down(mousePos) && !sliderButton.Released())
        {
            if(mousePos.x < slideRailPos.x)
            {
                volumeSliderPosWhite.x = slideRailPos.x;
                
            }
            else if(mousePos.x > slideRailPos.x + slideRail.width)
            {
                volumeSliderPosWhite.x = slideRailPos.x+ slideRail.width;
            }
            else
            {
                volumeSliderPosWhite.x = mousePos.x;
            }
            
            musicVolume = (volumeSliderPosWhite.x - slideRailPos.x )/ slideRail.width;
            soundVolume = (volumeSliderPosWhite.x - slideRailPos.x )/ slideRail.width;
            lastVolumeValue = musicVolume;
            SetMusicVolume(music,musicVolume);
            SetSoundVolume(button_click_sound,soundVolume);
            SetSoundVolume(button_hover_sound,soundVolume);
        }

        //Check if mute button is pressed & mute music if pressed
        if(musicButton.MouseOverButton(mousePos) && musicButton.Clicked(mousePos))
        {
            PlaySound(button_click_sound);
            muteMusic = !muteMusic;
            musicVolume = lastVolumeValue;
        }

        //Check if music should be muted
        if(muteMusic || musicVolume <= 0.f)
        {
            musicVolume = 0.f;
            SetMusicVolume(music, musicVolume);
            musicText = musicMuteIcon;
        }else
        {
            SetMusicVolume(music, musicVolume);
            musicText = musicIcon;
        }

        //Animate slider button
        if(sliderButton.MouseOverButton(mousePos) || sliderButton.Down(mousePos)) // Change background color if mouse is over
        {
            DrawRectangle(slideRailPos.x,slideRailPos.y,slideRail.width,slideRail.height, WHITE);
            DrawCircle(volumeSliderPosWhite.x,volumeSliderPosWhite.y, circleRad, WHITE);
            DrawRectangle(slideRailPos.x + sliderPad,slideRailPos.y + sliderPad,slideRail.width,slideRail.height, DARKPURPLE);
            DrawCircle(volumeSliderPosWhite.x + sliderPad/2.f,volumeSliderPosWhite.y + sliderPad/2.f, circleRad, DARKPURPLE);
        }
        else
        {
            DrawRectangle(slideRailPos.x,slideRailPos.y,slideRail.width,slideRail.height, DARKPURPLE);
            DrawCircle(volumeSliderPosWhite.x,volumeSliderPosWhite.y, circleRad, DARKPURPLE);
            DrawRectangle(slideRailPos.x + sliderPad,slideRailPos.y + sliderPad,slideRail.width,slideRail.height, WHITE);
            DrawCircle(volumeSliderPosWhite.x + sliderPad/2.f,volumeSliderPosWhite.y + sliderPad/2.f, circleRad, WHITE);
        }

        //Animate music button
        if(musicButton.MouseOverButton(mousePos)) // Change background color if mouse is over
        {
            DrawTexture(musicText,musicIconPosWhite.x,musicIconPosWhite.y, WHITE);
            DrawTexture(musicText,musicIconPosPurple.x,musicIconPosPurple.y, DARKPURPLE);
        }
        else
        {
            DrawTexture(musicText,musicIconPosWhite.x,musicIconPosWhite.y, DARKPURPLE);
            DrawTexture(musicText,musicIconPosPurple.x,musicIconPosPurple.y, WHITE);
        }

#pragma endregion 

        //Show menu if we have not pushed start game
        if(!inputTextField.IsEnteringName) menu.Show(!StartGame,startGameButton, quitGameButton);

        //Check if we click start game button
        if(!inputTextField.IsEnteringName && startGameButton.MouseOverButton(mousePos) && startGameButton.Clicked(mousePos) && menu.isActive)
        {
            PlaySound(button_click_sound);
            menu.isActive = false;
            StartGame = true;
        }

        //Check if we click quit button
        if(!inputTextField.IsEnteringName && quitGameButton.MouseOverButton(mousePos) && quitGameButton.Clicked(mousePos) && quitGameButton.IsActive)
        {
            QuitGame = true;
            menu.isActive = false;
            WindowShouldClose();
        }

        //Restart game button
        if(!inputTextField.IsEnteringName && restartButton.MouseOverButton(mousePos) && restartButton.Clicked(mousePos) && restartButton.IsActive || restartButton.IsActive && IsKeyPressed(KEY_ENTER) && scarfy.IsDead)
        {
            scarfy.IsDead = false;
            StartGame = false;
            restartButton.IsActive = false;

            for (auto &neb1 : nebulea)
            {
                neb1.ResetToInitPos();
            }

            finishLine = nebulea[nebulaeAmount - 1].GetPosition().x;
            PlayerScore = 0;
        }

        //Collision check
        if(!inputTextField.IsEnteringName && !scarfy.IsDead && StartGame)
        {
            //for (auto &neb : nebulea)
            for (int i = 0; i < nebulaeAmount; i++)
            {
                nebulea[i].Tick(deltaTime);

                if(CheckCollisionRecs( nebulea[i].GetCollisionRect(),scarfy.GetCollisionRect()))
                {
                    PlaySound(bzzt);
                    PlaySound(scarfy.deathSound);
                    scarfy.IsDead = true;
                    
                }

                if( nebulea[i].GetPosition().x < scarfy.GetPosition().x)
                {
                    if(i > PlayerScore) PlayerScore++;
                }
            }
        }

        //Show Game over if scarfy is dead
        if(scarfy.IsDead)
        {
            if(!menu.isActive)
            {
                DrawText("GAME OVER", windowWidth/2 - MeasureText("GAME OVER",menu.fontSize)/2,windowHeight/8,menu.fontSize, WHITE);
                DrawText("GAME OVER", windowWidth/2 - MeasureText("GAME OVER",menu.fontSize)/2 + 2,windowHeight/8 + 2,menu.fontSize, DARKPURPLE);
                startGameButton.IsActive = false;
                restartButton.IsActive = true;
                menu.DrawButton(restartButton);
                menu.DrawButton(quitGameButton);
            }
        }
        else if(menu.isActive) //Set scarfy to idle if we are in startmenu
        {
            scarfy.SetAnimation(scarfy.idle_animation);
        }
        else //Count down finishline
        {
            if(!inputTextField.IsEnteringName && StartGame) finishLine += static_cast<float>(nebula.GetVelocity()) * deltaTime;
            
            if(finishLine < scarfy.GetPosition().x)
            {
                DrawText("YOU WIN!", windowWidth/2 - MeasureText("YOU WIN!",menu.fontSize)/2,windowHeight/8,menu.fontSize, WHITE);
                DrawText("YOU WIN!", windowWidth/2 - MeasureText("YOU WIN!",menu.fontSize)/2 + 2,windowHeight/8 + 2,menu.fontSize, DARKPURPLE);
                startGameButton.IsActive = false;
                restartButton.IsActive = true;
                menu.DrawButton(restartButton);
                menu.DrawButton(quitGameButton);
            }
        }

        if(!inputTextField.IsActive && IsKeyPressed(KEY_ENTER) && !scarfy.IsDead)
        {
            menu.isActive = !menu.isActive;
            StartGame = !StartGame;
        }

        if(inputTextField.IsActive && IsKeyPressed(KEY_ENTER) && playerName.length() <= 0)
        {
            player.name = inputTextField.GetInput();
            player.highscore = 0;
        }

        
        EndDrawing();
    }

    
    UnloadMusicStream(music);
    UnloadSound(button_click_sound);
    UnloadSound(button_hover_sound);
    scarfy.Unload();
    nebula.Unload();
    UnloadTexture(farBG);
    UnloadTexture(middleBG);
    UnloadTexture(foreGround);

    CloseAudioDevice();
    CloseWindow();
}
