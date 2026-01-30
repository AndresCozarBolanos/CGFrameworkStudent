#include "entity.h"
#include "utils.h"
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

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c)
{
    if (!framebuffer || !camera || !mesh) return;

    const std::vector<Vector3>& v = mesh->GetVertices();
    if (v.size() < 3) return;

    const float w = (float)framebuffer->width;
    const float h = (float)framebuffer->height;

    for (size_t i = 0; i + 2 < v.size(); i += 3)
    {
        // Local -> World
        Vector3 v0 = model * v[i];
        Vector3 v1 = model * v[i + 1];
        Vector3 v2 = model * v[i + 2];

        // World -> Clip (NDC)
        Vector3 p0 = camera->ProjectVector(v0);
        Vector3 p1 = camera->ProjectVector(v1);
        Vector3 p2 = camera->ProjectVector(v2);

        // Culling en [-1,1]^3 (2.2)
        if (p0.x < -1 || p0.x > 1 || p0.y < -1 || p0.y > 1 || p0.z < -1 || p0.z > 1) continue;
        if (p1.x < -1 || p1.x > 1 || p1.y < -1 || p1.y > 1 || p1.z < -1 || p1.z > 1) continue;
        if (p2.x < -1 || p2.x > 1 || p2.y < -1 || p2.y > 1 || p2.z < -1 || p2.z > 1) continue;

        // NDC -> Screen
        // IMPORTANTE: en tu framework NO se invierte Y aquí
        Vector2 s0, s1, s2;

        s0.x = (p0.x * 0.5f + 0.5f) * w;
        s0.y = (p0.y * 0.5f + 0.5f) * h;

        s1.x = (p1.x * 0.5f + 0.5f) * w;
        s1.y = (p1.y * 0.5f + 0.5f) * h;

        s2.x = (p2.x * 0.5f + 0.5f) * w;
        s2.y = (p2.y * 0.5f + 0.5f) * h;

        // Wireframe
        framebuffer->DrawLineDDA((int)s0.x, (int)s0.y, (int)s1.x, (int)s1.y, c);
        framebuffer->DrawLineDDA((int)s1.x, (int)s1.y, (int)s2.x, (int)s2.y, c);
        framebuffer->DrawLineDDA((int)s2.x, (int)s2.y, (int)s0.x, (int)s0.y, c);
    }
}
