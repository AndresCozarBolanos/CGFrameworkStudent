#pragma once
#include "framework.h"
#include "mesh.h"

class Entity
{
public:
    Mesh mesh;
    Entity();

    Matrix44 model;
    void Render();
};