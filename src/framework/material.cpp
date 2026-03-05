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

    if (diffuse_texture) {
        shader->SetTexture("u_texture", diffuse_texture);
    }
}

void Material::Disable()
{
    if (shader) {
        shader->Disable();
    }
}