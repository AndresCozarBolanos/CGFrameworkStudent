#include "application.h"
#include "mesh.h"
#include "shader.h"
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
    buttons.clear();

    Image imgClear;   imgClear.LoadPNG("../res/images/clear.png");
    Image imgLoad;    imgLoad.LoadPNG("../res/images/load.png");
    Image imgSave;    imgSave.LoadPNG("../res/images/save.png");

    Image imgPencil;  imgPencil.LoadPNG("../res/images/pencil.png");
    Image imgEraser;  imgEraser.LoadPNG("../res/images/eraser.png");
    Image imgLine;    imgLine.LoadPNG("../res/images/line.png");
    Image imgRect;    imgRect.LoadPNG("../res/images/rectangle.png");
    Image imgTri;     imgTri.LoadPNG("../res/images/triangle.png");

    Image imgBlack;   imgBlack.LoadPNG("../res/images/black.png");
    Image imgWhite;   imgWhite.LoadPNG("../res/images/white.png");
    Image imgPink;    imgPink.LoadPNG("../res/images/pink.png");
    Image imgYellow;  imgYellow.LoadPNG("../res/images/yellow.png");
    Image imgRed;     imgRed.LoadPNG("../res/images/red.png");
    Image imgBlue;    imgBlue.LoadPNG("../res/images/blue.png");
    Image imgCyan;    imgCyan.LoadPNG("../res/images/cyan.png");
    Image imgGreen;   imgGreen.LoadPNG("../res/images/green.png");

    int x = 10, y = 10, s = 6;

    buttons.push_back(Button(imgClear,  Vector2((float)x, (float)y), ButtonType::CLEAR)); x += imgClear.width + s;
    buttons.push_back(Button(imgLoad,   Vector2((float)x, (float)y), ButtonType::LOAD));  x += imgLoad.width + s;
    buttons.push_back(Button(imgSave,   Vector2((float)x, (float)y), ButtonType::SAVE));  x += imgSave.width + 4*s;

    buttons.push_back(Button(imgPencil, Vector2((float)x, (float)y), ButtonType::PENCIL)); x += imgPencil.width + s;
    buttons.push_back(Button(imgEraser, Vector2((float)x, (float)y), ButtonType::ERASER)); x += imgEraser.width + s;
    buttons.push_back(Button(imgLine,   Vector2((float)x, (float)y), ButtonType::LINE));   x += imgLine.width + s;
    buttons.push_back(Button(imgRect,   Vector2((float)x, (float)y), ButtonType::RECTANGLE)); x += imgRect.width + s;
    buttons.push_back(Button(imgTri,    Vector2((float)x, (float)y), ButtonType::TRIANGLE));  x += imgTri.width + 4*s;

    buttons.push_back(Button(imgBlack,  Vector2((float)x, (float)y), ButtonType::COLOR_BLACK));  x += imgBlack.width + s;
    buttons.push_back(Button(imgWhite,  Vector2((float)x, (float)y), ButtonType::COLOR_WHITE));  x += imgWhite.width + s;
    buttons.push_back(Button(imgPink,   Vector2((float)x, (float)y), ButtonType::COLOR_PINK));   x += imgPink.width + s;
    buttons.push_back(Button(imgYellow, Vector2((float)x, (float)y), ButtonType::COLOR_YELLOW)); x += imgYellow.width + s;
    buttons.push_back(Button(imgRed,    Vector2((float)x, (float)y), ButtonType::COLOR_RED));    x += imgRed.width + s;
    buttons.push_back(Button(imgBlue,   Vector2((float)x, (float)y), ButtonType::COLOR_BLUE));   x += imgBlue.width + s;
    buttons.push_back(Button(imgCyan,   Vector2((float)x, (float)y), ButtonType::COLOR_CYAN));   x += imgCyan.width + s;
    buttons.push_back(Button(imgGreen,  Vector2((float)x, (float)y), ButtonType::COLOR_GREEN));

    canvas.Fill(backgroundColor);

    particlesMode = false;
}

void Application::Render()
{
    // Particles mode
    if (particlesMode)
    {
        framebuffer.Fill(backgroundColor); // Fill the framebuffer
        ps.Render(&framebuffer); // Render particles
        framebuffer.Render(); // Show
        return;
    }
    // Paint mode
    framebuffer.DrawImage(canvas, 0, 0);
    // Draw toolbar
    framebuffer.DrawRect(0,0,framebuffer.width,toolbarH,Color(60,60,60),1,true,Color(60,60,60));
    for (const Button& b : buttons) b.Render(framebuffer);

    framebuffer.Render(); // Show
}

void Application::Update(float dt)
{
    if (particlesMode)
        ps.Update(dt);
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
        if (isDrawing)
        {
            Color c = (currentTool==Tool::ERASER) ? backgroundColor : currentColor;

            canvas.SetPixel((unsigned int)mouse_position.x, (unsigned int)mouse_position.y, c);

            canvas.DrawLineDDA((int)prevPos.x, (int)prevPos.y,
                            (int)mouse_position.x, (int)mouse_position.y, c);

            prevPos = mouse_position;
        }

    }
}

void Application::OnWheel(SDL_MouseWheelEvent){}
void Application::OnFileChanged(const char* filename){ Shader::ReloadSingleShader(filename); }
