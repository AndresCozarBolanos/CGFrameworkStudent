#pragma once
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "camera.h"

enum class eRenderMode { WIREFRAME, TRIANGLES };
class Entity
{
public:
    eRenderMode mode = eRenderMode::TRIANGLES;
    //Define every entry
    Mesh* mesh = nullptr;
    Matrix44 model;
    Image* texture = nullptr;

	//Generated properties
    Vector3 position = Vector3(0,0,0);
    Vector3 rotation_axis = Vector3(0,1,0);
    float rotation_speed = 0.0f;   
    float rotation_angle = 0.0f;  
    float scale_value = 1.0f;

    //Functions 
    Entity(Mesh* m, const Matrix44& initial_model);

    void Update(float seconds_elapsed);
    void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer, bool show_tex, bool use_z, bool interp_uv);
};
