#pragma once
#include "framework.h"
#include "image.h"

class ParticleSystem {

    static const int MAX_PARTICLES = 1000;

    struct Particle {
        Vector2 position;
        Vector2 velocity;
        Color color;
        float acceleration;
        float ttl;
        bool inactive;
    };

    Particle particles[MAX_PARTICLES];

    int screenW = 800;
    int screenH = 600;

    int spawnPerSecond = 200;    
    float spawnAccumulator = 0.0f;


public:
    void SetSize(int w, int h) { screenW = w; screenH = h; }
    void Init();
    void Render(Image* framebuffer);
    void Update(float dt);
};
