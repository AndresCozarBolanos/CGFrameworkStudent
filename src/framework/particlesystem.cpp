#include "ParticleSystem.h"
#include <cmath>
#include <cstdlib> 

void ParticleSystem::Init() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].inactive = true;
    }
}

void ParticleSystem::Update(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].inactive) {
            particles[i].position = Vector2(rand() % 800, rand() % 100); 
            particles[i].velocity = Vector2((rand() % 20 - 10), rand() % 50 + 20);
            particles[i].color = Color::WHITE;
            particles[i].acceleration = 15.0f;
            particles[i].ttl = (rand() % 100) / 10.0f + 2.0f; 
            particles[i].inactive = false;
        }

        particles[i].position.x += particles[i].velocity.x * dt;
        particles[i].position.y += particles[i].velocity.y * dt;

        particles[i].velocity.y += particles[i].acceleration * dt;

        particles[i].ttl -= dt;

        if (particles[i].ttl <= 0 || particles[i].position.y > 600) {
            particles[i].inactive = true;
        }
    }
}

void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].inactive) {
            framebuffer->DrawPixel((int)particles[i].position.x,
                (int)particles[i].position.y,
                particles[i].color);
        }
    }
}