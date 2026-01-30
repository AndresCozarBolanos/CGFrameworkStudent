#pragma once

#include <vector>
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"
#include "particlesystem.h"

class Application
{
public:
    // Window
    SDL_Window* window = nullptr;
    int window_width;
    int window_height;

    float time;

    // Input
    const Uint8* keystate;
    int mouse_state;
    Vector2 mouse_position;
    Vector2 mouse_delta;

    Entity* entity = nullptr;
    Camera* camera = nullptr;

    // Framebuffer
    Image framebuffer;
    Image canvas;
    Color backgroundColor = Color::BLACK;

    // Tools
    enum class Tool { NONE, PENCIL, ERASER, LINE, RECT, TRIANGLE };
    Tool currentTool = Tool::NONE;

    // State
    bool isPlacing = false;
    bool isDrawing = false;
    Vector2 startPos, endPos, prevPos;

    int triClicks = 0;
    Vector2 triP0, triP1;

    Color currentColor = Color::WHITE;
    bool isFilled = false;

    int borderWidth = 1;

    // UI
    std::vector<Button> buttons;
    int toolbarH = 50;

    // Particles
    ParticleSystem ps;
    bool particlesMode = false;

    Application(const char* caption, int width, int height);
    ~Application();

    void Init();
    void Render();
    void Update(float dt);

    void OnKeyPressed(SDL_KeyboardEvent event);
    void OnMouseButtonDown(SDL_MouseButtonEvent event);
    void OnMouseButtonUp(SDL_MouseButtonEvent event);
    void OnMouseMove(SDL_MouseButtonEvent event);
    void OnWheel(SDL_MouseWheelEvent event);
    void OnFileChanged(const char* filename);

    void SetWindowSize(int width, int height)
    {
        glViewport(0, 0, width, height);
        window_width = width;
        window_height = height;
        framebuffer.Resize(width, height);
        canvas.Resize(width, height);
        canvas.Fill(backgroundColor);
    }
};