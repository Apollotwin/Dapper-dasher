#include <string>
#include "Menu.h"
#include "raylib.h"

using namespace std;

//Window
const int windowWidth{1350};
const int windowHeight{1100};

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    int maxFrame;
    float updateTime;
    float runningTime;
};

bool IsOnGround(AnimData data, int windowHeight)
{
    return  data.pos.y >= windowHeight - data.rect.height;
}

AnimData UpdateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //Update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;
            
        //Update animation frame
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if(data.frame > maxFrame) data.frame = 0;
    }
    return data;
}

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

void DebugLog(const char* text) 
{
    Vector2 txtSize = MeasureTextEx(GetFontDefault(), text,50.f,0.f);
    DrawText(text, windowWidth/4 - txtSize.x/2,windowHeight/8 * 7, 50.f,WHITE );
}

void InitNebulae(const Texture2D& nebula,const int nebulaeAmount, AnimData(nebulea)[15])
{
    for (int i = 0; i < nebulaeAmount; i++)
    {
        nebulea[i].rect.x = 0.0;
        nebulea[i].rect.y = 0.0;
        nebulea[i].rect.width = nebula.width/8;
        nebulea[i].rect.height = nebula.height/8;
        nebulea[i].pos.y = windowHeight - nebula.height/8;
        nebulea[i].pos.x = windowWidth + (300 * i);
        nebulea[i].frame = i;
        nebulea[i].maxFrame = 7;
        nebulea[i].runningTime = 1.0/12.0;
        nebulea[i].updateTime = 0.0;
    }
}

int main()
{
    InitWindow(windowWidth,windowHeight,"Dapper dasher!");

    //Background
    const Texture2D farBG = LoadTexture("textures/far-buildings.png");
    const Texture2D middleBG = LoadTexture("textures/back-buildings.png");
    const Texture2D foreGround = LoadTexture("textures/foreground.png");
    float bgX{};
    float mdgX{};
    float fgX{};
    
    //Nebula
    const Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    int nebulaeAmount = 15;
    int nebVel{-200};
    AnimData nebulea[15];
    InitNebulae(nebula, nebulaeAmount, nebulea);
    float finishLine{nebulea[nebulaeAmount - 1].pos.x};
    
    //Scarfy 
    const Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rect.width = scarfy.width/6;
    scarfyData.rect.height = scarfy.width/6,scarfy.height;
    scarfyData.rect.x = 0;
    scarfyData.rect.y = 0;
    scarfyData.pos.x = windowWidth/2 - (scarfy.width/6)/2;
    scarfyData.pos.y = windowHeight - scarfy.height;
    scarfyData.frame = 0;
    scarfyData.maxFrame = 5;
    scarfyData.updateTime = 1.f/12.f;
    scarfyData.runningTime = 0.f;
    
    // acceleration due to gravity (pixels/s)/s
    const int gravity{1000};
    
    //pixels/s
    const int jumpVel{-600};
    int velocity{0};
    
    bool isInAir{false};
    bool isDead{false};
    bool startGame{false};
    bool quitGame{false};

    //Menu & Buttons
    Menu menu(windowWidth,windowHeight,"Dapper Dasher", true);

    //Start button
    Button startGameButton{"Start Game", windowWidth,windowHeight,200,100,100,DARKPURPLE,WHITE,DARKPURPLE};
    startGameButton.UpdateTextPos(40.f);

    //Quit button
    Button quitGameButton{"Quit", windowWidth,windowHeight,200,100,startGameButton.fontSize,DARKPURPLE,WHITE,DARKPURPLE};
    quitGameButton.rect.y = startGameButton.rect.y + startGameButton.rect.height + 20.f;
    quitGameButton.UpdateTextPos(0.f);

    //Restart button
    Button restartButton{"Restart Game", windowWidth,windowHeight,200,100,100,DARKPURPLE,WHITE,DARKPURPLE};
    restartButton.UpdateTextPos(40.0f);

    //Credits
    string credits = "Music by Simon Magnusson";
    float textLength = static_cast<float> (MeasureText(credits.c_str(), 20));
    
    //Music & Sound
    InitAudioDevice();
    float musicVolume{0.7f};
    float soundVolume{1.f};
    float lastVolumeValue{musicVolume};
    Music music = LoadMusicStream("Sound/Dapper-dasher_music.wav");
    Sound button_click_sound = LoadSound("Sound/Click_button.wav");
    Sound button_hover_sound = LoadSound("Sound/Hover_button.wav");
    PlayMusicStream(music);
    SetMusicVolume(music,musicVolume);
    SetSoundVolume(button_click_sound,soundVolume);
    SetSoundVolume(button_hover_sound,soundVolume);

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
    
    SetMasterVolume(1.f);
    
    SetExitKey(KEY_ESCAPE);
    
    SetTargetFPS(60);

    bool playMusic{false};
    
    while (!WindowShouldClose() && !quitGame)
    {
        if(IsKeyPressed(KEY_A)) playMusic = true;
        
        if(playMusic) UpdateMusicStream(music);
        // Delta time (time since the last frame)
        const float deltaTime{GetFrameTime()};
        Vector2 mousePos = GetMousePosition();
        
        BeginDrawing();
        ClearBackground(PURPLE);

        if(!isDead)
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

        //Draw far-background
        DrawBackground(farBG, bgX);

        //Draw middle ground
        DrawMiddleGround(windowHeight, middleBG, mdgX);

        //Draw foreground
        DrawForeground(windowHeight, foreGround, fgX);

        //Draw Credit text
        DrawText(credits.c_str(), windowWidth/2 - textLength/2, 20.f, 20,WHITE );
        

        //Draw Music icons & buttons
        if(musicVolume > 1.f) musicVolume = 1.f;
        if(musicVolume < 0.f) musicVolume = 0.f;

        if(soundVolume > 1.f) soundVolume = 1.f;
        if(soundVolume < 0.f) soundVolume = 0.f;
        

        if(sliderButton.Down(mousePos))
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
        
        if(musicButton.MouseOverButton(mousePos) && musicButton.Clicked(mousePos))
        {
            PlaySound(button_click_sound);
            muteMusic = !muteMusic;
            musicVolume = lastVolumeValue;
        }

        if(muteMusic || musicVolume <= 0.f)
        {
            musicVolume = 0.f;
            SetMusicVolume(music, musicVolume);
            musicText = musicMuteIcon;
        }
        else
        {
            SetMusicVolume(music, musicVolume);
            musicText = musicIcon;
        }

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

        menu.Show(!startGame,startGameButton, quitGameButton);
        
        if(startGameButton.MouseOverButton(mousePos) && startGameButton.Clicked(mousePos))
        {
            PlaySound(button_click_sound);
            startGame = true;
            InitNebulae(nebula, nebulaeAmount, nebulea);
            finishLine = nebulea[nebulaeAmount - 1].pos.x;
        }

        if(quitGameButton.MouseOverButton(mousePos) && quitGameButton.Clicked(mousePos))
        {
            quitGame = true;
            WindowShouldClose();
        }
        
        if(restartButton.MouseOverButton(mousePos) && restartButton.Clicked(mousePos))
        {
            isDead = false;
            InitNebulae(nebula, nebulaeAmount, nebulea);
            finishLine = nebulea[nebulaeAmount - 1].pos.x;
        }
        
        //Ground check
        if(IsOnGround(scarfyData,windowHeight))
        {   //Rectangle is on ground
            isInAir = false;
            velocity = 0;
        }
        else
        {   // Rectangle is in air
            isInAir = true;
            velocity += gravity * deltaTime;
        }

        // Check for jump
        if(IsKeyPressed(KEY_SPACE) && !isInAir) velocity += jumpVel;
        
        //Update scarfy position 
        scarfyData.pos.y += velocity * deltaTime;

        if(!isInAir)
        {
           scarfyData = UpdateAnimData(scarfyData,deltaTime,scarfyData.maxFrame);
        }
        
        for (AnimData nebula : nebulea)
        {
            const float pad{50};
            const Rectangle nebRect{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rect.width - 2*pad,
                nebula.rect.height - 2*pad
            };

            const Rectangle scarfyRect{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rect.width,
                scarfyData.rect.height
            };
            
            if(CheckCollisionRecs(nebRect, scarfyRect))
            {
                isDead = true;
            }
        }

        if(isDead)
        {
            if(!menu.isActive)
            {
                DrawText("GAME OVER", windowWidth/2 - MeasureText("GAME OVER",menu.fontSize)/2,windowHeight/8,menu.fontSize, WHITE);
                DrawText("GAME OVER", windowWidth/2 - MeasureText("GAME OVER",menu.fontSize)/2 + 2,windowHeight/8 + 2,menu.fontSize, DARKPURPLE);
                menu.DrawButton(restartButton);
                menu.DrawButton(quitGameButton);
            }
        }
        else if(menu.isActive)
        {
            DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
        }
        else
        {
            for (int i = 0; i < nebulaeAmount; i++)
            {
                //Update nebula position
                nebulea[i].pos.x += nebVel * deltaTime;

                nebulea[i] = UpdateAnimData(nebulea[i],deltaTime,nebulea[i].maxFrame);
        
                DrawTextureRec(nebula,nebulea[i].rect,nebulea[i].pos,WHITE);
            }

            finishLine += nebVel * deltaTime;
            if(finishLine < scarfyData.pos.x)
            {
                DrawText("YOU WIN!", windowWidth/2 - MeasureText("YOU WIN!",menu.fontSize)/2,windowHeight/8,menu.fontSize, WHITE);
                DrawText("YOU WIN!", windowWidth/2 - MeasureText("YOU WIN!",menu.fontSize)/2 + 2,windowHeight/8 + 2,menu.fontSize, DARKPURPLE);
                menu.DrawButton(restartButton);
                menu.DrawButton(quitGameButton);
            }

            DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
        }

        if(IsKeyPressed(KEY_ENTER))
        {
            menu.isActive = !menu.isActive;
            startGame = !startGame;
        }
        
        EndDrawing();
    }
    
    UnloadMusicStream(music);
    UnloadSound(button_click_sound);
    UnloadSound(button_hover_sound);
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(farBG);
    UnloadTexture(middleBG);
    UnloadTexture(foreGround);

    CloseAudioDevice();
    CloseWindow();
}
