#pragma once

#include "framework.h" 
#include "shader.h"
#include "texture.h"
#include <vector>

struct sLight {
    Vector3 position;
    Vector3 diffuse_color; 
};

struct sUniformData {
    Matrix44 viewprojection_matrix;
    Matrix44 model_matrix;
    Vector3 camera_position; 
    Vector3 ambient_light;
    std::vector<sLight> lights;
};

class Material {
public:
    Shader* shader;

    Texture* diffuse_texture;  
    Texture* specular_texture; 
    Texture* normal_texture;   

    Vector3 color;       
    float shininess;     

    Material();
    ~Material();

    void Enable(const sUniformData& uniformData);
    void Disable();
};