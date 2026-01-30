#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>

// TODO:
//  Error handling on GLFW init failure

struct GLFWwindow;
struct GLFWmonitor;

namespace Base
{    
    struct WindowHint
    {
        i32 Hint;
        i32 Value;
    };
    
    struct WindowConfig
    {
        uvec2 Pos = { 0, 0 };
        uvec2 Size = { 0, 0 };
        GLFWmonitor* Monitor = nullptr;
        
        //Can All Be Left Default
        const char* Title = "Default Title";
        bool Resizeable = true;
        bool InitiallyVisible = true;
        bool HaveDecorations = true;
        bool InituiallyFocused = true;
        bool CenterCursorOnStartup = false;

    };

    enum CursorModes
    {
        CursorNormal,
        CursorHidden,
        CursorDisabled,
        CursorCapured,
    };

    enum MouseButtonCodes
    {
        LeftButton,     
        RightButton,     
        MiddleWheel,    
        Button4,
        Button5,
        Button6,
        Button7,
        NumMouseButtonCodes,
    };

    enum KeyCodes
    {
        UnknownKey,

        HatCentered,
        HatUp,
        HatRight,
        HatDown,
        HatLeft,
        HatRightUp,
        HatRightDown,
        HatLeftUp,
        HatLeftDown,

        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,

        Key0, Key1, Key2, Key3, Key4, 
        Key5, Key6, Key7, Key8, Key9,

        SemiColon,
        Equal,

        A, B, C, D, E, F, G, H, I, J, 
        K, L, M, N, O, P, Q, R, S, T,
        U, V, W, X, Y, Z,

        LeftBracket,
        Backslash,
        RightBracket,
        GraveAccent,
        World1,
        World2,
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,

        Right,
        Left,
        Down,
        Up,

        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,

        F1,  F2,  F3,  F4,  F5,  
        F6,  F7,  F8,  F9,  F10, 
        F11, F12, F13, F14, F15, 
        F16, F17, F18, F19, F20, 
        F21, F22, F23, F24, F25,
        
        KP0, KP1, KP2, KP3, KP4,
        KP5, KP6, KP7, KP8, KP9,
        
        KPDecimal,
        KPDivide,
        KPMultiply,
        KPSubtract,
        KPAdd,
        KPEnter,
        KPEqual,

        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,

        Menu,

        NumKeyCodes,
    };
        
    class Window
    {
    public:
        Window(const WindowConfig& Config);
        ~Window();
        bool ShouldClose();
        void Tick();

        ivec2 GetWindowPos();
        ivec2 GetWindowSize();
        ivec2 GetFrameBufferSize();
        GLFWwindow* GetGLFWWindow();
        GLFWmonitor* GetPrimaryMonitor();
        f64 GetDeltaTime();

        void Close();
        void SetWindowPos(const ivec2& NewPosition);
        void SetWindowSize(const ivec2& Size);

        void ToggleFullscreen();
        bool IsFullScreen();
        void Minimize();

        void SetCursorMode(CursorModes Mode);
        dvec2 GetMousePosition();
        dvec2 GetMouseDelta();
        f64 GetScrollDelta();

        bool IsMouseButtonDown(MouseButtonCodes Button);
        static const char* GetMouseButtonName(MouseButtonCodes Button);
        bool WasMouseButtonJustPressed(MouseButtonCodes Button);
        f64 GetTimeSinceMouseButtonPressed(MouseButtonCodes Button);

        bool IsKeyDown(KeyCodes Key);
        static const char* GetKeyName(KeyCodes Key);
        bool WasKeyJustPressed(KeyCodes Key);
        f64 GetTimeSinceKeyPressed(KeyCodes Key);

    private:
        struct Key
        {
            bool Pressed = false;
            bool Released = true; 
            f64 TimeSincePressed = 0.0;
        };

        std::array<Key, NumKeyCodes> Keys;
        std::array<Key, NumMouseButtonCodes> MouseButtons;

        dvec2 MousePosition = { 0.0, 0.0 };
        dvec2 LastMousePosition = { 0.0, 0.0 };
        dvec2 MouseDelta = { 0.0, 0.0 };
        f64 ScrollDelta = 0.0;
        f64 ScrollOffset= 0.0;

        GLFWwindow* WindowInstance = nullptr;
        GLFWmonitor* Monitor = nullptr;
        bool PendingClose = false;

        ivec2 FrameBufferSize = { 0, 0 };
        ivec2 Size = { 0, 0 };
        ivec2 Position = { 0, 0 };

        f64 DeltaTime = 0.0;
        f64 LastFrameTime = 0.0;

        ivec2 LastWindowedSize = { 0, 0 };
        ivec2 LastWindowedPosition = { 0, 0 };

        void SetWindowHints(const WindowConfig& Config);

        void SetPositionInternal(const ivec2& NewPosition);
        void SetSizeInternal(const ivec2& NewSize);
        void SetFrameBufferSizeInternal(const ivec2& NewFrameBufferSize);
        
        static constexpr KeyCodes ConvertGLFWKey(i32 GLFWKey);
        static constexpr MouseButtonCodes ConvertGLFWMouseButton(i32 GLFWMouseButton);
        static constexpr i32 ConvertToGLFWCursorMode(CursorModes CursorMode);

        static void PositionCallBack(GLFWwindow* WindowInstance, int X, int Y);
        static void SizeCallBack(GLFWwindow* WindowInstance, int Width, int Height);
        static void FrameBufferSizeCallBack(GLFWwindow* WindowInstance, int Width, int Height);
        
        static void KeyCallBack(GLFWwindow* WindowInstance, int Key, int Scancode, int Action, int Mods);
        static void CursorPositionCallBack(GLFWwindow* WindowInstance, double XPosition, double YPosition);
        static void MouseButtonCallBack(GLFWwindow* WindowInstance, int Button, int Action, int Mods);
        static void ScrollCallBack(GLFWwindow* WindowInstance, double XOffset, double YOffset);
    };
}