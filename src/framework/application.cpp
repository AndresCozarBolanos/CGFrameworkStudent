#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "entity.h"
#include "material.h"
#include "utils.h"
#include <algorithm>

// Constructor
Application::Application(const char* caption, int width, int height)
{
    window = createWindow(caption, width, height);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    window_width = w;
    window_height = h;

    keystate = SDL_GetKeyboardState(nullptr);
}

// Destructor
Application::~Application() {}

// Initialization
void Application::Init()
{
    camera = new Camera();
    camera->LookAt(Vector3(0, 10, 20), Vector3(0, 5, 0), Vector3(0, 1, 0));
    camera->SetPerspective(45.0f, window_width / (float)window_height, 0.1f, 1000.0f);

    is_lab5 = false;
    mode = 4;
    num_lights = 1;
    ambient_light = Vector3(0.2f, 0.2f, 0.2f);

    use_color_texture = true;
    use_specular_texture = true;
    use_normal_texture = true;

    scene_lights.clear();

    sLight luz0_white;
    luz0_white.position = Vector3(10.0f, 20.0f, 10.0f);
    luz0_white.diffuse_color = Vector3(1.0f, 1.0f, 1.0f);
    scene_lights.push_back(luz0_white);

    sLight luz1_purple;
    luz1_purple.position = Vector3(5.0f, 2.0f, 5.0f);
    luz1_purple.diffuse_color = Vector3(1.2f, 0.0f, 2.0f);
    scene_lights.push_back(luz1_purple);

    sLight luz2_pink;
    luz2_pink.position = Vector3(-5.0f, 2.0f, 5.0f);
    luz2_pink.diffuse_color = Vector3(1.0f, 0.2f, 0.6f);
    scene_lights.push_back(luz2_pink);

    Mesh* lee = new Mesh();
    lee->LoadOBJ("meshes/lee.obj");

    Matrix44 m;
    m.SetIdentity();

    entity = new Entity(lee, m);
    entity->position = Vector3(0, 0, 0);
    entity->rotation_axis = Vector3(0, 1, 0);
    entity->rotation_speed = 1.0f;
    entity->scale_value = 20.0f;

    Material* mat = new Material();
    mat->shader = Shader::Get("shaders/raster/raster.vs", "shaders/raster/raster.fs");

    mat->diffuse_texture  = Texture::Get("textures/lee_color_specular.tga");
    mat->normal_texture   = Texture::Get("textures/lee_normal.tga");
    mat->specular_texture = Texture::Get("textures/lee_color_specular.tga");

    mat->color = Vector3(1.0f, 1.0f, 1.0f);
    mat->shininess = 32.0f;

    entity->material = mat;
    entity->Update(0.0f);

    mesh = new Mesh();
    mesh->CreateQuad();

    texture = Texture::Get("images/fruits.png");
    shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_1.fs");
}

void Application::Render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (is_lab5)
    {
        glEnable(GL_DEPTH_TEST);

        uniform_data.viewprojection_matrix = camera->viewprojection_matrix;
        uniform_data.camera_position = camera->eye;
        uniform_data.ambient_light = ambient_light;
        uniform_data.lights.clear();

        uniform_data.use_color_texture = use_color_texture ? 1 : 0;
        uniform_data.use_specular_texture = use_specular_texture ? 1 : 0;
        uniform_data.use_normal_texture = use_normal_texture ? 1 : 0;

        if (num_lights >= 1 && scene_lights.size() > 0)
            uniform_data.lights.push_back(scene_lights[0]);
        if (num_lights >= 2 && scene_lights.size() > 1)
            uniform_data.lights.push_back(scene_lights[1]);
        if (num_lights >= 3 && scene_lights.size() > 2)
            uniform_data.lights.push_back(scene_lights[2]);

        if (entity)
            entity->Render(uniform_data);
    }

    else if (mode == 4)
    {
        glEnable(GL_DEPTH_TEST);

        if (entity)
            entity->Render(camera);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);

        if (shader)
        {
            shader->Enable();
            shader->SetFloat("u_time", time);
            shader->SetTexture("u_texture", texture);
            shader->SetVector2("u_resolution", Vector2((float)window_width, (float)window_height));
            mesh->Render();
            shader->Disable();
        }
    }
}

void Application::Update(float dt)
{
    if ((mode == 4 || is_lab5) && entity)
        entity->Update(dt);

    float speed = 0.5f;

    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        Matrix44 R;
        R.MakeRotationMatrix(-mouse_delta.x * speed * dt, Vector3(0, 1, 0));
        Vector3 new_eye = R * (camera->eye - camera->center);
        camera->eye = camera->center + new_eye;
        camera->LookAt(camera->eye, camera->center, camera->up);
    }

    if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        Vector3 delta = camera->GetLocalVector(Vector3(mouse_delta.x, -mouse_delta.y, 0)) * speed * dt;
        camera->eye = camera->eye - delta;
        camera->center = camera->center - delta;
        camera->UpdateViewMatrix();
    }
}

void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    switch (event.keysym.sym)
    {
        case SDLK_l:
        {
            is_lab5 = !is_lab5;

            if (entity && entity->material)
            {
                if (is_lab5)
                    entity->material->shader = Shader::Get("shaders/lighting/phong.vs", "shaders/lighting/phong.fs");
                else
                    entity->material->shader = Shader::Get("shaders/raster/raster.vs", "shaders/raster/raster.fs");
            }
            break;
        }

        case SDLK_g:
        {
            if (is_lab5 && entity && entity->material)
                entity->material->shader = Shader::Get("shaders/lighting/gouraud.vs", "shaders/lighting/gouraud.fs");
            break;
        }

        case SDLK_p:
        {
            if (is_lab5 && entity && entity->material)
                entity->material->shader = Shader::Get("shaders/lighting/phong.vs", "shaders/lighting/phong.fs");
            break;
        }

        case SDLK_c:
        {
            if (is_lab5)
                use_color_texture = !use_color_texture;
            else
            {
                if (mode == 1) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_3.fs");
                else if (mode == 2) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_3.fs");
            }
            break;
        }

        case SDLK_s:
        {
            if (is_lab5)
                use_specular_texture = !use_specular_texture;
            break;
        }

        case SDLK_n:
        {
            if (is_lab5)
                use_normal_texture = !use_normal_texture;
            else
                current_prop = 'N';
            break;
        }

        case SDLK_1:
        {
            if (is_lab5)
                num_lights = 1;
            else
            {
                mode = 1;
                shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_1.fs");
            }
            break;
        }

        case SDLK_2:
        {
            if (is_lab5)
                num_lights = 2;
            else
            {
                mode = 2;
                shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_1.fs");
            }
            break;
        }

        case SDLK_3:
        {
            if (is_lab5)
                num_lights = 3;
            else
            {
                mode = 3;
                shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad3_1.fs");
            }
            break;
        }

        case SDLK_4:
        {
            if (is_lab5)
                num_lights = 4;
            else
                mode = 4;
            break;
        }

        case SDLK_a:
        {
            if (!is_lab5)
            {
                if (mode == 1) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_1.fs");
                else if (mode == 2) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_1.fs");
                else if (mode == 3) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad3_1.fs");
                else if (mode == 4 && entity && entity->material)
                    entity->material->shader = Shader::Get("shaders/raster/raster.vs", "shaders/raster/raster.fs");
            }
            break;
        }

        case SDLK_b:
        {
            if (!is_lab5)
            {
                if (mode == 1) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_2.fs");
                else if (mode == 2) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_2.fs");
                else if (mode == 3) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad3_2.fs");
            }
            break;
        }

        case SDLK_d:
        {
            if (!is_lab5)
            {
                if (mode == 1) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_4.fs");
                else if (mode == 2) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_4.fs");
            }
            break;
        }

        case SDLK_e:
        {
            if (!is_lab5)
            {
                if (mode == 1) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_5.fs");
                else if (mode == 2) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_5.fs");
            }
            break;
        }

        case SDLK_f:
        {
            if (!is_lab5)
            {
                if (mode == 1) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad1_6.fs");
                else if (mode == 2) shader = Shader::Get("shaders/quads/quad.vs", "shaders/quads/quad2_6.fs");
            }
            break;
        }

        case SDLK_v:
        {
            current_prop = 'V';
            break;
        }

        case SDLK_PLUS:
        case SDLK_KP_PLUS:
        {
            if (current_prop == 'N') camera->near_plane += 1.0f;
            if (current_prop == 'V') camera->fov += 5.0f;
            camera->UpdateProjectionMatrix();
            break;
        }

        case SDLK_MINUS:
        case SDLK_KP_MINUS:
        {
            if (current_prop == 'N') camera->near_plane = std::max(0.01f, camera->near_plane - 1.0f);
            if (current_prop == 'V') camera->fov = std::max(1.0f, camera->fov - 5.0f);
            camera->UpdateProjectionMatrix();
            break;
        }

        case SDLK_ESCAPE:
        {
            exit(0);
            break;
        }

        default:
            break;
    }
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event){}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event){}

void Application::OnMouseMove(SDL_MouseButtonEvent){}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float factor = (event.y > 0) ? 0.9f : 1.1f;
    Vector3 forward = camera->eye - camera->center;
    camera->eye = camera->center + forward * factor;
    camera->UpdateViewMatrix();
}

void Application::OnFileChanged(const char* filename){ Shader::ReloadSingleShader(filename); }