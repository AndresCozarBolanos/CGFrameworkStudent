#include "entity.h"
#include "framework.h"

Entity::Entity()
{
    this->model.SetIdentity();
    this->mesh.Clear();
    
}

void Entity::Render()
{
    this->mesh.Render();
}