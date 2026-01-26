#include "entity.h"
#include "utils.h"
#include "camera.h"

Entity::Entity()
{
	this->mesh.Clear();
	this->model.SetIdentity();
}

void Entity::render() 
{
	this->mesh.Render();
}