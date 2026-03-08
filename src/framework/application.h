#pragma once

#include <vector>
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"
#include "entity.h"
#include "camera.h"

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

    // Camera
    Camera* camera = nullptr;

    // Scene
    Entity* entity = nullptr;
    std::vector<Entity*> entities;

    // General mode
    int mode = 1;
    bool is_lab5 = false;

    // Camera controls
    float camera_speed = 0.5f;
    float camera_property_step = 0.1f;
    char current_prop = 'N';

    // -------------------------
    // LAB 4
    // -------------------------
    Mesh* mesh = nullptr;          // quad
    Shader* shader = nullptr;      // quad shader
    Texture* texture = nullptr;    // quad texture
    int actual_quad = 1;

    // -------------------------
    // LAB 5
    // -------------------------
    sUniformData uniform_data;
    Vector3 ambient_light = Vector3(0.2f, 0.2f, 0.2f);
    std::vector<sLight> scene_lights;

    bool use_color_texture = true;
    bool use_specular_texture = true;
    bool use_normal_texture = true;

    int num_lights = 1;

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

        if (camera)
            camera->SetAspectRatio(width / (float)height);
    }
};