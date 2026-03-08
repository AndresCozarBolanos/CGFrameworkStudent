#pragma once
#include "mesh.h"
#include "framework.h"
#include "material.h"

struct sUniformData;

class Entity
{
public:

    // Geometry
    Mesh* mesh = nullptr;

    // Material (shader + textures)
    Material* material = nullptr;

    // Transform
    Matrix44 model;

    Vector3 position = Vector3(0,0,0);
    Vector3 rotation_axis = Vector3(0,1,0);

    float rotation_speed = 0.0f;
    float rotation_angle = 0.0f;
    float scale_value = 1.0f;

    // Constructor
    Entity(Mesh* m, const Matrix44& initial_model);

    // Update transform
    void Update(float seconds_elapsed);

    // Render using GPU pipeline
    void Render(sUniformData& uniformData);
};