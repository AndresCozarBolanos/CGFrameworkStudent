#pragma once
#include "mesh.h"
#include "framework.h"
#include "image.h"

class Entity
{
public:
	Mesh* mesh;
	Matrix44 model;

	Entity(Mesh* m, Matrix44 ma);
	void Render(Image* framebuffer, Camera* camera, const Color& c);
};