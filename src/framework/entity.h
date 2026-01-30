#pragma once
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "camera.h"

class Entity
{
public:
    Mesh* mesh = nullptr;
    Matrix44 model;

    // Parámetros de animación (simples y efectivos)
    Vector3 position = Vector3(0,0,0);
    Vector3 rotation_axis = Vector3(0,1,0);
    float rotation_speed = 0.0f;   // rad/s
    float rotation_angle = 0.0f;   // rad acumulada
    float scale_value = 1.0f;

    Entity(Mesh* m, const Matrix44& initial_model);

    void Update(float seconds_elapsed);
    void Render(Image* framebuffer, Camera* camera, const Color& c);
};
