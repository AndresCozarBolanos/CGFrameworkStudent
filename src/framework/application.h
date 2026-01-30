#pragma once

#include <vector>
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"
#include "particlesystem.h"
#include "entity.h"   // <-- NECESARIO para Entity

class Application
{
public:
    // Window
    SDL_Window* window = nullptr;
    int window_width = 0;
    int window_height = 0;

    float time = 0.0f;

    // Input
    const Uint8* keystate = nullptr;
    int mouse_state = 0;
    Vector2 mouse_position;
    Vector2 mouse_delta;

    // --- LAB2: escena ---
    Camera* camera = nullptr;

    Entity* entity = nullptr;               // modo 1: single
    std::vector<Entity*> entities;          // modo 2: multi
    int mode = 1;                           // 1 = single, 2 = multi

    // Framebuffer
    Image framebuffer;
    Image canvas;
    Color backgroundColor = Color::BLACK;

    // Tools (paint)
    enum class Tool { NONE, PENCIL, ERASER, LINE, RECT, TRIANGLE };
    Tool currentTool = Tool::NONE;

    // State (paint)
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

    // Particles (si lo quieres conservar)
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

        // IMPORTANT: si cambias tamaño, actualiza aspect (2.3/2.5)
        if (camera)
            camera->SetAspectRatio(width / (float)height);
    }

    
};
