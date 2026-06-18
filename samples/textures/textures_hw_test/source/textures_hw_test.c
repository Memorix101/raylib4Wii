/*******************************************************************************************
 *
 *   raylib4Wii [textures] example - hardware test: DrawTexturePro + GetRandomValue
 *
 *   Why this exists:
 *       Two things have historically broken on real PowerPC hardware (big-endian) while
 *       looking fine in emulators (Dolphin):
 *         1. Texture drawing - especially DrawTexturePro() with a source rectangle,
 *            rotation and scaling (UV / byte-order bugs in the GL_1.1 / opengx path).
 *         2. GetRandomValue() - returning out-of-range, constant, or biased values.
 *
 *       This sample exercises both so the result is obvious on a TV, not just in an emulator.
 *
 *   Texture source:
 *       By default it loads "sd:/res/textures/parrots.png".
 *
 *   Controls (Wii Remote / GameCube pad):
 *       A - re-roll the random numbers
 *       HOME / START- quit
 *
 *   Example licensed under an unmodified zlib/libpng license.
 *
 ********************************************************************************************/

#include <raylib.h>
#include <stdio.h> // snprintf()

#define ATTR_WII_WIDTH 640
#define ATTR_WII_HEIGHT 480

#define RANDOM_SAMPLES 256 // Number of random values gathered per roll
#define RANDOM_MIN 0
#define RANDOM_MAX 1000

// Gather a fresh batch of random values and compute observed min/max/mean.
// A correct RNG should fill [RANDOM_MIN, RANDOM_MAX] and never leave it.
static void RollRandom(int *values, int count, int *outMin, int *outMax, double *outMean)
{
    int lo = RANDOM_MAX + 1; // start inverted so the first sample sets the bounds
    int hi = RANDOM_MIN - 1;
    long long sum = 0;

    for (int i = 0; i < count; i++)
    {
        int v = GetRandomValue(RANDOM_MIN, RANDOM_MAX);
        values[i] = v;
        if (v < lo)
            lo = v;
        if (v > hi)
            hi = v;
        sum += v;
    }

    *outMin = lo;
    *outMax = hi;
    *outMean = (double)sum / (double)count;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = ATTR_WII_WIDTH;
    const int screenHeight = ATTR_WII_HEIGHT;

    InitWindow(screenWidth, screenHeight, "raylib4Wii [textures] hw test - DrawTexturePro + RNG");

    Image img = LoadImage("sd:/res/textures/parrots.png");

    int imgW = img.width;
    int imgH = img.height;

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    // ---- Random number test state ----
    int randomValues[RANDOM_SAMPLES] = {0};
    int randMin = 0, randMax = 0;
    double randMean = 0.0;

    SetRandomSeed(0xC0FFEE); // deterministic start (reproducible on hardware)
    RollRandom(randomValues, RANDOM_SAMPLES, &randMin, &randMax, &randMean);

    float rotation = 0.0f; // animated rotation for DrawTexturePro
    bool done = false;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!done && !WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        rotation += 1.0f;
        if (rotation >= 360.0f)
            rotation -= 360.0f;

        if (IsGamepadAvailable(0))
        {
            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) // A
                RollRandom(randomValues, RANDOM_SAMPLES, &randMin, &randMax, &randMean);

            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE) ||     // HOME
                IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) // PLUS / START
                done = true;
        }
        if (IsKeyPressed(KEY_SPACE))
            RollRandom(randomValues, RANDOM_SAMPLES, &randMin, &randMax, &randMean);
        if (IsKeyPressed(KEY_ESCAPE))
            done = true;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("TEXTURE + RNG HW TEST", 16, 10, 22, DARKBLUE);
        DrawText("texture: sd:/res/textures/parrots.png", 16, 36, 14, DARKGREEN);
        DrawText(TextFormat("image %dx%d", imgW, imgH),16, 52, 12, DARKGREEN);

        // 1) Plain DrawTexture (baseline)
        int baseY = 86;
        DrawText("1) DrawTexture", 16, baseY - 14, 12, GRAY);
        DrawTexture(tex, 16, baseY, WHITE);

        // 2) DrawTextureRec: draw only the top-left quarter (source rect = original size)
        DrawText("2) DrawTextureRec (1/4)", 170, baseY - 14, 12, GRAY);
        Rectangle quarter = {0, 0, (float)imgW / 2.0f, (float)imgH / 2.0f};
        DrawTextureRec(tex, quarter, (Vector2){170, baseY}, WHITE);

        // DrawTextureEx: scaled + rotated about top-left ---
        DrawText("3) DrawTextureEx", 320, baseY - 18, 12, GRAY);
        DrawTextureEx(tex, (Vector2){380, baseY + 40}, rotation, 0.6f, WHITE);

        // DrawTexturePro: source rect + dest rect + centered origin + rotation ---
        // THIS is the one reported as broken on real hardware.
        DrawText("4) DrawTexturePro", 470, baseY - 18, 12, MAROON);
        Rectangle src = {0, 0, (float)imgW, (float)imgH};
        Rectangle dst = {540, baseY + 50, 90, 90};
        Vector2 origin = {45, 45}; // rotate around the center of dst
        DrawTexturePro(tex, src, dst, origin, rotation, WHITE);
        DrawRectangleLines((int)(dst.x - origin.x), (int)(dst.y - origin.y),
                           (int)dst.width, (int)dst.height, Fade(MAROON, 0.5f));

        // --- Random number test: histogram + stats ---
        int histX = 16, histY = 230, histW = 608, histH = 120;
        DrawRectangleLines(histX, histY, histW, histH, LIGHTGRAY);
        DrawText("GetRandomValue() distribution", histX, histY - 18, 14, DARKBLUE);

        // Each sample drawn as a vertical bar; a healthy RNG fills the box evenly.
        for (int i = 0; i < RANDOM_SAMPLES; i++)
        {
            int bx = histX + (int)((long)i * histW / RANDOM_SAMPLES);
            int bh = (int)((long)randomValues[i] * histH / RANDOM_MAX);
            DrawLine(bx, histY + histH, bx, histY + histH - bh, DARKGREEN);
        }

        // Stats read-out: on broken RNG min==max, or values escape [MIN,MAX].
        char stats[160];
        bool inRange = (randMin >= RANDOM_MIN) && (randMax <= RANDOM_MAX) && (randMin != randMax);
        snprintf(stats, sizeof(stats),
                 "range [%d..%d]  observed min=%d max=%d mean=%.1f (expected ~%.0f)  -> %s",
                 RANDOM_MIN, RANDOM_MAX, randMin, randMax, randMean,
                 (RANDOM_MIN + RANDOM_MAX) / 2.0, inRange ? "OK" : "SUSPECT");
        DrawText(stats, histX, histY + histH + 8, 12, inRange ? DARKGREEN : RED);

        DrawText("A / SPACE: re-roll random    HOME / START / ESC: quit", 16, 430, 14, GRAY);
        DrawFPS(560, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(tex);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
