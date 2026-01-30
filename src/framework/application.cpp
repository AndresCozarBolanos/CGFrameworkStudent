#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "entity.h"
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

    Mesh* lee = new Mesh();
    lee->LoadOBJ("meshes/lee.obj");

    // ---- MODO 1: single entity ----
    Matrix44 m; 
    m.SetIdentity();

    entity = new Entity(lee, m);
    entity->position = Vector3(0, 0, 0);
    entity->rotation_axis = Vector3(0, 1, 0);
    entity->rotation_speed = 1.0f;
    entity->scale_value = 20.0f;   // SIEMPRE positiva
    entity->Update(0.0f);

    // ---- MODO 2: multi entities (mínimo 3) ----
    entities.clear();

    Entity* e1 = new Entity(lee, m);
    e1->position = Vector3(-6, 4, 0);             // <-- subido
    e1->rotation_axis = Vector3(0, 1, 0);
    e1->rotation_speed = 1.2f;
    e1->scale_value = 10.0f;
    e1->Update(0.0f);

    Entity* e2 = new Entity(lee, m);
    e2->position = Vector3(0, 4, 0);              // <-- subido y centrado en X
    e2->rotation_axis = Vector3(1, 0, 0);
    e2->rotation_speed = 1.0f;
    e2->scale_value = 10.0f;
    e2->Update(0.0f);

    Entity* e3 = new Entity(lee, m);
    e3->position = Vector3(6, 4, 0);              // <-- subido
    e3->rotation_axis = Vector3(0, 0, 1);
    e3->rotation_speed = 0.8f;
    e3->scale_value = 10.0f;
    e3->Update(0.0f);

    entities.push_back(e1);
    entities.push_back(e2);
    entities.push_back(e3);

    mode = 1;
}

void Application::Render()
{
    framebuffer.Fill(Color::BLACK);
    framebuffer.DrawImage(canvas, 0, 0);

    if (mode == 1)
    {
        if (entity)
            entity->Render(&framebuffer, camera, Color::WHITE);
    }
    else if (mode == 2)
    {
        if (entities.size() >= 1 && entities[0]) entities[0]->Render(&framebuffer, camera, Color::BLUE);
        if (entities.size() >= 2 && entities[1]) entities[1]->Render(&framebuffer, camera, Color::WHITE);
        if (entities.size() >= 3 && entities[2]) entities[2]->Render(&framebuffer, camera, Color::RED);
    }

    framebuffer.Render();
}


void Application::Update(float dt)
{
    if (mode == 1)
    {
        if (entity) entity->Update(dt);
    }
    else if (mode == 2)
    {
        for (Entity* e : entities)
            if (e) e->Update(dt);
    }
}

void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    switch(event.keysym.sym)
    {
        case SDLK_ESCAPE: exit(0);

        case SDLK_1: mode = 1; break; // SINGLE ENTITY
        case SDLK_2: mode = 2; break; // MULTI ANIMATED ENTITIES

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

void Application::OnWheel(SDL_MouseWheelEvent){}
void Application::OnFileChanged(const char* filename){ Shader::ReloadSingleShader(filename); }