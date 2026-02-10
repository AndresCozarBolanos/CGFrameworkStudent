#include "entity.h"
#include "utils.h"
#include "application.h"
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

void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer, bool show_tex, bool use_z, bool interp_uv)
{
    if (!framebuffer || !camera || !mesh) return;

    const std::vector<Vector3>& v = mesh->GetVertices();
    const std::vector<Vector2>& uvs = mesh->GetUVs();

    const float w = (float)framebuffer->width;
    const float h = (float)framebuffer->height;

    for (size_t i = 0; i + 2 < v.size(); i += 3)
    {
        Vector3 v0 = model * v[i];
        Vector3 v1 = model * v[i + 1];
        Vector3 v2 = model * v[i + 2];

        Vector3 p0 = camera->ProjectVector(v0);
        Vector3 p1 = camera->ProjectVector(v1);
        Vector3 p2 = camera->ProjectVector(v2);

        if (p0.x < -1 || p0.x > 1 || p0.y < -1 || p0.y > 1 || p0.z < -1 || p0.z > 1) continue;
        if (p1.x < -1 || p1.x > 1 || p1.y < -1 || p1.y > 1 || p1.z < -1 || p1.z > 1) continue;
        if (p2.x < -1 || p2.x > 1 || p2.y < -1 || p2.y > 1 || p2.z < -1 || p2.z > 1) continue;

        Vector3 s0, s1, s2;

        s0.x = (p0.x * 0.5f + 0.5f) * w;
        s0.y = (p0.y * 0.5f + 0.5f) * h;
        s0.z = p0.z;

        s1.x = (p1.x * 0.5f + 0.5f) * w;
        s1.y = (p1.y * 0.5f + 0.5f) * h;
        s1.z = p1.z;

        s2.x = (p2.x * 0.5f + 0.5f) * w;
        s2.y = (p2.y * 0.5f + 0.5f) * h;
        s2.z = p2.z;

        Vector2 uv0 = uvs.size() ? uvs[i] : Vector2(0, 0);
        Vector2 uv1 = uvs.size() ? uvs[i + 1] : Vector2(0, 0);
        Vector2 uv2 = uvs.size() ? uvs[i + 2] : Vector2(0, 0);

        Color c0 = Color::RED;
        Color c1 = Color::GREEN;
        Color c2 = Color::BLUE;

        framebuffer->DrawTriangleInterpolated(s0, s1, s2, c0, c1, c2, zBuffer, texture, uv0, uv1, uv2, use_z, interp_uv && show_tex);
    }
}
