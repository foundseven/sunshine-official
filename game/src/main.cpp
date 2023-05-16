#include "rlImGui.h"
#include "raylib.h"
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720



// main game loop
int main(void)
{

    /////////////////////////////////////init/////////////////////////////////////////////

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");//drawing the screen
    SetTargetFPS(60); //target frame rate
    

    //call on the ball
    Vector2 ballPos = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };


    //background
    Texture2D background = LoadTexture("../game/assets/textures/background.png");
    background.width = 1280;
    background.height = 720;

    Texture2D midground = LoadTexture("../game/assets/textures/buildings.png");
    midground.width = 1280 /2;
    midground.height = 720 /2;

    Texture2D foreground = LoadTexture("../game/assets/textures/foreground.png");
    foreground.width = 1280 /2;
    foreground.height = 720 /2;

    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;


    int screenUpperLimit = 40;      // Top menu limits

    bool pause = false;             // Movement pause

    bool collision = false;         // Collision detection

    InitAudioDevice();              // Initialize audio device
    Music music = LoadMusicStream("../game/assets/audio/citybackground.mp3");
    music.looping = false;
    float pitch = 1.0f;

    PlayMusicStream(music);

    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool p = false;             // Music playing paused

        ///////////////////for collision////////////////////////

    Vector2 boxA = { 900.0f, 380.0f };
    Vector2 boxB = { 100.0f, 100.0f };

    Color boxAColor = GREEN;
    Color boxBColor = DARKGREEN;
   

    while (!WindowShouldClose()) //Detect the window closing or the ESC key
    {


        ///////////////////////////////////update//////////////////////////////////////
       

        //is key pressed
        if (IsKeyDown(KEY_RIGHT)) ballPos.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) ballPos.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) ballPos.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) ballPos.y += 2.0f;


        // for background
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;

        if (scrollingMid <= -midground.width * 2) scrollingMid = 0;
        if (scrollingFore <= -foreground.width * 2) scrollingFore = 0;

        //for audio

        UpdateMusicStream(music);   // Update music buffer with new stream data

        // Restart music playing (stop and play)
        if (IsKeyPressed(KEY_SPACE))
        {
            p = !p;

            if (p) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }

        if (IsKeyDown(KEY_DOWN)) pitch -= 0.01f;
        else if (IsKeyDown(KEY_UP)) pitch += 0.01f;

        SetMusicPitch(music, pitch);

        // Get timePlayed scaled to bar dimensions
        timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music) * (SCREEN_WIDTH - 40);

        ////////////////////////////////////draw///////////////////////////////////////
        BeginDrawing();

        ClearBackground(RAYWHITE);


        //drawing text onto the screen
        DrawText("Would ya look at that!", 16, 9, 7, RED);


        // NOTE: Texture is scaled twice its size
        DrawTextureEx(background, (Vector2 {scrollingBack, 20 }), 0.0f, 2.0f, WHITE);
        DrawTextureEx(background, (Vector2{ background.width*2 + scrollingBack, 20 }), 0.0f, 2.0f, WHITE);

        // Draw midground image twice
        DrawTextureEx(midground, (Vector2 { scrollingMid, 20 }), 0.0f, 2.0f, WHITE);
        DrawTextureEx(midground, (Vector2 { midground.width * 2 + scrollingMid, 20 }), 0.0f, 2.0f, WHITE);

        // Draw foreground image twice
        DrawTextureEx(foreground, (Vector2 { scrollingFore, 70 }), 0.0f, 2.0f, WHITE);
        DrawTextureEx(foreground, (Vector2{ foreground.width * 2 + scrollingFore, 70 }), 0.0f, 2.0f, WHITE);
 
        //for circles
        DrawCircleV(boxA, 50, boxAColor);
        DrawCircleV(boxB, 50, boxBColor);

        // Draw time bar
        DrawRectangle(20, SCREEN_HEIGHT - 20 - 12, SCREEN_WIDTH - 40, 12, LIGHTGRAY);
        DrawRectangle(20, SCREEN_HEIGHT - 20 - 12, (int)timePlayed, 12, DARKGREEN);
        DrawRectangleLines(20, SCREEN_HEIGHT - 20 - 12, SCREEN_WIDTH - 40, 12, GRAY);

        ///////////////////for collision////////////////////////

        boxB = GetMousePosition();

        collision = CheckCollisionCircles(boxA, 50, boxB, 50);

        if (collision)
        {
            boxBColor = BLUE;
            boxAColor = BLUE;
        }
        else
        {
            boxBColor = GREEN;
            boxAColor = DARKGREEN;

        }

        //end draw
        EndDrawing();


    }
    ///////////////////////////// de init ////////////////////////////


    UnloadTexture(background);

    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)


    CloseWindow();
    
}