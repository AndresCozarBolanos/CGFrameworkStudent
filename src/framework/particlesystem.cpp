#include "ParticleSystem.h"
#include <cstdlib>

//generate a aleatory number between a && b
static float frand(float a, float b)
{
    return a + (b - a) * (float(rand()) / float(RAND_MAX));
}

void ParticleSystem::Init()
{
    for (int i = 0; i < MAX_PARTICLES; ++i)
        particles[i].inactive = true;

    spawnAccumulator = 0.0f;
}

void ParticleSystem::Update(float dt)
{
    // Specify the quantity of spawns that we can create, depending of the dt that the accumulator recive
    spawnAccumulator += dt * (float)spawnPerSecond;
    int spawnBudget = (int)spawnAccumulator;
    if (spawnBudget > MAX_PARTICLES) spawnBudget = MAX_PARTICLES;
    spawnAccumulator -= (float)spawnBudget;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle& p = particles[i];

        if (p.inactive) //if the particle is inactive, we can operate
        {
            if (spawnBudget <= 0) //if is not empty, we can continuate with the if
                continue;

            spawnBudget--;

            //colocate the particle in the top
            p.position = Vector2(
                frand(0.0f, (float)(screenW - 1)),
                frand((float)(screenH - 1 - 20), (float)(screenH - 1))
            );

            // a velocity from up to down
            p.velocity = Vector2(
                frand(-15.0f, 15.0f),
                -frand(40.0f, 120.0f)
            );

            p.color = Color(
                (unsigned char)(60 + rand() % 196),
                (unsigned char)(60 + rand() % 196),
                (unsigned char)(60 + rand() % 196)
            );

            //initial values
            p.acceleration = -80.0f;
            p.ttl = frand(2.0f, 8.0f);
            p.inactive = false;
        }

        if (p.inactive) continue;

        //update the values with the formulas
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.velocity.y += p.acceleration * dt;
        p.ttl -= dt;

        //see if the particle is inactive or active, to see if we did an error
        if (p.ttl <= 0.0f || p.position.y < 0.0f || p.position.x < 0.0f || p.position.x >= screenW)
            p.inactive = true;
    }
}


void ParticleSystem::Render(Image* framebuffer)
{
    screenW = (int)framebuffer->width;
    screenH = (int)framebuffer->height;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle& p = particles[i];
        if (p.inactive) continue; //if is active continue with the for

        int x = (int)p.position.x;
        int y = (int)p.position.y;

        if (x < 0 || x >= screenW || y < 0 || y >= screenH) continue;

        //paint the particle and the route of it
        framebuffer->SetPixel((unsigned int)x, (unsigned int)y, p.color);
        if (x + 1 < screenW) framebuffer->SetPixel((unsigned int)(x + 1), (unsigned int)y, p.color);
        if (y + 1 < screenH) framebuffer->SetPixel((unsigned int)x, (unsigned int)(y + 1), p.color);
        if (x + 1 < screenW && y + 1 < screenH)
            framebuffer->SetPixel((unsigned int)(x + 1), (unsigned int)(y + 1), p.color);
    }
}
