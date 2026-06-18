/*******************************************************************************************
*
*   raylib [core] example - USB keyboard input (raylib4Wii)
*
*   Plug a USB keyboard into the Wii and start typing. raylib4Wii feeds the USB HID
*   keyboard through the normal raylib input system, so this example is plain, portable
*   raylib code: no libwiikeyboard calls are needed.
*
*       - GetCharPressed()     -> printable characters (already keymap/shift translated)
*       - IsKeyPressed()       -> edge-triggered key presses (BACKSPACE, ENTER, ...)
*       - GetKeyPressed()      -> last KEY_* code pressed this frame
*       - IsKeyDown()          -> held keys / modifiers (SHIFT, CONTROL, ...)
*
*   Press ESC on the keyboard, or HOME on the Wii Remote, to quit.
*
*   Example licensed under an unmodified zlib/libpng license.
*
********************************************************************************************/

#include <raylib.h>
#include <string.h>     // memmove(), strlen()

#define ATTR_WII_WIDTH  640
#define ATTR_WII_HEIGHT 480

#define MAX_INPUT_CHARS 64

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = ATTR_WII_WIDTH;
    const int screenHeight = ATTR_WII_HEIGHT;

    InitWindow(screenWidth, screenHeight, "raylib4Wii [core] example - USB keyboard input");

    char text[MAX_INPUT_CHARS + 1] = "\0";  // NUL-terminated input buffer
    int letterCount = 0;

    int lastKey = KEY_NULL;                 // Last KEY_* code pressed (for display)
    int framesCounter = 0;                  // Used to blink the text cursor

    Rectangle textBox = { screenWidth/2 - 225, 140, 450, 50 };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    bool done = false;

    // Main game loop
    while (!done && !WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        // Quit: ESC on the keyboard or HOME on the Wii Remote
        if (IsKeyPressed(KEY_ESCAPE)) done = true;
        if (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE)) done = true;

        // Drain all printable characters typed since last frame and append them
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
            {
                text[letterCount] = (char)key;
                text[letterCount + 1] = '\0';
                letterCount++;
            }

            key = GetCharPressed();     // Next character in the queue (if any)
        }

        // BACKSPACE removes the last character (auto-repeats while held)
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            if (letterCount > 0)
            {
                letterCount--;
                text[letterCount] = '\0';
            }
        }

        // ENTER clears the line
        if (IsKeyPressed(KEY_ENTER))
        {
            letterCount = 0;
            text[0] = '\0';
        }

        // Remember the last key pressed (for the on-screen read-out)
        int pressed = GetKeyPressed();
        if (pressed != KEY_NULL) lastKey = pressed;

        bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        bool ctrl  = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
        bool alt   = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);

        framesCounter++;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("raylib4Wii - USB KEYBOARD", screenWidth/2 - 190, 30, 28, DARKBLUE);
            DrawText("Plug in a USB keyboard and type:", 40, 100, 20, GRAY);

            // Input box
            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawRectangleLinesEx(textBox, 2, DARKGRAY);
            DrawText(text, (int)textBox.x + 8, (int)textBox.y + 12, 30, MAROON);

            DrawText(TextFormat("%i/%i", letterCount, MAX_INPUT_CHARS),
                     (int)textBox.x, (int)(textBox.y + textBox.height + 6), 20, DARKGRAY);

            // Blinking cursor at the end of the text
            int textW = MeasureText(text, 30);
            if (((framesCounter/20)%2) == 0 && (letterCount < MAX_INPUT_CHARS))
            {
                DrawText("_", (int)textBox.x + 10 + textW, (int)textBox.y + 14, 30, MAROON);
            }

            // Last key + modifier read-out
            DrawText(TextFormat("Last key code: %i", lastKey), 40, 260, 20, DARKGRAY);
            DrawText(TextFormat("SHIFT: %s   CTRL: %s   ALT: %s",
                                shift ? "ON" : "off", ctrl ? "ON" : "off", alt ? "ON" : "off"),
                     40, 290, 20, shift || ctrl || alt ? DARKGREEN : GRAY);

            // Help
            DrawText("BACKSPACE: delete   ENTER: clear line", 40, 380, 20, GRAY);
            DrawText("ESC (keyboard) or HOME (Wii Remote): quit", 40, 410, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
