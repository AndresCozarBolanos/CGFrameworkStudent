#pragma once
#include "mesh.h"
#include "framework.h"

class Entity
{
public:
	Mesh mesh;
	matrix44 model;

	Entity();
	void render();
};
