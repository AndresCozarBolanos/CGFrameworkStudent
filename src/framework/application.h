#pragma once

#include <vector>
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "entity.h"   
#include "mesh.h"
#include "shader.h"
#include "material.h"

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

    //lab 4
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
	Texture* texture = nullptr;
    int actual_quad = 1;

	//lab 5
    sUniformData uniform_data;
    Vector3 ambient_light;
    std::vector<sLight> scene_lights;
	bool is_lab5 = false;
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

        framebuffer.Resize(width, height);

        if (camera)
            camera->SetAspectRatio(width / (float)height);
    }

    
};
