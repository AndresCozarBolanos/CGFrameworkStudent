/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include <vector> 

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"

class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	int borderWidth = 1;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// lab 1

	// ====== NUEVO: lienzo (esto es lo que se guarda/carga) ======
	Image canvas;                       // MODIFICADO/NUEVO
	Color backgroundColor = Color::BLACK; // NUEVO

	// ====== Tools (las iremos completando) ======
	enum class Tool { NONE, PENCIL, ERASER, LINE, RECT, TRIANGLE };  // MODIFICADO
	Tool currentTool = Tool::NONE;
	// ====== Estado de colocacion (SIN preview) ======

	bool isPlacing = false;
	Vector2 startPos;
	Vector2 endPos;

	// Triangulo por 3 clicks
	int triClicks = 0;
	Vector2 triP0;
	Vector2 triP1;

	// ====== Primitivas guardadas (solo RAM; al reiniciar se borran) ======
	struct LinePrim { int x0, y0, x1, y1; Color c; };
	struct RectPrim { int x, y, w, h; Color border; int bw; bool filled; Color fill; };
	struct TriPrim  { Vector2 p0, p1, p2; Color border; bool filled; Color fill; };

	std::vector<LinePrim> lines;
	std::vector<RectPrim> rects;
	std::vector<TriPrim>  tris;


	// UI

	std::vector<Button> buttons; // list of toolbar buttons
	int toolbarH = 50;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
		// Modified to also resize the canvas
		this->canvas.Resize(width, height);
		this->canvas.Fill(backgroundColor);
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
