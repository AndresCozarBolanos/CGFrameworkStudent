#include "entity.h"
#include "utils.h"
#include "application.h"
#include "material.h"
#include <cmath>

Entity::Entity(Mesh* m, const Matrix44& initial_model)
{
    mesh = m;
    model = initial_model;
}

void Entity::Update(float seconds_elapsed)
{
    rotation_angle += rotation_speed * seconds_elapsed;

    Matrix44 S;
    S.SetIdentity();
    S.M[0][0] = scale_value;
    S.M[1][1] = scale_value;
    S.M[2][2] = scale_value;

    Matrix44 R;
    R.SetIdentity();
    R.MakeRotationMatrix(rotation_angle, rotation_axis);

    Matrix44 T;
    T.SetIdentity();
    T.M[3][0] = position.x;
    T.M[3][1] = position.y;
    T.M[3][2] = position.z;

    model = T * R * S;
}

void Entity::Render(sUniformData& uniformData)
{
    if (!mesh || !material)
        return;

    uniformData.model_matrix = model;

    material->Enable(uniformData);
    mesh->Render();
    material->Disable();
}