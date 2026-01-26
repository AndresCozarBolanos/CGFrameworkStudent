#pragma once
#include "framework.h"
#include "image.h"

class ParticleSystem {

    static const int MAX_PARTICLES = 1000;

public:
    struct Particle {
        Vector2 position;
        Vector2 velocity;      // speed and direction
        Color color;
        float acceleration;
        float ttl;             // time to live
        bool inactive;         // reusable slot
    };

    Particle particles[MAX_PARTICLES];

    void Init();
    void Render(Image* framebuffer);
    void Update(float dt);
};
