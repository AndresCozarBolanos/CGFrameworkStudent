#include "entity.h"
#include "utils.h"
#include "camera.h"

Entity::Entity(Mesh* m, Matrix44 ma);
{
	this->mesh = m;
	this->modelMatrix = ma;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
{
	//Get mesh
	Mesh* mesh = this->mesh;
	if (!mesh) return;

	//Start itinerateinhg all the triangles
	for (size_t i = 0; i < mesh->GetVertices().size(); i += 3) {
		if (mesh->GetVertices().size() < 3) break;

		//a.Local to screen space
		Vector3 v0 = mesh->GetVertices()[i];
		Vector3 v1 = mesh->GetVertices()[i + 1];
		Vector3 v2 = mesh->GetVertices()[i + 2];

		v0 = modelMatrix * v0;
		v1 = modelMatrix * v1;
		v2 = modelMatrix * v2;

		//b. Project each of the world space vertices to clip space
		v0 = camera->ProjectVector(v0);
		v1 = camera->ProjectVector(v1);
		v2 = camera->ProjectVector(v2);

		//c. Reject points outside the box
		if (p0.x < -1 || p0.x > 1 || p0.y < -1 || p0.y > 1 || p0.z < -1 || p0.z > 1) continue;
		if (p1.x < -1 || p1.x > 1 || p1.y < -1 || p1.y > 1 || p1.z < -1 || p1.z > 1) continue;
		if (p2.x < -1 || p2.x > 1 || p2.y < -1 || p2.y > 1 || p2.z < -1 || p2.z > 1) continue;

		//d. Convert from clip space to screen space
		float w = (float)framebuffer->width;
		float h = (float)framebuffer->height;

		Vector2 s0, s1, s2;
		s0.x = (p0.x + 1.0f) * 0.5f * w;
		s0.y = (1.0f - (p0.y + 1.0f) * 0.5f) * h; 

		s1.x = (p1.x + 1.0f) * 0.5f * w;
		s1.y = (1.0f - (p1.y + 1.0f) * 0.5f) * h;

		s2.x = (p2.x + 1.0f) * 0.5f * w;
		s2.y = (1.0f - (p2.y + 1.0f) * 0.5f) * h;

		framebuffer->DrawLineDDA((int)s0.x, (int)s0.y, (int)s1.x, (int)s1.y, c);
		framebuffer->DrawLineDDA((int)s1.x, (int)s1.y, (int)s2.x, (int)s2.y, c);
		framebuffer->DrawLineDDA((int)s2.x, (int)s2.y, (int)s0.x, (int)s0.y, c);
	}
}