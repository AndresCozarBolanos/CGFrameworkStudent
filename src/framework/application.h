#pragma once

#include <vector>
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"
#include "particlesystem.h"
#include "entity.h"   
#include "mesh.h"
#include "shader.h"

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

    Camera* camera = nullptr;

    Entity* entity = nullptr;               
    std::vector<Entity*> entities;          
    int mode = 1; 

    float camera_speed = 0.5f;
    float camera_property_step = 0.1f;
    char current_prop = 'N'; 

    FloatImage zBuffer;
    bool show_texture = true;
    bool use_zbuffer = true;
    bool interpolate_uvs = true;

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

    bool wireframe = false;

    // UI
    std::vector<Button> buttons;
    int toolbarH = 50;

    //lab 4
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
    //int actual_shader = 0;

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

        if (camera)
            camera->SetAspectRatio(width / (float)height);
    }

    
};
