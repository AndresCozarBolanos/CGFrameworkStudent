#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include <algorithm>

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
	buttons.clear();

	// We load the images
	Image imgClear; imgClear.LoadPNG("../res/images/clear.png");
	Image imgLoad; imgLoad.LoadPNG("../res/images/load.png");
	Image imgSave; imgSave.LoadPNG("../res/images/save.png");
	Image imgEraser; imgEraser.LoadPNG("../res/images/eraser.png");
	Image imgLine; imgLine.LoadPNG("../res/images/line.png");
	Image imgRect; imgRect.LoadPNG("../res/images/rectangle.png");
	Image imgCircle; imgCircle.LoadPNG("../res/images/circle.png");
	Image imgTri; imgTri.LoadPNG("../res/images/triangle.png");
	Image imgBlack; imgBlack.LoadPNG("../res/images/black.png");
	Image imgWhite; imgWhite.LoadPNG("../res/images/white.png");
	Image imgPink; imgPink.LoadPNG("../res/images/pink.png");
	Image imgYellow; imgYellow.LoadPNG("../res/images/yellow.png");
	Image imgRed; imgRed.LoadPNG("../res/images/red.png");
	Image imgBlue; imgBlue.LoadPNG("../res/images/blue.png");
	Image imgCyan; imgCyan.LoadPNG("../res/images/cyan.png");

	// Positioning variables
	int margin = 10;
	int spacing = 6;
	int x = margin;
	int y = margin;

	// We create the buttons with the recently loaded images
	// moving then a bit to the right each time
	buttons.push_back(Button(imgClear, Vector2((float)x, (float)y), ButtonType::CLEAR)); x += imgClear.width + spacing;
	buttons.push_back(Button(imgLoad, Vector2((float)x, (float)y), ButtonType::LOAD)); x += imgLoad.width + spacing;
	buttons.push_back(Button(imgSave, Vector2((float)x, (float)y), ButtonType::SAVE)); x += imgSave.width + spacing;
	buttons.push_back(Button(imgEraser, Vector2((float)x, (float)y), ButtonType::ERASER)); x += imgEraser.width + spacing;
	buttons.push_back(Button(imgLine, Vector2((float)x, (float)y), ButtonType::LINE)); x += imgLine.width + spacing;
	buttons.push_back(Button(imgRect, Vector2((float)x, (float)y), ButtonType::RECTANGLE)); x += imgRect.width + spacing;
	buttons.push_back(Button(imgCircle, Vector2((float)x, (float)y), ButtonType::CIRCLE)); x += imgCircle.width + spacing;
	buttons.push_back(Button(imgTri, Vector2((float)x, (float)y), ButtonType::TRIANGLE)); x += imgTri.width + spacing;
	buttons.push_back(Button(imgBlack, Vector2((float)x, (float)y), ButtonType::COLOR_BLACK));x += imgBlack.width + spacing;
	buttons.push_back(Button(imgWhite, Vector2((float)x, (float)y), ButtonType::COLOR_WHITE)); x += imgWhite.width + spacing;
	buttons.push_back(Button(imgPink, Vector2((float)x, (float)y), ButtonType::COLOR_PINK)); x += imgPink.width + spacing;
	buttons.push_back(Button(imgYellow, Vector2((float)x, (float)y), ButtonType::COLOR_YELLOW)); x += imgYellow.width + spacing;
	buttons.push_back(Button(imgRed, Vector2((float)x, (float)y), ButtonType::COLOR_RED)); x += imgRed.width + spacing;
	buttons.push_back(Button(imgBlue, Vector2((float)x, (float)y), ButtonType::COLOR_BLUE)); x += imgBlue.width + spacing;
	buttons.push_back(Button(imgCyan, Vector2((float)x, (float)y), ButtonType::COLOR_CYAN));

	// Null tool state
	currentTool = Tool::NONE;
	isPlacing = false;
	triClicks = 0;
}

void Application::Render(void)
{
	framebuffer.Fill(Color::BLACK);

	// Tools
	for (const auto& L : lines)
		framebuffer.DrawLineDDA(L.x0, L.y0, L.x1, L.y1, L.c);

	for (const auto& R : rects)
		framebuffer.DrawRect(R.x, R.y, R.w, R.h, R.border, R.bw, R.filled, R.fill);

	for (const auto& T : tris)
		framebuffer.DrawTriangle(T.p0, T.p1, T.p2, T.border, T.filled, T.fill);

	framebuffer.DrawRect(0, 0, framebuffer.width, toolbarH, Color(60,60,60), 1, true, Color(60,60,60));
	for (const Button& b : buttons)
		b.Render(framebuffer);
		
	framebuffer.Render();
}

void Application::Update(float seconds_elapsed)
{	;
}

void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break;

		case SDLK_PLUS:
			borderWidth++;
			break;

		case SDLK_MINUS:
			borderWidth = std::max(1, borderWidth - 1);
			break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button != SDL_BUTTON_LEFT) return;

	// 1) Click en botones
	for (const Button& b : buttons)
	{
		if (!b.IsMouseInside(mouse_position)) continue;

		// Solo usaremos estos por ahora
		if (b.type == ButtonType::LINE) { currentTool = Tool::LINE;     isPlacing = false; triClicks = 0; return; }
		if (b.type == ButtonType::RECTANGLE) { currentTool = Tool::RECT;     isPlacing = false; triClicks = 0; return; }
		if (b.type == ButtonType::TRIANGLE)  { currentTool = Tool::TRIANGLE; isPlacing = false; triClicks = 0; return; }

		// Clear (opcional pero útil)
		if (b.type == ButtonType::CLEAR)
		{
			lines.clear(); rects.clear(); tris.clear();
			isPlacing = false; triClicks = 0;
			return;
		}

		// Ignoramos el resto por ahora
		return;
	}

	// 2) Click fuera de UI: colocar forma
	if (currentTool == Tool::NONE) return;

	// LINE/RECT: empezamos a "colocar" pero NO dibujamos nada hasta soltar
	if (currentTool == Tool::LINE || currentTool == Tool::RECT)
	{
		isPlacing = true;
		startPos = mouse_position;
		endPos = mouse_position;
		return;
	}

	// TRIANGLE: 3 clicks y al tercero aparece (se guarda)
	if (currentTool == Tool::TRIANGLE)
	{
		if (triClicks == 0) { triP0 = mouse_position; triClicks = 1; return; }
		if (triClicks == 1) { triP1 = mouse_position; triClicks = 2; return; }

		// tercer click: ahora SI aparece (guardamos)
		Vector2 p2 = mouse_position;
		tris.push_back({ triP0, triP1, p2, Color::WHITE, false, Color::BLACK });
		triClicks = 0;
		return;
	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button != SDL_BUTTON_LEFT) return;
	if (!isPlacing) return;

	isPlacing = false;

	// Al soltar, ahora SI aparece (guardamos)
	if (currentTool == Tool::LINE)
	{
		lines.push_back({ (int)startPos.x, (int)startPos.y, (int)endPos.x, (int)endPos.y, Color::WHITE });
	}
	else if (currentTool == Tool::RECT)
	{
		int x0 = (int)startPos.x, y0 = (int)startPos.y;
		int x1 = (int)endPos.x,   y1 = (int)endPos.y;

		int x = std::min(x0, x1);
		int y = std::min(y0, y1);
		int w = std::abs(x1 - x0);
		int h = std::abs(y1 - y0);

		rects.push_back({ x, y, w, h, Color::WHITE, borderWidth, false, Color::BLACK });
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	(void)event;

	// Solo actualiza el punto final mientras "colocas"
	if (isPlacing)
		endPos = mouse_position;
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;
	// ...
}

void Application::OnFileChanged(const char* filename)
{
	Shader::ReloadSingleShader(filename);
}
