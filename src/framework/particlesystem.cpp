#include "ParticleSystem.h"
#include <cstdlib>
#include <algorithm>

int screenW = 0;
int screenH = 0;
int spawnPerSecond = 200;
float spawnAccumulator = 0.0f;

static float frand(float a, float b)
{
    return a + (b - a) * (float(rand()) / float(RAND_MAX));
}

static void SpawnParticle(ParticleSystem::Particle& p, int screenW, int screenH)
{
    // Particles spawn in random position along the top
    p.position = Vector2(
        frand(0.0f, (float)(screenW - 1)),
        frand((float)(screenH - 1 - 20), (float)(screenH - 1))
    );

    // velocity
    p.velocity = Vector2(
        frand(-15.0f, 15.0f),
        -frand(40.0f, 120.0f)
    );

    // random bright color
    p.color = Color(
        (unsigned char)(60 + rand() % 196),
        (unsigned char)(60 + rand() % 196),
        (unsigned char)(60 + rand() % 196)
    );

    p.acceleration = -80.0f;
    p.ttl = frand(2.0f, 8.0f);
    p.inactive = false;
}

void ParticleSystem::Init()
{
    spawnAccumulator = 0.0f;

    for (int i = 0; i < MAX_PARTICLES; ++i)
        particles[i].inactive = true;

    // spawn some initial particles
    for (int i = 0; i < std::floor(MAX_PARTICLES / 10); ++i)
        SpawnParticle(particles[i], screenW, screenH);
}

void ParticleSystem::Update(float dt)
{
    // compute how many particles to spawn this frame
    spawnAccumulator += dt * (float)spawnPerSecond;
    int spawnBudget = (int)spawnAccumulator;
    spawnBudget = std::min(spawnBudget, MAX_PARTICLES);
    spawnAccumulator -= (float)spawnBudget;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle& p = particles[i];

        // spawn into inactive slots while we still have budget
        if (p.inactive)
        {
            if (spawnBudget <= 0) continue;

            spawnBudget--;
            SpawnParticle(p, screenW, screenH);
        }

        if (p.inactive) continue;

        // integrate motion
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.velocity.y += p.acceleration * dt;
        p.ttl -= dt;

        // kill particle if expired or out of bounds
        if (p.ttl <= 0.0f ||
            p.position.y < 0.0f ||
            p.position.x < 0.0f || p.position.x >= (float)screenW)
        {
            p.inactive = true;
        }
    }
}

void ParticleSystem::Render(Image* framebuffer)
{
    // update size from framebuffer
    screenW = (int)framebuffer->width;
    screenH = (int)framebuffer->height;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle& p = particles[i];
        if (p.inactive) continue;

        int x = (int)p.position.x;
        int y = (int)p.position.y;

        // draw the particle
        framebuffer->SetPixel((unsigned int)x, (unsigned int)y, p.color);
        // make it a bit bigger becuase otherwise it's hard to see
        if (x + 1 < screenW) framebuffer->SetPixel((unsigned int)(x + 1), (unsigned int)y, p.color);
        if (y + 1 < screenH) framebuffer->SetPixel((unsigned int)x, (unsigned int)(y + 1), p.color);
        if (x + 1 < screenW && y + 1 < screenH)
            framebuffer->SetPixel((unsigned int)(x + 1), (unsigned int)(y + 1), p.color);
    }
}
