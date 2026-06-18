/**********************************************************************************************
*
*   rcore_wii - Functions to manage window, graphics device and inputs
*
*   PLATFORM: WII
*       - Nintendo Wii
*
*   LIMITATIONS:
*       - Limitation 01
*       - Limitation 02
*
*   POSSIBLE IMPROVEMENTS:
*       - Improvement 01
*       - Improvement 02
*
*   ADDITIONAL NOTES:
*       - Port done by memorix101	
*
*   CONFIGURATION:
*       #define RCORE_PLATFORM_CUSTOM_FLAG
*           Custom flag for rcore on target platform -not used-
*
*   DEPENDENCIES:
*       WII libogc2 devkitpro and toolchains - Provides C API to access Wii homebrew functionality
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5) and contributors
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include <gccore.h>            // libogc2
#include <opengx.h>            // OpenGX
#include <wiiuse/wpad.h>       // Wii Remote (WPAD) input
#include <wiikeyboard/keyboard.h>  // USB keyboard input
#include <fat.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
static void *xfb[2] = { NULL, NULL };
static int which_fb = 0;
static GXRModeObj *rmode = NULL;
static void *gp_fifo = NULL;
#define DEFAULT_FIFO_SIZE (256*1024)

typedef struct {
    // TODO: Define the platform specific variables required
    int version;
    // Display data
    //EGLDisplay device;                  // Native display device (physical screen connection)
    //EGLSurface surface;                 // Surface to draw on, framebuffers (connected to context)
    //EGLContext context;                 // Graphic context, mode in which drawing can be done
    //EGLConfig config;                   // Graphic config
} PlatformData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern CoreData CORE;                   // Global CORE state context

static PlatformData platform = { 0 };   // Platform specific data

//----------------------------------------------------------------------------------
// Module Internal Functions Declaration
//----------------------------------------------------------------------------------
int InitPlatform(void);          // Initialize platform (graphics, inputs and more)
bool InitGraphicsDevice(void);   // Initialize graphics device

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
// NOTE: Functions declaration is provided by raylib.h

//----------------------------------------------------------------------------------
// Module Functions Definition: Window and Graphics Device
//----------------------------------------------------------------------------------

// Check if application should close
bool WindowShouldClose(void)
{
    if (CORE.Window.ready) return CORE.Window.shouldClose;
    else return true;
}

// Toggle fullscreen mode
void ToggleFullscreen(void)
{
    TRACELOG(LOG_WARNING, "ToggleFullscreen() not available on target platform");
}

// Toggle borderless windowed mode
void ToggleBorderlessWindowed(void)
{
    TRACELOG(LOG_WARNING, "ToggleBorderlessWindowed() not available on target platform");
}

// Set window state: maximized, if resizable
void MaximizeWindow(void)
{
    TRACELOG(LOG_WARNING, "MaximizeWindow() not available on target platform");
}

// Set window state: minimized
void MinimizeWindow(void)
{
    TRACELOG(LOG_WARNING, "MinimizeWindow() not available on target platform");
}

// Set window state: not minimized/maximized
void RestoreWindow(void)
{
    TRACELOG(LOG_WARNING, "RestoreWindow() not available on target platform");
}

// Set window configuration state using flags
void SetWindowState(unsigned int flags)
{
    TRACELOG(LOG_WARNING, "SetWindowState() not available on target platform");
}

// Clear window configuration state flags
void ClearWindowState(unsigned int flags)
{
    TRACELOG(LOG_WARNING, "ClearWindowState() not available on target platform");
}

// Set icon for window
void SetWindowIcon(Image image)
{
    TRACELOG(LOG_WARNING, "SetWindowIcon() not available on target platform");
}

// Set icon for window
void SetWindowIcons(Image *images, int count)
{
    TRACELOG(LOG_WARNING, "SetWindowIcons() not available on target platform");
}

// Set title for window
void SetWindowTitle(const char *title)
{
    CORE.Window.title = title;
}

// Set window position on screen (windowed mode)
void SetWindowPosition(int x, int y)
{
    TRACELOG(LOG_WARNING, "SetWindowPosition() not available on target platform");
}

// Set monitor for the current window
void SetWindowMonitor(int monitor)
{
    TRACELOG(LOG_WARNING, "SetWindowMonitor() not available on target platform");
}

// Set window minimum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMinSize(int width, int height)
{
    CORE.Window.screenMin.width = width;
    CORE.Window.screenMin.height = height;
}

// Set window maximum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMaxSize(int width, int height)
{
    CORE.Window.screenMax.width = width;
    CORE.Window.screenMax.height = height;
}

// Set window dimensions
void SetWindowSize(int width, int height)
{
    TRACELOG(LOG_WARNING, "SetWindowSize() not available on target platform");
}

// Set window opacity, value opacity is between 0.0 and 1.0
void SetWindowOpacity(float opacity)
{
    TRACELOG(LOG_WARNING, "SetWindowOpacity() not available on target platform");
}

// Set window focused
void SetWindowFocused(void)
{
    TRACELOG(LOG_WARNING, "SetWindowFocused() not available on target platform");
}

// Get native window handle
void *GetWindowHandle(void)
{
    TRACELOG(LOG_WARNING, "GetWindowHandle() not implemented on target platform");
    return NULL;
}

// Get number of monitors
int GetMonitorCount(void)
{
    TRACELOG(LOG_WARNING, "GetMonitorCount() not implemented on target platform");
    return 1;
}

// Get number of monitors
int GetCurrentMonitor(void)
{
    TRACELOG(LOG_WARNING, "GetCurrentMonitor() not implemented on target platform");
    return 0;
}

// Get selected monitor position
Vector2 GetMonitorPosition(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorPosition() not implemented on target platform");
    return (Vector2){ 0, 0 };
}

// Get selected monitor width (currently used by monitor)
int GetMonitorWidth(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorWidth() not implemented on target platform");
    return 0;
}

// Get selected monitor height (currently used by monitor)
int GetMonitorHeight(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorHeight() not implemented on target platform");
    return 0;
}

// Get selected monitor physical width in millimetres
int GetMonitorPhysicalWidth(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorPhysicalWidth() not implemented on target platform");
    return 0;
}

// Get selected monitor physical height in millimetres
int GetMonitorPhysicalHeight(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorPhysicalHeight() not implemented on target platform");
    return 0;
}

// Get selected monitor refresh rate
int GetMonitorRefreshRate(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorRefreshRate() not implemented on target platform");
    return 0;
}

// Get the human-readable, UTF-8 encoded name of the selected monitor
const char *GetMonitorName(int monitor)
{
    TRACELOG(LOG_WARNING, "GetMonitorName() not implemented on target platform");
    return "";
}

// Get window position XY on monitor
Vector2 GetWindowPosition(void)
{
    TRACELOG(LOG_WARNING, "GetWindowPosition() not implemented on target platform");
    return (Vector2){ 0, 0 };
}

// Get window scale DPI factor for current monitor
Vector2 GetWindowScaleDPI(void)
{
    TRACELOG(LOG_WARNING, "GetWindowScaleDPI() not implemented on target platform");
    return (Vector2){ 1.0f, 1.0f };
}

// Set clipboard text content
void SetClipboardText(const char *text)
{
    TRACELOG(LOG_WARNING, "SetClipboardText() not implemented on target platform");
}

// Get clipboard text content
// NOTE: returned string is allocated and freed by GLFW
const char *GetClipboardText(void)
{
    TRACELOG(LOG_WARNING, "GetClipboardText() not implemented on target platform");
    return NULL;
}

// Show mouse cursor
void ShowCursor(void)
{
    CORE.Input.Mouse.cursorHidden = false;
}

// Hides mouse cursor
void HideCursor(void)
{
    CORE.Input.Mouse.cursorHidden = true;
}

// Enables cursor (unlock cursor)
void EnableCursor(void)
{
    // Set cursor position in the middle
    SetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);

    CORE.Input.Mouse.cursorHidden = false;
}

// Disables cursor (lock cursor)
void DisableCursor(void)
{
    // Set cursor position in the middle
    SetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);

    CORE.Input.Mouse.cursorHidden = true;
}

// Swap back buffer with front buffer (screen drawing)
void SwapScreenBuffer(void)
{
    GX_CopyDisp(xfb[which_fb], GX_TRUE);
    GX_DrawDone();

    VIDEO_SetNextFramebuffer(xfb[which_fb]);
    VIDEO_Flush();
    VIDEO_WaitForFlush();

    which_fb ^= 1;
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Misc
//----------------------------------------------------------------------------------

// Get elapsed time measure in seconds since InitTimer()
double GetTime(void)
{
    double time = 0.0;
    struct timespec ts = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned long long int nanoSeconds = (unsigned long long int)ts.tv_sec*1000000000LLU + (unsigned long long int)ts.tv_nsec;

    time = (double)(nanoSeconds - CORE.Time.base)*1e-9;  // Elapsed time since InitTimer()

    return time;
}

// Open URL with default system browser (if available)
// NOTE: This function is only safe to use if you control the URL given.
// A user could craft a malicious string performing another action.
// Only call this function yourself not with user input or make sure to check the string yourself.
// Ref: https://github.com/raysan5/raylib/issues/686
void OpenURL(const char *url)
{
   // Security check to (partially) avoid malicious code on target platform
    if (strchr(url, '\'') != NULL) TRACELOG(LOG_WARNING, "SYSTEM: Provided URL could be potentially malicious, avoid [\'] character");
    else
    {
        // TODO:
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Inputs
//----------------------------------------------------------------------------------

// Set internal gamepad mappings
int SetGamepadMappings(const char *mappings)
{
    TRACELOG(LOG_WARNING, "SetGamepadMappings() not implemented on target platform");
    return 0;
}

// Set mouse position XY
void SetMousePosition(int x, int y)
{
    CORE.Input.Mouse.currentPosition = (Vector2){ (float)x, (float)y };
    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
}

// Set mouse cursor
void SetMouseCursor(int cursor)
{
    TRACELOG(LOG_WARNING, "SetMouseCursor() not implemented on target platform");
}

#define INPUT_DEADZONE 18

// GameCube controller button mapping table (still valid on Wii through the GC ports)
static const struct {
    uint16_t gcButton;
    int raylibButton;
} buttonMap[] = {
    { PAD_BUTTON_START, GAMEPAD_BUTTON_MIDDLE_RIGHT },
    { PAD_BUTTON_A, GAMEPAD_BUTTON_RIGHT_FACE_DOWN },
    { PAD_BUTTON_X, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT },
    { PAD_BUTTON_B, GAMEPAD_BUTTON_RIGHT_FACE_LEFT },
    { PAD_BUTTON_Y, GAMEPAD_BUTTON_RIGHT_FACE_UP },
    { PAD_TRIGGER_L, GAMEPAD_BUTTON_LEFT_TRIGGER_1 },
    { PAD_TRIGGER_R, GAMEPAD_BUTTON_RIGHT_TRIGGER_1 },
    { PAD_BUTTON_UP, GAMEPAD_BUTTON_LEFT_FACE_UP },
    { PAD_BUTTON_DOWN, GAMEPAD_BUTTON_LEFT_FACE_DOWN },
    { PAD_BUTTON_LEFT, GAMEPAD_BUTTON_LEFT_FACE_LEFT },
    { PAD_BUTTON_RIGHT, GAMEPAD_BUTTON_LEFT_FACE_RIGHT },
};

// Wii Remote (+ Nunchuk/Classic) button mapping table
// NOTE: Wiimote D-pad directions assume the remote is pointed at the screen (held vertically)
static const struct {
    uint32_t wpadButton;
    int raylibButton;
} wpadButtonMap[] = {
    { WPAD_BUTTON_PLUS, GAMEPAD_BUTTON_MIDDLE_RIGHT },
    { WPAD_BUTTON_MINUS, GAMEPAD_BUTTON_MIDDLE_LEFT },
    { WPAD_BUTTON_HOME, GAMEPAD_BUTTON_MIDDLE },
    { WPAD_BUTTON_A, GAMEPAD_BUTTON_RIGHT_FACE_DOWN },
    { WPAD_BUTTON_B, GAMEPAD_BUTTON_RIGHT_FACE_LEFT },
    { WPAD_BUTTON_1, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT },
    { WPAD_BUTTON_2, GAMEPAD_BUTTON_RIGHT_FACE_UP },
    { WPAD_BUTTON_UP, GAMEPAD_BUTTON_LEFT_FACE_UP },
    { WPAD_BUTTON_DOWN, GAMEPAD_BUTTON_LEFT_FACE_DOWN },
    { WPAD_BUTTON_LEFT, GAMEPAD_BUTTON_LEFT_FACE_LEFT },
    { WPAD_BUTTON_RIGHT, GAMEPAD_BUTTON_LEFT_FACE_RIGHT },
    { WPAD_NUNCHUK_BUTTON_Z, GAMEPAD_BUTTON_LEFT_TRIGGER_1 },
    { WPAD_NUNCHUK_BUTTON_C, GAMEPAD_BUTTON_LEFT_TRIGGER_2 },
};

static void MapControls(int padIndex, int index)
{
    uint16_t gcButton=PAD_ButtonsDown(index);
    // Iterate over the button map and update button states
    for (int j = 0; j < sizeof(buttonMap) / sizeof(buttonMap[0]); j++) {
        if (gcButton & buttonMap[j].gcButton) {
            CORE.Input.Gamepad.currentButtonState[padIndex][buttonMap[j].raylibButton] = 1;
            CORE.Input.Gamepad.lastButtonPressed = buttonMap[j].raylibButton;
        } else {
            CORE.Input.Gamepad.currentButtonState[padIndex][buttonMap[j].raylibButton] = 0;
        }
    }

    // Handle axis data
    int8_t x = PAD_StickX(index);
    int8_t y = PAD_StickY(index);
    int8_t sx = PAD_SubStickX(index);
    int8_t sy = PAD_SubStickY(index);

    float moveX = (abs(x) > INPUT_DEADZONE) ? (x / 128.0f) : 0.0f;
    float moveY = (abs(y) > INPUT_DEADZONE) ? (y / 128.0f) : 0.0f;
    float moveSubX = (abs(sx) > INPUT_DEADZONE) ? (sx / 128.0f) : 0.0f;
    float moveSubY = (abs(sy) > INPUT_DEADZONE) ? (sy / 128.0f) : 0.0f;

    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_LEFT_X] = moveX;
    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_LEFT_Y] = moveY;
    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_RIGHT_X] = moveSubX;
    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_RIGHT_Y] = moveSubY;
    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_LEFT_TRIGGER] = (float)PAD_TriggerL(index) / 255;
    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_RIGHT_TRIGGER] = (float)PAD_TriggerR(index) / 255;

    CORE.Input.Gamepad.axisCount[padIndex] = 6;
}

// Map a Wii Remote (channel index) onto raylib gamepad slot padIndex
static void MapControlsWPAD(int padIndex, int index)
{
    uint32_t held = WPAD_ButtonsHeld(index);

    for (int j = 0; j < sizeof(wpadButtonMap) / sizeof(wpadButtonMap[0]); j++)
    {
        if (held & wpadButtonMap[j].wpadButton)
        {
            CORE.Input.Gamepad.currentButtonState[padIndex][wpadButtonMap[j].raylibButton] = 1;
            CORE.Input.Gamepad.lastButtonPressed = wpadButtonMap[j].raylibButton;
        }
        else CORE.Input.Gamepad.currentButtonState[padIndex][wpadButtonMap[j].raylibButton] = 0;
    }

    // Nunchuk analog stick (if attached) -> left stick
    // NOTE: libogc/wiiuse exposes the stick as angle (degrees, 0 = up, clockwise) + magnitude (0..1)
    float moveX = 0.0f;
    float moveY = 0.0f;

    WPADData *data = WPAD_Data(index);
    if ((data != NULL) && (data->exp.type == WPAD_EXP_NUNCHUK))
    {
        float ang = data->exp.nunchuk.js.ang*(3.14159265f/180.0f);
        float mag = data->exp.nunchuk.js.mag;
        if (mag > 1.0f) mag = 1.0f;

        if (mag > 0.1f)   // radial deadzone
        {
            moveX = mag*sinf(ang);
            moveY = -mag*cosf(ang);   // raylib expects down positive
        }
    }

    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_LEFT_X] = moveX;
    CORE.Input.Gamepad.axisState[padIndex][GAMEPAD_AXIS_LEFT_Y] = moveY;
    CORE.Input.Gamepad.axisCount[padIndex] = 2;
}

// Update the raylib mouse position from the Wii Remote IR pointer (channel 0)
static void UpdateWiimotePointer(void)
{
    WPADData *data = WPAD_Data(0);

    if ((data != NULL) && data->ir.valid)
    {
        CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
        CORE.Input.Mouse.currentPosition = (Vector2){ data->ir.x, data->ir.y };

        // Mirror the IR pointer onto the touch interface (single touch point)
        CORE.Input.Touch.position[0] = CORE.Input.Mouse.currentPosition;
    }
}

// Translate a USB HID keyboard usage code (Usage Page 0x07) into a raylib key
static int MapKeyboardKeycode(u8 hid)
{
    // Contiguous ranges map directly
    if ((hid >= 0x04) && (hid <= 0x1D)) return KEY_A + (hid - 0x04);      // a..z
    if ((hid >= 0x1E) && (hid <= 0x26)) return KEY_ONE + (hid - 0x1E);    // 1..9
    if ((hid >= 0x3A) && (hid <= 0x45)) return KEY_F1 + (hid - 0x3A);     // F1..F12
    if ((hid >= 0x59) && (hid <= 0x61)) return KEY_KP_1 + (hid - 0x59);   // keypad 1..9

    switch (hid)
    {
        case 0x27: return KEY_ZERO;
        case 0x28: return KEY_ENTER;
        case 0x29: return KEY_ESCAPE;
        case 0x2A: return KEY_BACKSPACE;
        case 0x2B: return KEY_TAB;
        case 0x2C: return KEY_SPACE;
        case 0x2D: return KEY_MINUS;
        case 0x2E: return KEY_EQUAL;
        case 0x2F: return KEY_LEFT_BRACKET;
        case 0x30: return KEY_RIGHT_BRACKET;
        case 0x31: return KEY_BACKSLASH;
        case 0x33: return KEY_SEMICOLON;
        case 0x34: return KEY_APOSTROPHE;
        case 0x35: return KEY_GRAVE;
        case 0x36: return KEY_COMMA;
        case 0x37: return KEY_PERIOD;
        case 0x38: return KEY_SLASH;
        case 0x39: return KEY_CAPS_LOCK;
        case 0x46: return KEY_PRINT_SCREEN;
        case 0x47: return KEY_SCROLL_LOCK;
        case 0x48: return KEY_PAUSE;
        case 0x49: return KEY_INSERT;
        case 0x4A: return KEY_HOME;
        case 0x4B: return KEY_PAGE_UP;
        case 0x4C: return KEY_DELETE;
        case 0x4D: return KEY_END;
        case 0x4E: return KEY_PAGE_DOWN;
        case 0x4F: return KEY_RIGHT;
        case 0x50: return KEY_LEFT;
        case 0x51: return KEY_DOWN;
        case 0x52: return KEY_UP;
        case 0x53: return KEY_NUM_LOCK;
        case 0x54: return KEY_KP_DIVIDE;
        case 0x55: return KEY_KP_MULTIPLY;
        case 0x56: return KEY_KP_SUBTRACT;
        case 0x57: return KEY_KP_ADD;
        case 0x58: return KEY_KP_ENTER;
        case 0x62: return KEY_KP_0;
        case 0x63: return KEY_KP_DECIMAL;
        case 0xE0: return KEY_LEFT_CONTROL;
        case 0xE1: return KEY_LEFT_SHIFT;
        case 0xE2: return KEY_LEFT_ALT;
        case 0xE3: return KEY_LEFT_SUPER;
        case 0xE4: return KEY_RIGHT_CONTROL;
        case 0xE5: return KEY_RIGHT_SHIFT;
        case 0xE6: return KEY_RIGHT_ALT;
        case 0xE7: return KEY_RIGHT_SUPER;
        default: return KEY_NULL;
    }
}

// Drain the USB keyboard event queue into raylib key/char state
static void PollKeyboardEvents(void)
{
    keyboard_event event;

    while (KEYBOARD_GetEvent(&event))
    {
        if (event.type == KEYBOARD_PRESSED)
        {
            int key = MapKeyboardKeycode(event.keycode);
            if (key != KEY_NULL)
            {
                CORE.Input.Keyboard.currentKeyState[key] = 1;
                CORE.Input.Keyboard.keyRepeatInFrame[key] = 1;

                if (CORE.Input.Keyboard.keyPressedQueueCount < MAX_KEY_PRESSED_QUEUE)
                {
                    CORE.Input.Keyboard.keyPressedQueue[CORE.Input.Keyboard.keyPressedQueueCount] = key;
                    CORE.Input.Keyboard.keyPressedQueueCount++;
                }
            }

            // Queue printable character (symbol holds the keymap-translated codepoint)
            if ((event.symbol >= 32) && (event.symbol < 0xF000) && (event.symbol != 127))
            {
                if (CORE.Input.Keyboard.charPressedQueueCount < MAX_CHAR_PRESSED_QUEUE)
                {
                    CORE.Input.Keyboard.charPressedQueue[CORE.Input.Keyboard.charPressedQueueCount] = (int)event.symbol;
                    CORE.Input.Keyboard.charPressedQueueCount++;
                }
            }
        }
        else if (event.type == KEYBOARD_RELEASED)
        {
            int key = MapKeyboardKeycode(event.keycode);
            if (key != KEY_NULL) CORE.Input.Keyboard.currentKeyState[key] = 0;
        }
    }
}

// Register all input events
void PollInputEvents(void)
{
    /*joypad_style_t style;
    joypad_accessory_type_t accessory_type;
    bool rumble_supported;
    bool rumble_active;
    joypad_inputs_t inputs;*/

#if SUPPORT_GESTURES_SYSTEM
    // NOTE: Gestures update must be called every frame to reset gestures correctly
    // because ProcessGestureEvent() is just called on an event, not every frame
    UpdateGestures();
#endif

    // Reset keys/chars pressed registered
    CORE.Input.Keyboard.keyPressedQueueCount = 0;
    CORE.Input.Keyboard.charPressedQueueCount = 0;

    // Register previous keys states and reset key repeats
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++)
    {
        CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];
        CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;
    }

    // Poll USB keyboard events (if a keyboard is connected)
    PollKeyboardEvents();

    // Reset last gamepad button/axis registered state
    CORE.Input.Gamepad.lastButtonPressed = 0; // GAMEPAD_BUTTON_UNKNOWN
    //CORE.Input.Gamepad.axisCount = 0;

    // Register previous touch states
    for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.previousTouchState[i] = CORE.Input.Touch.currentTouchState[i];

    // Scan both input systems for this frame
    // NOTE: Wii Remotes are the primary input; GameCube controllers (through the
    // console's GC ports) are used as a fallback for any slot without a Wiimote.
    WPAD_ScanPads();
    unsigned int pads_enabled = PAD_ScanPads();

    for (int i = 0; (i < PAD_CHANMAX) && (i < MAX_GAMEPADS); i++)
    {
        u32 wpadType = 0;
        bool wpadReady = (WPAD_Probe(i, &wpadType) == WPAD_ERR_NONE);
        bool gcReady = (pads_enabled & (1 << i)) != 0;

        CORE.Input.Gamepad.ready[i] = (wpadReady || gcReady);

        if (wpadReady) MapControlsWPAD(i, i);
        else if (gcReady) MapControls(i, i);

        // TODO: Add callbacks for connect / disconnected pads
    }

    // Map the Wii Remote IR pointer (channel 0) onto the mouse/touch position
    UpdateWiimotePointer();
}

void CustomLog(int msgType, const char *text, va_list args)
{
    char buffer[1024] = { 0 };

   
    vsnprintf(buffer,1024, text, args);
    buffer[1024-1] = 0;
    
    
    switch (msgType)
    {
        case LOG_TRACE: SYS_Report("[WII][TRACE]: %s\n",buffer); break;
        case LOG_DEBUG: SYS_Report("[WII][DEBUG]: %s\n",buffer); break;
        case LOG_INFO: SYS_Report("[WII][INFO]: %s\n",buffer); break;
        case LOG_WARNING: SYS_Report("[WII][WARNING]: %s\n",buffer); break;
        case LOG_ERROR: SYS_Report("[WII][ERROR]: %s\n",buffer); break;
        case LOG_FATAL: SYS_Report("[WII][FATAL]: %s\n",buffer); break;
        default: break;
    }
}

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------

// Initialize platform: graphics, inputs and more
int InitPlatform(void)
{

    //TRACELOG(LOG_INFO, "PLATFORM: calling dreamcast gl init");
    //glKosInit();
    //TRACELOG(LOG_INFO, "PLATFORM: calling VIDEO_Init");
    VIDEO_Init();
    //TRACELOG(LOG_INFO, "PLATFORM: calling VIDEO_GetPreferredMode ");
    rmode = VIDEO_GetPreferredMode(NULL);
    /*if (rmode != NULL) {
        printf("Resolucion de Render (EFB): %dx%d\n", rmode->fbWidth, rmode->efbHeight);
        printf("Resolucion de Pantalla (XFB): %dx%d\n", rmode->fbWidth, rmode->xfbHeight);
        printf("Dimensiones VI (Salida TV): %dx%d\n", rmode->viWidth, rmode->viHeight);
        
        // Verificar si es progresivo o entrelazado
        if (rmode->viTVMode & VI_PROGRESSIVE) {
            printf("Modo: Progresivo (480p)\n");
        } else {
            printf("Modo: Entrelazado (480i)\n");
        }
    }*/
    //TRACELOG(LOG_INFO, "PLATFORM: calling SYS_AllocateFramebuffer");
    xfb[0] = SYS_AllocateFramebuffer(rmode);
    xfb[1] = SYS_AllocateFramebuffer(rmode);
    VIDEO_Configure(rmode);

    //TRACELOG(LOG_INFO, "PLATFORM: calling VIDEO_SetNextFramebuffer");
    VIDEO_SetNextFramebuffer(xfb[which_fb]);
    //TRACELOG(LOG_INFO, "PLATFORM: calling VIDEO_SetBlack");
    VIDEO_SetBlack(false);
    //TRACELOG(LOG_INFO, "PLATFORM: calling VIDEO_Flush");
    VIDEO_Flush();
    //TRACELOG(LOG_INFO, "PLATFORM: calling VIDEO_WaitForFlush");
    VIDEO_WaitForFlush();

    // Inicializar FIFO de GX
    gp_fifo = memalign(32, DEFAULT_FIFO_SIZE);
    GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);

    //TRACELOG(LOG_INFO, "PLATFORM: calling ogx_initialize");
    ogx_initialize();

    //TRACELOG(LOG_INFO, "PLATFORM: calling PAD_Init");
    PAD_Init();         // GameCube controllers (through the console GC ports)

    // Wii Remotes: enable buttons + accelerometer + IR pointer
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
    WPAD_SetVRes(WPAD_CHAN_ALL, CORE.Window.screen.width, CORE.Window.screen.height);

    // USB keyboard (events polled in PollInputEvents); NULL = no per-key callback
    if (KEYBOARD_Init(NULL) < 0) TRACELOG(LOG_WARNING, "PLATFORM: Failed to initialize USB keyboard");

    //access to all storage devices
    if(!fatInitDefault())
    {
        TRACELOG(LOG_ERROR, "PLATFORM: something was wrong mounting sd card");
    }

    CORE.Window.flags |= FLAG_FULLSCREEN_MODE;

    CORE.Window.display.width = CORE.Window.screen.width;            // User desired width
    CORE.Window.display.height = CORE.Window.screen.height;          // User desired height
    CORE.Window.render.width = CORE.Window.screen.width;
    CORE.Window.render.height = CORE.Window.screen.height;
    CORE.Window.eventWaiting = false;
    CORE.Window.screenScale = MatrixIdentity();     // No draw scaling required by default
    CORE.Window.currentFbo.width = CORE.Window.screen.width;
    CORE.Window.currentFbo.height = CORE.Window.screen.height;
    CORE.Input.Mouse.currentPosition.x = (float)CORE.Window.screen.width/2.0f;
    CORE.Input.Mouse.currentPosition.y = (float)CORE.Window.screen.height/2.0f;
    CORE.Input.Mouse.scale = (Vector2){ 1.0f, 1.0f };


    

    // At this point we need to manage render size vs screen size
    // NOTE: This function use and modify global module variables:
    //  -> CORE.Window.screen.width/CORE.Window.screen.height
    //  -> CORE.Window.render.width/CORE.Window.render.height
    //  -> CORE.Window.screenScale
    //SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);

    //ANativeWindow_setBuffersGeometry(platform.app->window, CORE.Window.render.width, CORE.Window.render.height, displayFormat);
    //ANativeWindow_setBuffersGeometry(platform.app->window, 0, 0, displayFormat);       // Force use of native display size

    
    {
        CORE.Window.render.width = CORE.Window.screen.width;
        CORE.Window.render.height = CORE.Window.screen.height;
        CORE.Window.currentFbo.width = CORE.Window.render.width;
        CORE.Window.currentFbo.height = CORE.Window.render.height;

        TRACELOG(LOG_INFO, "PLATFORM: Device initialized successfully");
        TRACELOG(LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
        TRACELOG(LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
        TRACELOG(LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
        TRACELOG(LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);
    }

    // Load OpenGL extensions
    // NOTE: GL procedures address loader is required to load extensions
    //rlLoadExtensions(eglGetProcAddress);
    //const char *gl_exts = (char *) glGetString(GL_EXTENSIONS);
    //TRACELOG(LOG_INFO,"PLATFORM: GL_VENDOR:   \"%s\"", glGetString(GL_VENDOR));
    //TRACELOG(LOG_INFO,"PLATFORM: GL_VERSION:  \"%s\"", glGetString(GL_VERSION));
    //TRACELOG(LOG_INFO,"PLATFORM: GL_RENDERER: \"%s\"", glGetString(GL_RENDERER));
    //TRACELOG(LOG_INFO,"PLATFORM: SL_VERSION:  \"%s\"", glGetString(GL_SHADING_LANGUAGE_VERSION));
    CORE.Window.ready = true;

    // Initialize hi-res timer
    InitTimer();

    // Initialize base path for storage
    CORE.Storage.basePath = GetWorkingDirectory();
    TRACELOG(LOG_INFO, "PLATFORM: Initialized");

    return 0;
}

// Close platform
void ClosePlatform(void)
{
    // TODO: De-initialize graphics and more
    KEYBOARD_Deinit();
    WPAD_Shutdown();
}

// EOF
