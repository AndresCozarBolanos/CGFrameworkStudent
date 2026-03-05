#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "entity.h"
#include "material.h"
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

    framebuffer.Resize(w, h);
    canvas.Resize(w, h);
    canvas.Fill(backgroundColor);
}

// Destructor
Application::~Application() {}

// Initialization
void Application::Init()
{
    camera = new Camera();
    camera->LookAt(Vector3(0, 10, 20), Vector3(0, 5, 0), Vector3(0, 1, 0));
    camera->SetPerspective(45.0f, window_width / (float)window_height, 0.1f, 1000.0f);

    ambient_light = Vector3(0.2f, 0.2f, 0.2f);
    sLight main_light;
    main_light.position = Vector3(10.0f, 20.0f, 10.0f);
    main_light.diffuse_color = Vector3(1.0f, 1.0f, 1.0f);
    scene_lights.push_back(main_light);

    Mesh* lee = new Mesh();
    lee->LoadOBJ("meshes/lee.obj");

    //Texture* lee_texture = Texture::Get("textures/lee_color_specular.tga");

    //Shader* raster_shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");

    Matrix44 m;
    m.SetIdentity();

    entity = new Entity(lee, m);
    entity->position = Vector3(0, 0, 0);
    entity->rotation_axis = Vector3(0, 1, 0);
    entity->rotation_speed = 1.0f;
    entity->scale_value = 20.0f;
    //entity->texture = lee_texture; 
	//entity->shader = raster_shader;

    Material* mat = new Material();
    mat->diffuse_texture = Texture::Get("textures/lee_color_specular.tga");
    mat->shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
    entity->material = mat;

    entity->Update(0.0f);

    // ---- MODE M: multi entities ----
    //entities.clear();

    //Entity* e1 = new Entity(lee, m);
    //e1->position = Vector3(-6, 4, 0);             
    //e1->rotation_axis = Vector3(0, 1, 0);
    //e1->rotation_speed = 1.2f;
    //e1->scale_value = 10.0f;
    //if (hasTexture) e1->texture = lee_texture;
    //e1->Update(0.0f);

    //Entity* e2 = new Entity(lee, m);
    //e2->position = Vector3(0, 4, 0);              
    //e2->rotation_axis = Vector3(1, 0, 0);
    //e2->rotation_speed = 1.0f;
    //e2->scale_value = 10.0f;
    //if (hasTexture) e2->texture = lee_texture;
    //e2->Update(0.0f);

    //Entity* e3 = new Entity(lee, m);
    //e3->position = Vector3(6, 4, 0);              
    //e3->rotation_axis = Vector3(0, 0, 1);
    //e3->rotation_speed = 0.8f;
    //e3->scale_value = 10.0f;
    //if (hasTexture) e3->texture = lee_texture;
    //e3->Update(0.0f);

    //entities.push_back(e1);
    //entities.push_back(e2);
    //entities.push_back(e3);

    //---- MODE U: single entity ----
    

    //shader = Shader::Get("shaders/quad.vs", "shaders/quad1,1.fs");

   //Buffer.Resize(window_width, window_height);

    mesh = new Mesh();
    mesh->CreateQuad();
    texture = Texture::Get("images/fruits.png");

    mode = 4;
    mat->shader = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
    //shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
    //shader = Shader::Get("shaders/quad.vs", "shaders/quad1,1.fs");
}

void Application::Render()
{
    //framebuffer.Fill(Color::BLACK);
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mode == 4)
    {
        glEnable(GL_DEPTH_TEST); 

        uniform_data.viewprojection_matrix = camera->viewprojection_matrix;
        uniform_data.camera_position = camera->eye;
        uniform_data.ambient_light = ambient_light;
        uniform_data.lights = scene_lights;

        if (entity) {
            entity->Render(uniform_data);
        }
    }
    else
    {
        glDisable(GL_DEPTH_TEST); 

        if (shader) {
            shader->Enable();
            shader->SetFloat("u_time", time);
            shader->SetTexture("u_texture", texture);
            shader->SetVector2("u_resolution", Vector2(window_width, window_height)); 

            mesh->Render(); 

            shader->Disable();
        }
    }
    //else if (mode == 2)
    //{
        //entities[0]->Render(&framebuffer, camera, &zBuffer, show_texture, use_zbuffer, interpolate_uvs);
        //entities[1]->Render(&framebuffer, camera, &zBuffer, show_texture, use_zbuffer, interpolate_uvs);
        //entities[2]->Render(&framebuffer, camera, &zBuffer, show_texture, use_zbuffer, interpolate_uvs);

    //}

    //framebuffer.Render();
}


void Application::Update(float dt)
{
    if (mode == 4)
    {
        if (entity) entity->Update(dt);
    }
    //else if (mode == 2)
    //{
        //for (Entity* e : entities)
            //if (e) e->Update(dt);
    //}
    float speed = 0.5f;

    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        Matrix44 R;
        R.MakeRotationMatrix(-mouse_delta.x * speed * dt, Vector3(0, 1, 0));
		// Rotate the camera around the center point
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
    switch(event.keysym.sym)
    {
        case SDLK_1:
            mode = 1;
            shader = Shader::Get("shaders/quad.vs", "shaders/quad1,1.fs");
            break;

        case SDLK_2:
            mode = 2;
            shader = Shader::Get("shaders/quad.vs", "shaders/quad2,1.fs");
            break;

        case SDLK_3: 
            mode = 3;
            shader = Shader::Get("shaders/quad.vs", "shaders/quad3,1.fs");
            break;

        case SDLK_4: 
            mode = 4; 
			shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
			break;

        case SDLK_a:
            if (mode == 1) shader = Shader::Get("shaders/quad.vs", "shaders/quad1,1.fs");
            else if (mode == 2) shader = Shader::Get("shaders/quad.vs", "shaders/quad2,1.fs");
            else if (mode == 3) shader = Shader::Get("shaders/quad.vs", "shaders/quad3,1.fs");
			else if (mode == 4) shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
            break;

        case SDLK_b:
            if (mode == 1) shader = Shader::Get("shaders/quad.vs", "shaders/quad1,2.fs");
            else if (mode == 2) shader = Shader::Get("shaders/quad.vs", "shaders/quad2,2.fs");
            else if (mode == 3) shader = Shader::Get("shaders/quad.vs", "shaders/quad3,2.fs");
            break;

        case SDLK_c:
            if (mode == 1) shader = Shader::Get("shaders/quad.vs", "shaders/quad1,3.fs");
            else if (mode == 2) shader = Shader::Get("shaders/quad.vs", "shaders/quad2,3.fs");
            break;

        case SDLK_d:
            if (mode == 1) shader = Shader::Get("shaders/quad.vs", "shaders/quad1,4.fs");
            else if (mode == 2) shader = Shader::Get("shaders/quad.vs", "shaders/quad2,4.fs");
            break;

        case SDLK_e:
            if (mode == 1) shader = Shader::Get("shaders/quad.vs", "shaders/quad1,5.fs");
            else if (mode == 2) shader = Shader::Get("shaders/quad.vs", "shaders/quad2,5.fs");
            break;

        case SDLK_f:
            if (mode == 1) shader = Shader::Get("shaders/quad.vs", "shaders/quad1,6.fs");
            else if (mode == 2) shader = Shader::Get("shaders/quad.vs", "shaders/quad2,6.fs");
            break;

        case SDLK_ESCAPE: exit(0);

        //case SDLK_1: mode = 1; break; // SINGLE ENTITY
        //case SDLK_2: mode = 2; break; // MULTI ANIMATED ENTITIES

        case SDLK_n: current_prop = 'N'; break;
        //case SDLK_f: current_prop = 'F'; break;
        case SDLK_v: current_prop = 'V'; break;

        case SDLK_t:
            show_texture = !show_texture;
            break;
        case SDLK_z:
            use_zbuffer = !use_zbuffer;
            break;
        case SDLK_w:
            wireframe = !wireframe;

            if (entity) {
                entity->mode = wireframe ? eRenderMode::WIREFRAME : eRenderMode::TRIANGLES;
            }
            for (Entity* e : entities) {
                if (e) e->mode = wireframe ? eRenderMode::WIREFRAME : eRenderMode::TRIANGLES;
            }
            break;
        //case SDLK_c:
            //interpolate_uvs = !interpolate_uvs;
            //break;

        case SDLK_PLUS:
        case SDLK_KP_PLUS: 
            if (current_prop == 'N') camera->near_plane += 1.0f;
            //if (current_prop == 'F') camera->far_plane += 50.0f;
            if (current_prop == 'V') camera->fov += 5.0f;
            camera->UpdateProjectionMatrix(); 
            break;

        case SDLK_MINUS:
        case SDLK_KP_MINUS: 
            if (current_prop == 'N') camera->near_plane = std::max(0.01f, camera->near_plane - 1.0f);
            //if (current_prop == 'F') camera->far_plane = std::max(1.0f, camera->far_plane - 50.0f);
            if (current_prop == 'V') camera->fov = std::max(1.0f, camera->fov - 5.0f);
            camera->UpdateProjectionMatrix(); 
            break;

        default: break;
    }
}


void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
    if (event.button != SDL_BUTTON_LEFT) return;

    for (const Button& b : buttons)
    {
        if (!b.IsMouseInside(mouse_position)) continue;

        if (b.type==ButtonType::CLEAR){ canvas.Fill(backgroundColor); return; }
        if (b.type==ButtonType::SAVE){ canvas.SaveTGA("paint.tga"); return; }
        if (b.type==ButtonType::LOAD){
            Image img;
            if (img.LoadTGA("paint.tga",true))
            {
                canvas.Fill(backgroundColor);
                canvas.DrawImage(img,0,0);
            }
            return;
        }

        if (b.type==ButtonType::PENCIL) currentTool=Tool::PENCIL;
        if (b.type==ButtonType::ERASER) currentTool=Tool::ERASER;
        if (b.type==ButtonType::LINE) currentTool=Tool::LINE;
        if (b.type==ButtonType::RECTANGLE) currentTool=Tool::RECT;
        if (b.type==ButtonType::TRIANGLE) currentTool=Tool::TRIANGLE;

        if (b.type==ButtonType::COLOR_BLACK) currentColor=Color::BLACK;
        if (b.type==ButtonType::COLOR_WHITE) currentColor=Color::WHITE;
        if (b.type==ButtonType::COLOR_RED) currentColor=Color::RED;
        if (b.type==ButtonType::COLOR_BLUE) currentColor=Color::BLUE;
        if (b.type==ButtonType::COLOR_CYAN) currentColor=Color::CYAN;
        if (b.type==ButtonType::COLOR_YELLOW) currentColor=Color::YELLOW;
        if (b.type==ButtonType::COLOR_PINK) currentColor=Color::PURPLE;
        if (b.type==ButtonType::COLOR_GREEN) currentColor=Color::GREEN;

        return;
    }

    if (mouse_position.y < toolbarH) return;

    if (currentTool==Tool::PENCIL || currentTool==Tool::ERASER)
    {
        isDrawing=true;
        prevPos=mouse_position;
    }
    else if (currentTool==Tool::LINE || currentTool==Tool::RECT)
    {
        isPlacing=true;
        startPos=endPos=mouse_position;
    }
    else if (currentTool==Tool::TRIANGLE)
    {
        if (triClicks==0){ triP0=mouse_position; triClicks=1; }
        else if (triClicks==1){ triP1=mouse_position; triClicks=2; }
        else{
            canvas.DrawTriangle(triP0,triP1,mouse_position,currentColor,isFilled,currentColor);
            triClicks=0;
        }
    }
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
    if (event.button!=SDL_BUTTON_LEFT) return;

    if (isDrawing){ isDrawing=false; return; }
    if (!isPlacing) return;

    isPlacing=false;

    if (currentTool==Tool::LINE)
        canvas.DrawLineDDA((int)startPos.x,(int)startPos.y,(int)endPos.x,(int)endPos.y,currentColor);

    if (currentTool==Tool::RECT)
    {
        int x0 = (int)startPos.x;
        int y0 = (int)startPos.y;
        int x1 = (int)endPos.x;
        int y1 = (int)endPos.y;

        int x = std::min(x0, x1);
        int y = std::min(y0, y1);
        int w = std::abs(x1 - x0);
        int h = std::abs(y1 - y0);
        canvas.DrawRect(x,y,w,h,currentColor,borderWidth,isFilled,currentColor);
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent)
{
    if (isPlacing) endPos=mouse_position;

    if (isDrawing)
    {
        Color c = (currentTool==Tool::ERASER) ? backgroundColor : currentColor;

        canvas.SetPixel((unsigned int)mouse_position.x, (unsigned int)mouse_position.y, c);

        canvas.DrawLineDDA((int)prevPos.x, (int)prevPos.y,
                        (int)mouse_position.x, (int)mouse_position.y, c);

        prevPos = mouse_position;
    }

}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float factor = (event.y > 0) ? 0.9f : 1.1f;
    Vector3 forward = camera->eye - camera->center;
    camera->eye = camera->center + forward * factor;
    camera->UpdateViewMatrix();
}
void Application::OnFileChanged(const char* filename){ Shader::ReloadSingleShader(filename); }