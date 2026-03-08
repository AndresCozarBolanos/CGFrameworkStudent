#include "material.h"

Material::Material()
{
    shader = nullptr;
    diffuse_texture = nullptr;
    specular_texture = nullptr;
    normal_texture = nullptr;

    color = Vector3(1.0f, 1.0f, 1.0f); 
    shininess = 30.0f; 
}

Material::~Material()
{
}

void Material::Enable(const sUniformData& uniformData)
{
    if (!shader) return;

    shader->Enable();

    shader->SetMatrix44("u_model", uniformData.model_matrix);
    shader->SetMatrix44("u_viewprojection", uniformData.viewprojection_matrix);

    shader->SetVector3("u_camera_position", uniformData.camera_position);
    shader->SetVector3("u_ambient_light", uniformData.ambient_light);

    if (!uniformData.lights.empty())
    {
        shader->SetVector3("u_light_position", uniformData.lights[0].position);
        shader->SetVector3("u_light_color", uniformData.lights[0].diffuse_color);
    }

    if (diffuse_texture)
        shader->SetTexture("u_texture", diffuse_texture);

    if (specular_texture)
        shader->SetTexture("u_specular_texture", specular_texture);

    if (normal_texture)
        shader->SetTexture("u_normal_texture", normal_texture);

    shader->SetVector3("u_material_color", color);
    shader->SetFloat("u_shininess", shininess);
}

void Material::Disable()
{
    if (shader) {
        shader->Disable();
    }
}