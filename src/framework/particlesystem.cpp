#include "ParticleSystem.h"
#include <cstdlib>

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
    // [NUEVO] cuántas partículas podemos crear este frame
    spawnAccumulator += dt * (float)spawnPerSecond;
    int spawnBudget = (int)spawnAccumulator;
    if (spawnBudget > MAX_PARTICLES) spawnBudget = MAX_PARTICLES;
    spawnAccumulator -= (float)spawnBudget;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle& p = particles[i];

        // ===== Respawn SOLO si hay presupuesto =====
        if (p.inactive)
        {
            if (spawnBudget <= 0)
                continue;

            spawnBudget--;

            // Spawn ARRIBA (en tu framework 0,0 es ABAJO)
            p.position = Vector2(
                frand(0.0f, (float)(screenW - 1)),
                frand((float)(screenH - 1 - 20), (float)(screenH - 1))
            );

            // Caer hacia abajo => y negativa
            p.velocity = Vector2(
                frand(-15.0f, 15.0f),
                -frand(40.0f, 120.0f)
            );

            // Color random vivo
            p.color = Color(
                (unsigned char)(60 + rand() % 196),
                (unsigned char)(60 + rand() % 196),
                (unsigned char)(60 + rand() % 196)
            );

            p.acceleration = -80.0f;
            p.ttl = frand(2.0f, 8.0f);
            p.inactive = false;
        }

        if (p.inactive) continue;

        // ===== Integración =====
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.velocity.y += p.acceleration * dt;
        p.ttl -= dt;

        if (p.ttl <= 0.0f || p.position.y < 0.0f || p.position.x < 0.0f || p.position.x >= screenW)
            p.inactive = true;
    }
}


void ParticleSystem::Render(Image* framebuffer)
{
    // Actualiza tamaño real aquí también (por si redimensionas ventana)
    screenW = (int)framebuffer->width;
    screenH = (int)framebuffer->height;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle& p = particles[i];
        if (p.inactive) continue;

        int x = (int)p.position.x;
        int y = (int)p.position.y;

        if (x < 0 || x >= screenW || y < 0 || y >= screenH) continue;

        // ===== Estrella "más grande": 2x2 =====
        framebuffer->SetPixel((unsigned int)x, (unsigned int)y, p.color);
        if (x + 1 < screenW) framebuffer->SetPixel((unsigned int)(x + 1), (unsigned int)y, p.color);
        if (y + 1 < screenH) framebuffer->SetPixel((unsigned int)x, (unsigned int)(y + 1), p.color);
        if (x + 1 < screenW && y + 1 < screenH)
            framebuffer->SetPixel((unsigned int)(x + 1), (unsigned int)(y + 1), p.color);
    }
}
