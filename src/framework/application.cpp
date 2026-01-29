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
    camera->LookAt(Vector3(0, 20, 50), Vector3(0, 5, 0), Vector3(0, 1, 0));
    camera->SetPerspective(45.0f, window_width / (float)window_height, 0.1f, 1000.0f); 

    
	Mesh* mesh = new Mesh();
	mesh->LoadOBJ("meshes/lee.obj");

    Matrix44 m;
    m.SetIdentity();
    entity = new Entity(mesh, m);
}

void Application::Render()
{
    framebuffer.Fill(Color::BLACK);
    framebuffer.DrawImage(canvas, 0, 0);
    if (entity) {
        entity->Render(&framebuffer, camera, Color::WHITE);
    }
    framebuffer.Render();
}

void Application::Update(float dt)
{
    camera->UpdateViewMatrix();
    camera->UpdateProjectionMatrix();
}

void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    switch(event.keysym.sym)
    {
        case SDLK_ESCAPE: exit(0);

        case SDLK_1: particlesMode = false; break;
        case SDLK_2: particlesMode = true;  break;

        case SDLK_PLUS:
        case SDLK_KP_PLUS:
            borderWidth++;
            break;
        case SDLK_MINUS:
        case SDLK_KP_MINUS:
            borderWidth = std::max(1,borderWidth-1);
            break;

        case SDLK_f:
            isFilled = !isFilled;
            break;
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
